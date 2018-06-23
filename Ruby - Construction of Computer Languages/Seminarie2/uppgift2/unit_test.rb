#!/usr/bin/env ruby
require 'test-unit'
require './calendar_event'
require './event.rb'

class TestCalenderEvent < Test::Unit::TestCase
  def test_initializer
    $event = Calendar_Event.new("My summary", "2016-05-12")
    assert_equal("My summary", $event.summary)
    assert_equal("2016-05-12", $event.date)
  end

  def test_setters
    $event.summary = "Better summary"
    assert_equal("Better summary", $event.summary)
    $event.date = "2017-12-24"
    assert_equal("2017-12-24", $event.date)
    $event.location = "C4"
    assert_equal("C4", $event.location)
    $event.submitter= "Me"
    assert_equal("Me", $event.submitter)
    $event.description = "We are going to something this day, it's an awesome event!"
    assert_equal("We are going to something this day, it's an awesome event!", $event.description)
  end
end

class TestGetEvents < Test::Unit::TestCase
  def test_returned_events
    #Test if correct amount of events was found
    events = get_events("events.html")
    assert_equal(8, events.length)
    
    #Test if event data has been correctly stored
    assert_equal("The Dark Carnival - 101.9FM", events[0].summary)    
    assert_equal("2008-01-06", events[1].date)    
    assert_equal("248 Front Street Belleville Ontario", events[1].location)    
    assert_equal("Kingston Ontario", events[2].location)
    assert_equal("LunaSlave", events[3].submitter)
    assert_equal("Gothic, Industrial, Dark Alternative w/ DJ LunaSlave", events[5].description)
  end
end
