#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
require './rdparse'
require './RaukLang'
require 'logger'



class RaukLang
  def initialize
    @RaukParser = Parser.new("RaukLang") do
      token(/\s+/) #Ignorera blanksteg
      token(/\t+/)  #Ignorerar tab
      token(/\d+/){|m| m} #Matchar integers
      token(/\/\/.*\$/) #Ignorerar kommentarer
      token(/[a-zA-Z_]+/) {|x| x.to_s} #Martchar character
      token(/"[^"]+"/) {|m| m} #Matchar strängar
      token(/\s+/) #ignore whitespace
      token(/\t+/)  #ignore tab
      token(/\d+/){|m| m} #integer
      token(/\$.*/) #comments
      token(/[a-zA-Z_]+/) {|x| x.to_s} #Chars
      token(/"[^"]+"/) {|m| m}#strings

      #operators
      token(/</){|m| m}
      token(/>/){|m| m}
      token(/&&/){|m| m}
      token(/\|\|/){|m| m}
      token(/\<\=/){|m| m}
      token(/\>\=/){|m| m}
      token(/\!\=/){|m| m}
      token(/\=\=/){|m| m}
      token(/./) {|m| m } #Allt annat

      start :begin do
        match(:stmt_list){|a| Final_stmt.new(a)}
      end

      rule :stmt_list do
        match(:stmt, :stmt_list) {|a, b| Multi_stmt.new(a,b) }
        match(:stmt)
      end

      rule :stmt do 
        match(:return_stmt)
        match(:for_stmt)
        match(:while_stmt)
        match(:assign)
        match(:print_stmt)
        match(:assign)
        match(:if_stmt)
        match(:for_stmt)
        match(:while_stmt)
        match(:return_stmt)
        match(:func_def)
        match(:func_call)
      end

      rule :while_stmt do
        match('while', '(', :bool_expr, ')', ':', :stmt_list, 'end_while', ';'){|_, _, a, _, _, b, _, _| While.new(a,b)}
      end

      rule :for_stmt do
        match('for', '(', :assign, :bool_expr, ';', :name, '=' , :add_expr, ')', ':', :stmt_list, 'end_for', ';'){|_, _, a, b, _, c, _, d, _, _, e, _, _ | For.new(a,b,c,d,e)}
      end

      rule :if_stmt do
        match('if', '(', :bool_expr, ')', ':', :stmt_list, :else_stmt) {|_, _, a, _, _, b, c| If.new(a,b,c)}
      end

      rule :else_stmt do
        match('else_if', '(', :bool_expr, ')', ':', :stmt_list, :else_stmt) {|_, _, a, _, _, b, c| If.new(a,b,c)}
        match('else', ':', :stmt_list, 'end_if', ';') {|_, _, a, _, _| Else.new(a)}
        match('end_if', ';')
      end

      rule :print_stmt do
        match('print', '(', :add_expr, ')', ';') {|_, _, a, _, _| Print.new(a)}
      end

      rule :return_stmt do
        match('return', '(', :add_expr, ')', ';') {|_, _, a, _, _| Return.new(a)}
      end

      rule :func_def do
        match('func', :name, '(', :call_list, ')', ':', :stmt_list, 'end_func', ';') {|_, a, _, b, _, _, c, _, _| FuncDef.new(a,b,c)}
      end

      rule :func_call do
        match(:name, '(', :call_list, ')', ';') {|a, _, b, _, _| FuncCall.new(a, b)}
        match(:name, '(', ')', ';')
      end
      
      rule :call_list do
        match(:call_list, ',', :add_expr) {|a, _, b| [a,b]}
        match(:add_expr)
      end

      rule :assign do 
        match(:name, '=', :bool, ';' ) {|a, _, b, _| Assign.new(a,b)}
        match(:name, '=', :func_call) {|a, _, b| Assign.new(a,b)}
        match(:name, '=', :add_expr, ';') {|a, _, b, _| Assign.new(a,b)}
      end

      rule :comp_op do
        match('<')
        match('>')
        match('<=')
        match('>=')
        match('!=')
        match('==')
      end

      rule :bool_expr do
        match(:bool_expr, :bool_or, :bool_expr){|a, b, c| BoolExpr.new(a, b, c)}
        match(:add_expr, :comp_op, :add_expr){|a, b, c | BoolExpr.new(a, b, c)}
        match(:bool)
      end

      rule :bool_or do
        match('||')
        match(:bool_and) 
      end
      
      rule :bool_and do 
        match('&&')
      end

      rule :bool do
        match(true) {|a| Atom.new(a.to_s)}
        match(false) {|a| Atom.new(a.to_s)}
      end
      
      rule :add_expr do
        match(:add_expr, :add_op, :mult_expr) {|a,b,c| AddExpr.new(a,b,c)}
        match(:mult_expr)
      end

      rule :add_op do
        match('+')
        match('-')
      end

      rule :mult_expr do
        match(:mult_expr, :mult_op, :factor) {|a,b,c| MultExpr.new(a,b,c)}
        match(:factor)
      end

      rule :mult_op do 
        match('*')
        match('/')
      end
      
      rule :factor do 
        match(:int)
        match(:string)
        match(:func_call)
        match(:float)
        match(:name)
      end

      rule :int do
        match(/[0-9]+/) {|a| Atom.new(a.to_i)}
      end

      rule :string do 
        match(/"([^"]+)"/) {|a| Atom.new(a.to_s)}
      end

      rule :float do
        match(/[0-9]+\.[0-9]+/){|a| Atom.new(a.to_f)}
      end

      rule :name do 
        match(/[A-Za-z_]+/) {|a| Name.new(a)}
      end

    end
  end

  def go
    print "[RaukEn] "
    str = gets
    @RaukParser.parse(str).eval
    go
  end

  def log(state = true)
    if state
      @RaukParser.logger.level = Logger::DEBUG
    else
      @RaukParser.logger.level = Logger::WARN
    end
  end

  def start(file)
    @RaukParser.parse(File.read(file)).eval
  end
end

if ARGV.length == 1
  file = ARGV[0]
  if File.exist?(file)
    rauk = RaukLang.new
    rauk.log(true)
    rauk.start(file)
  else
    puts "Can't open"
  end
else
  RaukLang.new.go
end
