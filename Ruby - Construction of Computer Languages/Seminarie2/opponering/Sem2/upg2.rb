require "hpricot" # need hpricot and open-uri
require "open-uri"

class Event_Reader
    attr_reader :events
    def initialize(url = nil)
        @events = Array.new
        read(url) unless url.nil?
    end
    def read(url = nil)
        return false if url.nil?
        doc = Hpricot(open(url))
        doc.search("div.vevent").each do | entry |
                                        desc = entry.search("p").text
                                        title = entry.search("span.summary").text
                                        date = entry.search("span.dtstart").text

                                        event = Event.new(date, title, desc)
                                        @events.push(event) unless event.nil?
                                      end

    end
end

class Event
    attr_reader :title, :date, :desc
    def initialize(date = nil, title = nil, desc = nil)
        @date = date
        @title = title
        @desc = desc
    end
    def to_s
        "Title: #{title} \nDate: #{date} \nDesc: #{desc}"
    end
end

event_reader = Event_Reader.new("https://www.ida.liu.se/~TDP007/material/seminarie2/events.html")
event_reader.events.each do | event | puts event.to_s, "\n" end
