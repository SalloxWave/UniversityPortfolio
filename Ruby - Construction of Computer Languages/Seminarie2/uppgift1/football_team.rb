class FootballTeam
  attr_reader :name
  attr_accessor :made_goals
  attr_accessor :received_goals

  def initialize(name = "Unknown")
    @name = name
  end

  def get_goal_diff
    return (made_goals - received_goals).abs
  end
end