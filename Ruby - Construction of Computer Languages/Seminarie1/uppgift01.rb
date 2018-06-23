#!/usr/bin/env ruby

def n_times(count)
  count.times do
    #Perform added block
    yield
  end
end

=begin
#Alternativt:
def n_times(n) 
    n.times { yield }
end
=end 

class Repeat
  attr_reader :count
  def initialize(count)
    @count = count
  end

  def each
    @count.times do
      #Perform added block
      yield
    end
  end
end
