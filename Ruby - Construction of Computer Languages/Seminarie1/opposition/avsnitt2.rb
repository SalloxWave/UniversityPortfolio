#uppgift5
require 'date'

class PersonName
	def initialize (name, surname)
		@name = name
		@surname = surname
	end

	def fullname
		"#{@surname} #{@name}"
	end

	def fullname=(fullname)
		@name, @surname = fullname.split
	end
end

#uppgift6

class Person
	def initialize(name="Adam", surname="Sterner", age=28)
		@name = PersonName.new(name, surname)
		@age = age
	end

	attr_reader :name
	attr_accessor :age

	def birthyear
		DateTime.now.year - @age
	end

	def birthyear=(year)
		@age = DateTime.now.year - year
	end

end

