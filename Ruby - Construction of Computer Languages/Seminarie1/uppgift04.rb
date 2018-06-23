#!/usr/bin/env ruby

def find_it(arr_str)
    current_str = arr_str[0]
  arr_str.length.times do |i|
    #To avoid getting out of index
    if (i < arr_str.length)
      #Apply function to the two indexes in the array
      if yield(arr_str[i], current_str)
        #Set current string to "it"
        current_str = arr_str[i]
      end
    end
  end
  #Return it
    return current_str
end