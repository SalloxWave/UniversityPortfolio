require './avsnitt1.rb'
require './avsnitt2.rb'
require './avsnitt3.rb'
require './avsnitt4.rb'
require 'test/unit'

class TestLabb < Test::Unit::TestCase
	def test_avsnitt1
		#uppgift2
		assert_equal(120, factorial(5))
		assert_equal(2432902008176640000, factorial(20))
	end

	def test_avsnitt2
		#uppgift5
		p=PersonName.new('Adam', 'Sterner')
		assert_equal("Sterner Adam", p.fullname)
		p.fullname = "Pelle Filipsson"
		assert_equal("Filipsson Pelle", p.fullname)

		#uppgift6
		person = Person.new('Adam', 'Sterner', 28)
		assert_equal(28, person.age)
		assert_equal(1989, person.birthyear)
		assert_equal("Sterner Adam", person.name.fullname)

		person.birthyear = 1979
		assert_equal(38, person.age)
		assert_equal(1979, person.birthyear)

		person.age = 48
		assert_equal(48, person.age)
		assert_equal(1969, person.birthyear)

	end

	def test_avsnitt3
		#uppgift7
		assert_equal(13, 7.fib)
		assert_equal(1, 1.fib)
		assert_equal(1, 2.fib)

		#uppgift9
		assert_equal([2,3,1], [1,2,3].rotate_left)
		assert_equal([3,1,2], [1,2,3].rotate_left(2))
	end

	def test_avsnitt4
		#uppgift10
		assert_equal("adam", username("hej: adam"))
		assert_equal("VvV", username("UsarNajm: VvV"))

		#uppgift12
		assert_equal("ABC123", regnr("Regnumret e ABC123"))
		assert_equal(false, regnr("XQV113"))
	end
end
