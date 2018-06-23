require './upg2'
require 'test-unit'

class TestHtmlReader < Test::Unit::TestCase
  def test_simple
    url = "https://www.ida.liu.se/~TDP007/material/seminarie2/events.html"
    assert_equal("The Dark Carnival - 101.9FM", Event_Reader.new(url).events[0].title)
    assert_equal("2008-01-04 10:00pm EST", Event_Reader.new(url).events[0].date)
    assert_equal("\"The Dark Carnival is two hours of spooky goodness. Every week, expect the best goth, industrial, and other dark music, as well as news and reviews from the wider world of goth culture. Embrace the darkness! Fridays, from 10 PM until the Witching Hour.\"", Event_Reader.new(url).events[0].desc)
    assert_equal("Sinister Sundays", Event_Reader.new(url).events[1].title)
  end
  def test_bad_cases
      assert_raise(TypeError) { Event_Reader.new(123123) }
      assert_equal(false, Event_Reader.new().read() )
  end
end
