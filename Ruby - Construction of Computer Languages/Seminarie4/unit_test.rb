#!/usr/bin/env ruby

require 'test-unit'
require './upp1_temperature.rb'
require './constraint-parser.rb'

class TestMath < Test::Unit::TestCase

  def setup
    @a = Connector.new('a')
    @b = Connector.new('b')
    @c = Connector.new('c')
  end
  
  def test_add
    #Create awesome stuff
    #a+b=c
    Adder.new(@a, @b, @c)
    #3+5=8
    @a.user_assign(3)
    @b.user_assign(5)
    assert_equal(3, @a.value)
    assert_equal(5, @b.value)
    assert_equal(8, @c.value)
    #a+5=c
    @a.forget_value('user')
    assert_equal(false, @a.value)
    assert_not_equal(false, @b.value)
    assert_equal(false, @c.value)
    #a+5=17
    @c.user_assign(17)
    #12+5=17
    assert_equal(12, @a.value)
    assert_equal(17, @c.value)
    @c.forget_value('user')
    assert_equal(false, @a.value)
    assert_equal(false, @c.value)
  end

  def test_multiplier
    puts
    #Multiplier 
    #a*b=c
    Multiplier.new(@a, @b, @c)
    #3*5=15
    @a.user_assign(3)
    @b.user_assign(5)
    assert_equal(3, @a.value)
    assert_equal(5, @b.value)
    assert_equal(15, @c.value)
    # a*5=c
    @a.forget_value('user')
    assert_equal(false, @a.value)
    assert_not_equal(false, @b.value)
    assert_equal(false, @c.value)
    # a*5=75
    @c.user_assign(75)
    assert_equal(15, @a.value)
    assert_equal(5, @b.value)
    assert_equal(75, @c.value)
    @c.forget_value('user')
    assert_equal(false, @a.value)
    assert_equal(false, @c.value)
  end
  
end


class TestTemperature < Test::Unit::TestCase
  def setup
    puts
    @c, @f = celsius2fahrenheit
  end

  def test_temperature
    @c.user_assign(100)
    assert_equal(100, @c.value)
    assert_equal(212, @f.value)

    @c.user_assign(0)
    assert_equal(0, @c.value)
    assert_equal(32, @f.value)

    @c.forget_value("user")
    assert_equal(false, @c.value)
    assert_equal(false, @f.value)

    @f.user_assign(100)
    assert_equal(100, @f.value)
    assert_equal(37, @c.value)  
  end  
end


class TestParser < Test::Unit::TestCase
  def setup
    puts
    cp=ConstraintParser.new
    @c, @f=cp.parse "9*c=5*(f-32)"
  end

  def test_parser
    @f.user_assign 0
    assert_equal(0, @f.value)
    assert_equal(-18, @c.value)
    @f.user_assign 100
    assert_equal(100, @f.value)
    assert_equal(37, @c.value)
    @f.forget_value 'user'
    @c.user_assign 0
    assert_equal(0, @c.value)
    assert_equal(32, @f.value)
    @c.user_assign 100
    assert_equal(100, @c.value)
    assert_equal(212, @f.value)
    @c.user_assign 800
    assert_equal(800, @c.value)
    assert_equal(1472, @f.value)
    

  end
end


# cp=ConstraintParser.new
# c,f=cp.parse "9*c=5*(f-32)"
