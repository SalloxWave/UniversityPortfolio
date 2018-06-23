#Uppgift7
class Integer
	def fib
		if self <= 0
			0
		elsif self == 1
			1
		else
			(self-1).fib + (self-2).fib
		end
	end
end


#Uppgift9
class Array
	def rotate_left(n=1)
		unless n <= 0
			self.push(self.shift).rotate_left(n-1)
		else
			self
		end
	end
end

