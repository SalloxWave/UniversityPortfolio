#!usr/bin/env ruby
require './football_team'
require './weather_day'

def get_football_teams(filename)
  teams = []

  #Read whole file
  football_data = File.read(filename)

  #Get team name from regex
  regex = /\d\.\s(\w+)/
  name_data = football_data.scan(regex)

  #Get made goals and received goals
  regex = /(\d+)\s+-\s+(\d+)/
  goal_data = football_data.scan(regex)

  #Go through all teams
  name_data.length.times do |i|
    #Set name of team
    team = FootballTeam.new(name_data[i][0])

    #Set made and received goals for team
    team.made_goals = goal_data[i][0].to_i
    team.received_goals = goal_data[i][1].to_i

    #Add team to list of teams
    teams.push(team)
  end

  return teams
end

def print_football_teams(football_teams)
  puts '-'*30
  #Decide team with least difference in made and received goals
  min_diff = football_teams.min_by { |x| x.get_goal_diff }

  #Display data about team with least difference in made and received goals
  puts "Least difference of made and received goals:"
  puts "#{min_diff.name}: #{min_diff.get_goal_diff}"
  print "\n"

  #Sort teams by difference between made and received goals
  football_teams.sort_by! { |x| x.get_goal_diff }

  puts 'Difference between made and received goals sorted in ascending order:'

  #Display the sorted data
  football_teams.length.times do |i|
    puts "#{football_teams[i].name}: #{football_teams[i].get_goal_diff}"
  end
end

def get_weather_days(filename)
  weather_days = []

  #Read whole file
  weather_data = File.read(filename)

  #Get highest and lowest temperature
  regex = /^\s+\d+\s+(\d+)\s+(\d+)/
  temperature_data = weather_data.scan(regex)

  #Go through all weather days
  temperature_data.length.times do |i|
    #Create a new day.
    wd = WeatherDay.new(i+1)

    #Set min- and max temperature.
    wd.min_temperature = temperature_data[i][0].to_i
    wd.max_temperature = temperature_data[i][1].to_i

    #Add day to list of days
    weather_days.push(wd)
  end

  return weather_days
end

def print_weather_days(weather_days)
  puts '-'*30
  #Decide day with with least difference in temperature
  min_diff = weather_days.min_by { |x| x.get_temperature_diff }

  #Display weather day with the least difference in temperature
  puts "Least difference of temperature: "
  puts "Day number #{min_diff.day_number}: #{min_diff.get_temperature_diff}"
  print "\n"

  #Sort weather days by difference between highest and lowest temperature
  weather_days.sort_by! { |x| x.get_temperature_diff }

  puts 'Difference between highest and lowest temperature sorted in ascending order:'

  #Display the sorted data
  weather_days.length.times do |i|
    puts "Day number #{weather_days[i].day_number}: #{weather_days[i].get_temperature_diff}"
  end
end

#Display football data
football_teams = get_football_teams("football.txt")
print_football_teams(football_teams)

#Display weather data
weather_days = get_weather_days("weather.txt")
print_weather_days(weather_days)