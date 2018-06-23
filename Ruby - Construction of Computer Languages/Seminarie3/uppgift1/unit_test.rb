#!/usr/bin/env ruby
require 'test-unit'
require './policy_parser.rb'

class TestPerson < Test::Unit::TestCase

  def setup
    @person = Person.new("VoLvO", "58435", 2, "mAle", 32)
  end

  def test_init
    assert_equal("volvo", @person._car_brand)
    assert_equal("58435", @person._zip_code)
    assert_equal(2, @person._license_years)
    assert_equal("male", @person._gender)
    assert_equal(32, @person._age)    
  end

  def test_points
    #(5 + 4 + 1 + 4.5) * 0.9 * 1.2 = 15.66
    assert_equal(15.66, @person.evaluate_policy("policy.rb"))

    #3 + 0 + 4.5 + 1 + 5 = 13.5½
  	person = Person.new("Fiat", "57002", 10, "female", 45)
    assert_equal(13.5, person.evaluate_policy("policy.rb"))    

    #0 + 0 + 0 + 4.5 = 4.5
    person = Person.new("DeLorean", "61237", -1, "genderfluid", 38)
    assert_equal(4.5, person.evaluate_policy("policy.rb"))    
  end
end