#!/usr/bin/env ruby
require 'test-unit'
require './football_team.rb'
require './weather_day.rb'
require './main.rb'

class TestFootballTeam < Test::Unit::TestCase
  def test_initializer
    #Default value
    $football_team = FootballTeam.new
    assert_equal("Unknown", $football_team.name)

    #Init with name
    $football_team = FootballTeam.new("Awesome team")
    assert_equal("Awesome team", $football_team.name)
  end

  def test_goal_diff
    $football_team = FootballTeam.new("Even more awesome team")
    $football_team.made_goals = 10
    $football_team.received_goals = 20
    assert_equal(10, $football_team.get_goal_diff)

    $football_team.made_goals = 100
    $football_team.received_goals = 20
    assert_equal(80, $football_team.get_goal_diff)
  end
end

class TestWeatherDay < Test::Unit::TestCase
  def test_initializer
    #Default value
    $weather_day = WeatherDay.new
    assert_equal(0, $weather_day.day_number)

    #Init with name
    $weather_day = WeatherDay.new(666)
    assert_equal(666, $weather_day.day_number)
  end

  def test_temp_diff
    $weather_day.max_temperature = 10
    $weather_day.min_temperature = 20
    assert_equal(10, $weather_day.get_temperature_diff)

    $weather_day.max_temperature = 100
    $weather_day.min_temperature = 20
    assert_equal(80, $weather_day.get_temperature_diff)
  end
end

class TestGetTeams
  def test_return_values
    #Test correct amount of teams
    teams = get_football_teams('football.txt')
    assert_equal(20, teams.length)

    #Test various team values
    assert_equal(79, teams[0].made_goals)
    assert_equal(36, teams[0].received_goals)

    assert_equal(30, teams[19].made_goals)
    assert_equal(64, teams[19].received_goals)
  end
end

class TestGetWeatherDays
  def test_return_values
    #Test correct amount of teams
    days = get_weather_days('weather.txt')
    assert_equal(30, days.length)

    #Test various team values
    assert_equal(88, days[0].made_goals)
    assert_equal(59, days[0].received_goals)

    assert_equal(90, days[19].made_goals)
    assert_equal(45, days[19].received_goals)
  end
end