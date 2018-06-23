require './upg1'
require 'test-unit'

class TestLeastDiffGoals < Test::Unit::TestCase
  def test_simple
    assert_equal("Aston_Villa", FootballReader.new('football.txt').least_diff_goals)
  end
end

class TestSortedDiffGoals < Test::Unit::TestCase
  def test_simple
    fb = FootballReader.new('football.txt')
    assert_equal(true, fb.sorted_diff_goals.each_cons(2).all?{|a,b| a[1] >= b[1]})
  end
end

class TestLeastDiffTemp < Test::Unit::TestCase
  def test_simple
    assert_equal("14", WeatherReader.new('weather.txt').least_diff_temperature)
  end
end

class TestSortedDiffTemp < Test::Unit::TestCase
  def test_simple
    fb = WeatherReader.new('weather.txt')
    assert_equal(true, fb.sorted_diff_temperature.each_cons(2).all?{|a,b| a[1] >= b[1]})
  end
end
