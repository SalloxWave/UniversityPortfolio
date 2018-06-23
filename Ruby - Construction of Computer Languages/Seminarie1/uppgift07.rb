#!/usr/bin/env ruby

class Integer
  def fib
    if (self <= 2)
      return 1
    end
    #Start at the first and second number of fibonacci series
    first = 1
    second = 1
    next_fib = 0
    #To ignore first two numbers
    count = self - 2
    count.times do
      next_fib = first + second
      first = second
      second = next_fib
    end
    return next_fib

=begin Recursive solution    
    if (self <= 2)
      return 1
    end
    return (self-1).fib + (self-2).fib if self > 1
=end
  end
end