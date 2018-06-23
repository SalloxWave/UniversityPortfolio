#!/usr/bin/env ruby

def longest_string(str_arr)
    current_longest = str_arr[0]
    str_arr.length.times do |i|
        #Next string item is longer than current longest length
        if (str_arr[i].length > current_longest.length)
            #Find length of currently longest string item
            current_longest = str_arr[i]
        end
    end
    return current_longest
end