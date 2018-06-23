#!/usr/bin/env ruby
# encoding: UTF-8

#Check if valid command line arguments and abort if yes
if ( ARGV[0].to_i < 1024 )
  puts "invalid or missing arguments."
  puts "Usage: \"./proxy.rb [PORT]\""
  puts "  [PORT] must be larger than 1024"
  abort()
end

require 'socket'

#Check if specified string includes any bad words
def has_bad_word(str)
  #Turn string (url or body) into UTF-8 and lower case
  new_str = str.force_encoding("UTF-8").downcase
  bad_words = ["spongebob",
               "britney spears",
               "paris hilton",
               "norrköping"]
  return bad_words.any? { |word| new_str.include?(word) }
end

#Fetches the hostname and path from the specified URL
def host_and_path(url)
  md = url.scan(/\/\/([^\/]*)(.*)/)
  return md[0][0], md[0][1]
end

#Redirect specified client to specified URL
def redirect_client(client, url)
  client.puts("HTTP/1.1 302 Found\r\nLocation: #{url}\r\n\r\n")
end

#Open server on user specified port
port = ARGV[0]
server = TCPServer.open(port)

#Infinite loop to keep server running
loop do
  #Create new thread accepting multiple clients
  Thread.start(server.accept) do |client|
    #Get request from client (BOWSER)
    request = client.gets
    #Split request into http_method, url, http_version
    http_method, url, http_version = request.split
    #Get hostname and path
    hostname, path = host_and_path(url)

    #Check if client's requested URL contains any bad words
    if ( has_bad_word(url) )
      #Redirect client to "net ninny"
      redirect_client(client, 
            "http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error1.html")
    else
      #Open and new socket to the host where this
      #    proxy server acts as client (do the thing!)
      proxy_client = TCPSocket.new(hostname, 80)
      #Send request to web server
      proxy_client.print("#{http_method} #{path} HTTP/1.1\r\nHost: #{hostname}\r\nConnection: close\r\n\r\n")

      #################################################################
      # First we create two booleans that we'll need later.           # 
      # Then we take the first 1024 bytes and put it into the         #
      # variable <line>, then we check if <line> is a string.         #
      # if it is, we check if we're still in the header.              #
      # if yes, then we check if the line contains "Content-Length    #
      # and "Content-Type: text". regardless we set header to false.  #
      # if it does, it means it's a textfile that we can check for    #
      # bad words.                                                    #
      #   if it does contains a bad word, we redirect.                #
      #   if it doesn't, we continue to print it.                     #
      # if it isn't a text-file we've got, we just print it.          #
      # and if the line isn't a string, we've already reached the end #
      # and close the connection.                                     #
      #################################################################
      
      textfile = false
      header = true
      loop do
        line = proxy_client.read(1024)
        if ( line.class != String )
          break
        end
        if ( header )
          if ( line.include?("Content-Length:") &&
               line.include?("Content-Type: text") )
            textfile = true
          end
          header = false
        end
        if ( textfile )
          if ( has_bad_word(line) )
            #Redirect client to "net ninny"
            redirect_client(client, 
             "http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error2.html")
            line = nil
          end
        end
        client.print(line)
      end
    end
    client.close
  end
end
