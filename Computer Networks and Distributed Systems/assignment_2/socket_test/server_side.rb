#!/usr/bin/env ruby
# encoding: utf-8

require 'socket'
require 'net/http'

#Open server
server = TCPServer.open(2000)

def has_bad_word(str)
  puts "Welcome to bad word checker!!!"
  new_str = str.downcase
  #Create array of bad words
  bad_words = ["spongebob", "britney spears", "paris hilton", "norrköping"]
  #Check if string has any of the bad words
  return bad_words.any? { |word| new_str.include?(word) }
end

def get_host_and_path(request)
  md = request.scan(/\/\/([^\/]*)(.*)/)
  return md[0][0], md[0][1]
end

def redirect_bad_website(server, client)
  puts "Requesting bad word website..."
  proxy_server = TCPSocket.open("www.ida.liu.se", 80)
	proxy_server.print("HTTP/1.1 301 Moved Permanently\nLocation: http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error1.html\r\n\r\n")
  puts "Request accepted by server"
  puts "Redirecting client to bad website"
  response = proxy_server.read
  header, body = response.split("\r\n\r\n", 2)
  puts "Header_______::::::\n"
  puts body
  client.puts(response)
  puts "Bad-word-website rendered properly"
end


#Infinite loop
loop do
  #Create a new thread for each connected clients to allow multiple clients
  Thread.start(server.accept) do |client|
    puts "A new client has connected to the server!"

    #Get whole HTTP request from client
    request = client.gets
    #Fetch the paths from the request (METHOD HOSTNAME HTTPVERSION)
    http_method, url, http_version = request.split
    #Fetch the hostname and path from the request
    hostname, path = get_host_and_path(url)
=begin
    #DEBUG INFORMATION
    puts "Client #{client.addr} connected the proxy server #{server.addr}"
    puts
    puts "Client information: "
    puts "Request: #{request}"
    puts "HTTP METHOD: #{http_method}"
    puts "URL: #{url}"
    puts "HTTP version: #{http_version}"
    puts "Hostname: #{hostname}"
    puts "Path: #{path}"
    puts 
=end
    
    puts "Connecting to requested server '#{hostname}'"
    puts "Using request:  \n#{http_method} #{path} HTTP/1.1\r\nHost: #{hostname}\r\n\r\n"
    
    proxy_client = TCPSocket.open(hostname, 80)
    proxy_client.print("#{http_method} #{path} HTTP/1.1\r\nHost: #{hostname}\r\n\r\n")
    response = proxy_client.read
    
    header, body = response.split("\r\n\r\n", 2)
    
    puts "______Header______: #{header}"
    puts "______Body________: #{body}"
    
    puts "Checking if requested file is a text-file..."
    if ( header.include?("text/html") )
      puts "File was a text-file"
            
      puts "Looking for bad word in URL..."
      if ( has_bad_word(url) )
        puts "URL has bad word"
        #Send client to http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error1.html
        redirect_bad_website(server)  
      elsif ( has_bad_word(body) )
        puts "Bad word was found in html-content"        
        redirect_bad_website(server)
      else
        puts "The website was OKAY"
        puts "Printing content to client"
        client.puts(response)
        puts "The file was rendered to client properly"
      end
    else
      puts "File was NONE-text file"
      puts "Printing content to client"
      client.puts(response)
      puts "The file was rendered to client properly"
    end
    client.close
    puts "Client was closed\n\n\n\n\n\n\n\n\n"
  end
end







