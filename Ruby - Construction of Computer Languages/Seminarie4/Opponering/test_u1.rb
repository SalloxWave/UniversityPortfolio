require "./u1.rb"
require "test/unit"

#Uppgift 1
class TestTemperature < Test::Unit::TestCase
  def test_celsius_to_fahrenheit
    c,f = c2f

    c.user_assign(100)
    assert_equal(212.0, f.value)
    c.forget_value("user")
   
    f.user_assign(50)
    assert_equal(10.0, c.value)
    f.forget_value("user")
   
    c.user_assign(0)
    assert_equal(32.0, f.value)
    c.forget_value("user")
  end
end
