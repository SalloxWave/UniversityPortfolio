#!/usr/bin/env ruby

class PersonName
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