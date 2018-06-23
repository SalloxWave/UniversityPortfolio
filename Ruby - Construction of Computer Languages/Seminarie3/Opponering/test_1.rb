require 'test/unit'
require './uppgift1'
class TC_MyTest < Test::Unit::TestCase

  def setup
    @erik = Person.new("Volvo", "58435", 2, "Man", 32)
    @gud = Person.new("BMW", "58726", 666666, "Woman", 99999)
    @jesus = Person.new("Fiat", "58647", 1337, "Man", 99998)
    @moses = Person.new("Mercedes", "58937", 5, "Man", 40)
    @mohammed = Person.new("Opel", "18130", 22, "Woman", 23)
    @trump = Person.new("Cadillac", "Washington, DC 20500", 50, "Man", 70)
  end
  
  def test_1
    assert_equal(15.66, @erik.evaluate_policy("policy.rb"))
  end

  def test_2
    assert_equal(11, @gud.evaluate_policy("policy.rb"))
  end

  def test_3
    assert_equal(7, @jesus.evaluate_policy("policy.rb"))
  end

  def test_4
    assert_equal(24.5, @moses.evaluate_policy("policy.rb"))
  end

  def test_5
    assert_equal(13, @mohammed.evaluate_policy("policy.rb"))
  end

  def test_5
    assert_equal(10, @trump.evaluate_policy("policy.rb"))
  end
end
