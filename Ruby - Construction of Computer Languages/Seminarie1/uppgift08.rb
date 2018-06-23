#!/usr/bin/env ruby

class String
  def acronym
    tmp = self.split(" ")
    #Remove special letters by matching
    #special character and words beginning with special matches
    tmp = self.gsub(/[^a-zA-Z\s]\w+|[^a-zA-Z\s]/, '')

    #Split the words into an array
    word_arr = tmp.split(" ")

    result = ""
    #Go through words
    word_arr.length.times do |i|
        #Add first letter of word
        result+=word_arr[i][0]
    end
    #Return acronym converted to uppercase
    return result.upcase
  end
end