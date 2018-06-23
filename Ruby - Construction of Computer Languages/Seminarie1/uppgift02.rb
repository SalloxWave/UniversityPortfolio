#!/usr/bin/env ruby

#Alt solution
  #return (1..n).inject(:*) || 1

def faculty(n)
  return (1..n).inject(1, :*)
end