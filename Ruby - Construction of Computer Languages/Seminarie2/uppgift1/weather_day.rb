class WeatherDay
  attr_reader :day_number
  attr_accessor :min_temperature
  attr_accessor :max_temperature

  def initialize(day_number = 0)
    @day_number = day_number
  end

  def get_temperature_diff
    return (max_temperature - min_temperature).abs
  end
end