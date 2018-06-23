#!/usr/bin/env ruby

class Array
  def rotate_left(steps = 1)
    #If there's more than one round in array
    if ( steps > self.length )
      #Calculate new amount of steps needed
      steps %= self.length
    end
    self.length.times do |i|
      #Calculate where item should be inserted
      pos = i-steps
      #Move item specified steps to the left
      self.insert(pos, self[i])
      #Clean all unique numbers ()
      self.uniq!
    end     
  end
end