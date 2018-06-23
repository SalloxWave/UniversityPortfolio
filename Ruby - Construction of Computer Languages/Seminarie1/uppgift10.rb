#!/usr/bin/env ruby

def get_username(str)
  #Capture username found after "USERNAME: "
  regex = /USERNAME:\s(\w+)/
  md = regex.match(str)
  #A username was found
  if ( md != nil )
      return md.captures[0]
  end
  #Return empty string is no username was found
  return ""
end