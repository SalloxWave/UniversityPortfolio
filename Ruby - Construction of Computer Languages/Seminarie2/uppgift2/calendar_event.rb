class Calendar_Event
  attr_accessor :summary
  attr_accessor :date
  attr_accessor :location
  attr_accessor :submitter
  attr_accessor :description

  def initialize(summary="Unspecified", date="Unspecified")
    @summary = summary
    @date = date
  end

  def print
    puts '-'*30
    puts "Title: #{summary}"
    puts "Date: #{date}"
    puts "Location: #{location}"
    puts "Submitter: #{submitter}"
    puts "Description: #{description}"
    puts
  end
end
