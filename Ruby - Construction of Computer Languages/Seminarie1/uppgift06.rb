#!/usr/bin/env ruby

require 'date'

class PersonName
  attr_accessor :fullname
  attr_reader :name
  attr_reader :surname

  def fullname
    return "#{@surname} #{@name}"
  end

  def fullname=(fullname)
    #Set first name as first name of full name and capitalize the name
    @name = fullname.split(" ")[0].capitalize
    #Set surname as second name of full name and capitalize the name
    @surname = fullname.split(" ")[1].capitalize
  end
end

class Person
  attr_reader :age
  attr_reader :birthyear
  attr_reader :names

  def initialize(name="Unknown", surname="Unknown", age=0)
    #Create name of the person
    @names = PersonName.new
    @names.fullname = "#{name} #{surname}"
    @age = age
    #Set birthyear according to difference of today's year and new age
    @birthyear = Date.today.year - age
  end

  def age=(new_age)
    @age = new_age
    #Update birthyear according to difference of today's year and new age
    @birthyear = Date.today.year - new_age
  end

  def birthyear=(new_birthyear)
    #Update age according to difference of old and new birthyear
    @age += (@birthyear - new_birthyear)
    @birthyear = new_birthyear
  end

  def fullname
    @names.fullname
  end
  
  def name
    @names.name
  end

  def surname
    @names.surname
  end
end