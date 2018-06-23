#!/usr/bin/env ruby

#Check if valid command line arguments and abort if yes
if ( ARGV[0].to_i < 1024 )
  puts "invalid or missing arguments."
  puts "Usage: \"./proxy.rb [PORT]\""
  puts "  [PORT] must be larger than 1024"
  abort()
end

require 'socket'

#Send output to log-file.
$stdout.reopen("proxy.log", "w")
$stdout.sync = true

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
  puts "#{client} Redirecting..."
  client.puts("HTTP/1.1 302 Found\r\nLocation: #{url}\r\n\r\n")
end

#Open server on user specified port
port = ARGV[0]
server = TCPServer.open(port)

clientcount = 0
#Infinite loop to keep server running
loop do
  #Create new thread accepting multiple clients
  Thread.start(server.accept) do |client|
    clientcount+=1
    clientid ="#{clientcount} (#{Time.now}): "
    #Get request from client (BOWSER)
    request = client.gets
    #Split request into http_method, url, http_version
    http_method, url, http_version = request.split
    #Get hostname and path
    hostname, path = host_and_path(url)

    puts "#{clientid} Was opened at [#{Time.now}]"
    
    #Check if client's requested URL contains any bad words
    if ( has_bad_word(url) )
      #Redirect client to "net nanny"
      puts "#{clientid} Bad word found in URL. #{url}"
      redirect_client(client, 
            "http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error1.html")
    else
      #Open and new socket to the host where this
      #    proxy server acts as client (do the thing!)
      proxy_client = TCPSocket.new(hostname, 80)
      #Send request to web server
      puts "#{clientid} Sending request by:"
      puts "#{clientid}   #{http_method} #{path} HTTP/1.1"
      puts "#{clientid}   Host: #{hostname}"
      puts "#{clientid}   Connection: close"
      proxy_client.print("#{http_method} #{path} HTTP/1.1\r\nHost: #{hostname}\r\nConnection: close\r\n\r\n")
      
      #Get response from server and split it into header and body
      t_start = Time.now
        response = proxy_client.read
      t_stop = Time.now
      t_delta = ((t_stop - t_start)*1000).to_i
      puts "#{clientid} response from #{hostname} took ~#{t_delta} ms."
      header, body = response.split("\r\n\r\n", 2)
      
      #Check if body contains any bad words if the file is a text file
      if ( header.include?("Content-Type: text") )
        if ( has_bad_word(body) )
          #Redirect client to "net nanny"
          puts "#{clientid} #{url} Contents contains bad word."
          redirect_client(client, 
            "http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error2.html")
          response = nil
        end
      end
      puts "#{clientid} #{url} seems fine."
      puts "#{clientid} Sending to browser."
      client.print(response)
    end
    client.close
    puts "#{clientid} Connection to #{hostname} was closed."
  end
end
