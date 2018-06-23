#!/usr/bin/env ruby
require './key_words.rb'

class Person
  attr_reader :_car_brand
  attr_reader :_zip_code
  attr_reader :_license_years
  attr_reader :_gender
  attr_reader :_age

  private
  attr_accessor :points
  attr_accessor :key_words

  public
  def initialize(car_brand, zip_code, license_years, gender, age)
    @_car_brand = car_brand.downcase
    @_zip_code = zip_code.downcase
    @_license_years = license_years
    @_gender = gender.downcase
    @_age = age

    @points = 0

    @key_words = Key_Words.load("key_words_db.rb")
  end

  def evaluate_policy(policy_filename)
    self.instance_eval(File.read(policy_filename))    
    return points.round(2)
  end

  private
  def method_missing(name, *args)
    args[0].downcase!

    #Convert value to range if it is a valid range value
    args[0] = Range.new(*args[0].split("-").map(&:to_i)) if args[0].match(/\d+-\d+/)
 
    #Check if value in class matches policy value
    if ( args[0] === eval("@_#{name}") )
      #Change to dot if needed
      args[1].gsub!(',', '.')
      @points+=args[1].to_f  #Float conversion
    end
  end

  def rule(condition, modifier)
    condition.downcase!

    #Parse condition to valid Ruby code    
    parse_condition(condition)

    #Multiply points by modifier if condition is true
    @points*=modifier.to_f if eval("#{condition}")
  end

  def parse_condition(condition)
    #Change DSL keywords to valid Ruby keywords
    @key_words.each do |key, value|
      condition.gsub!(key, value)     
    end
    
    #Move "start_with?"-function one step to the left
    condition.gsub!(/\s\.start_with/, ".start_with")

    #Change variables in DSL code to valid instance variables
    parse_var(condition)
  end

  def parse_var(str)
    #Go through all instance variables
    instance_variables.each do |var|
      #Fetch the "name" of the variable
      var_name = var.to_s.gsub('@_','')
      var_parts = var_name.split('_')

      #Variable had multiple words
      if ( var_parts.length > 1 )
        #Add an underscore to variable name
        str.gsub!(/#{var_parts[0]}(.*?)#{var_parts[1]}/, 
                  "#{var_parts[0]}_#{var_parts[1]}")
      end

      #Convert all variable names in condition to valid instance variables
      str.gsub!(Regexp.new(var_name), "@_#{var_name}")
    end
  end
end