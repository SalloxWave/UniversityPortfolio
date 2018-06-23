#!/usr/bin/env ruby

def regnr(str)
  #Create regex matching 3 valid letters followed by 3 numbers
  regex = /[A-HJ-PR-UW-X]{3}\d{3}/
  md = regex.match(str)

  #No valid reg number returns false
  if (md == nil)
    return false
  end
  #Return first match of valid reg number
  return md[0]
end