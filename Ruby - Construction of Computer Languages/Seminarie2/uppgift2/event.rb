require 'rexml/document'
require './calendar_event'

def get_events(filename)
  events = []
  event_file = File.open(filename)
  doc = REXML::Document.new(event_file)

  #Find all events
  doc.root.elements.each ('.//[@class="vevent"]') do |elem|
    event = Calendar_Event.new
    event.summary = elem.get_elements('.//[@class="summary"]')[0].text
    event.date = elem.get_elements('.//[@class="dtstart"]')[0].text

    #Get and set location (adr) info
    loc = ""
    elem.get_elements('.//[@class="adr"]/span').each do |e|
      loc += "#{e.text} "
    end
    #Remove last whitespace of string 
    if (loc[-1] == " ")
        loc.slice!(-1)
    end
    event.location = loc

    event.submitter = elem.get_elements('.//[@class="userLink "]')[0].text
    #Need "/*" to allow <p> tags
    event.description = elem.get_elements('.//[@class="description"]/*')[0].text
    
    #Add event to list of events
    events << event
  end
  return events
end
