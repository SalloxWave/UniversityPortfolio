#uppgift1
def n_times(n)
	for i in 1..n
		yield
	end
end

class Repeat
	def initialize(count)
		@count = count
	end

	def each
		for i in 1..@count
			yield
		end
	end
end

#uppgift2
def factorial(n)
	(1..n).inject { | result, entry | result * entry }
end
