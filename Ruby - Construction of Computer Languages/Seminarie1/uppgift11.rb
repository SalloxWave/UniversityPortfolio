#!/usr/bin/env ruby
require 'open-uri.rb'

def tag_names(str)
  #Create regex to match all tags
  regex = /<(\w+)[^>]*>|<\/(\w+)[^>]*>/
  #Get all matches
  groups = str.scan(regex)
  tags = []
  groups.length.times do |i|
    #Add captured tag from regex match by removing all nil captures
    tags << groups[i].compact[0]
  end
  return tags
end
