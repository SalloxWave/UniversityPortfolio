#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

require './rdparse.rb'
require './Nibla_Code.rb'

class Nibla

  def initialize
    @nibla = Parser.new("Nibla") do
      token(/\s+/)               #Ignore whitespace
      token(/\d+/) {|x| x.to_i } #Integer
      token(/\#.*/)              #Comment
      token(/[a-zA-Z]+/) {|x| x.to_s} #Chars
      token(/"[\w\W]*"/) {|x| x.to_s} #Strings
      token(/\=\=/) {|x| x}      #match relation op
      token(/\!\=/) {|x| x} 
      token(/\>\=/) {|x| x}
      token(/\<\=/) {|x| x}
      token(/./) {|x| x }        #Let everything else be as it is
      
      start :program do
        match(:stmt_list) {|stmts| Final_eval.new(stmts)}
      end
      
      rule :stmt_list do
        match(:stmt_list, ',', :stmt) {|a, _, c| MultiStmt.new([a,c])}
        match(:stmt) {|a| a}
      end

      rule :stmt do
        match(:function_def)
        match(:function_call)
        match(:condition)
        match(:repetition)
        match(:break)
        match(:return)
        match(:assign)
        match(:output)
        match(:input)
        match(:expr)
      end

      rule :function_def do
        match('fun', :identifier, :stmt_list, 'endfun') {|_, b, c, _| SetVar.new(b, FunctionDef.new(c))}
        match('fun', :identifier, '(', :para_list, ')', :stmt_list, 'endfun') {|_, b, _, d, _, f, _| SetVar.new(b, FunctionDef.new(f, d))}
      end

      rule :para_list do
        match(String, ',', :para_list) {|a, _, c| [a, c].flatten}
        match(String) {|a| [a]}
      end

      rule :function_call do
        match('call', :identifier, '(', :call_list, ')') {|_, b, _, d, _| FunctionCall.new(b, d)}
        match('call', :identifier) {|_, b| FunctionCall.new(b)}
      end

      rule :call_list do
        match(:expr, ',', :call_list) {|a, _, c| [a, c].flatten}
        match(:expr) {|a| [a]}
      end

      rule :condition do
        match('if', '(', :comp_expr, ')', :stmt_list, :else_con) {|_, _, c , _, e, f| If.new(c, e, f)}
      end

      rule :else_con do
        match('elseif', '(', :comp_expr, ')', :stmt_list, :else_con) {|_, _, c, _, e, f| If.new(c, e ,f)}
        match('else', :stmt_list, 'endif') {|_, b, _| b}
        match('endif')
      end

      rule :repetition do
        match('loop', '(', :range_expr, ')', :stmt_list, 'endloop') {|_, _, c, _, e, _| Loop1.new(c[0], c[1], c[2], e)}
        match('loop', '(', :comp_expr, ')', :stmt_list, 'endloop') {|_, _, c, _, e, _| Loop2.new(c, e)}
        match('loop', '(', :identifier, 'in', :expr, ')', :stmt_list, 'endloop') {|_, _, c, _, e, _, g, _| Loop3.new(c, e, g)}
      end

      rule :range_expr do
        match(:assign, 'upto', :expr) {|a, b, c| [a, b, c]}
        match(:assign, 'downto', :expr) {|a, b, c| [a, b, c]}
      end

      rule :comp_expr do
        match(:expr, :real_oper, :expr) {|a, b, c| Relation.new(a, b, c)}
      end

      rule :break do
        match('break') {Break.new}
      end

      rule :return do
        match('return', :expr) {|_, b| Return.new(b)}
        match('return') {Return.new}
      end

      rule :assign do
        match(:identifier, '=', :expr) {|a, _, c| SetVar.new(a, c)}
        match(:identifier, 'add', :expr) {|a, _, c| AddList.new(a, c)}
      end

      rule :output do
        match('print', '(', :mult_output, ')') {|_, _, c, _| Output.new(c)}
        match('print', :expr) {|_, b| Output.new([b])}
      end

      rule :mult_output do
        match(:expr, ',', :mult_output) {|a, _, c| [a, c].flatten}
        match(:expr) {|a| [a]}
      end

      rule :input do
        match('input', :identifier) {|_, a| Input.new(a, gets)}
      end
      
      rule :expr do
        match(:expr, :add_oper, :term) {|a, b, c|  Expression.new(a, b, c)}
        match(:term)
      end
      
      rule :term do
        match(:term, :multi_oper, :modulu) {|a, b ,c| Expression.new(a, b, c)}
        match(:modulu)
      end

      rule :modulu do
        match(:modulu, :mod_oper, :factor) {|a, b, c| Expression.new(a, b, c)}
        match(:factor)
      end
      
      rule :factor do
        match(:digit)
        match(:string)
        match(:function_call)
        match(:container)
        match(:identifier)
        match('(', :expr, ')') {|_,a,_| a}
      end
      
      rule :add_oper do
        match('+')
        match('-')
      end
      
      rule :multi_oper do
        match('*')
        match('/')
      end

      rule :mod_oper do
        match('%')
      end
      
      rule :real_oper do
        match('==')
        match('!=')
        match('>')
        match('<')
        match('>=')
        match('<=')
      end

      rule :string do
        match(/"[\w\W]*"/) {|a| String.new(a)}
      end

      rule :container do
        match(:identifier, '[', :digit, ']') {|a, _, c, _| List.new(a, c)}
        match('[', :list, '[', :digit, ']') {|_, b, _, d, _| List.new(b, d)}
        match('[', :list) {|_, b| List.new(b)}
        match('[', ']') {List.new}
        
        match('{', '}') {|_, b| Dict.new({})}
        match(:identifier, '[', String, ']', '=', :expr) {|a, _, c, _, _, f| Dict.new(a, c, f)}
        match(:identifier, '[', String, ']') {|a, _, c, _| Dict.new(a, c)}
      end

      rule :list do
        match(:expr, ',',  :list) {|a, _, c| [a, c].flatten}
        match(:expr, ']') {|a, _| [a]}
      end
      
      rule :identifier do
        match(:letter)
        match(:letter, (:letter or :digit))
        match(:letter, :identifier)
      end
  
      rule :letter do
        match(/^[a-zA-Z_]+/) {|a| Variable.new(a)}
      end
      
      rule :digit do
        match(Integer) {|a| Digit.new(a)}
      end
    end
  end
  
  def eval_file(str)
    file = File.open(str.split[1])
    parse = true
    counter = 0
    for line in file
      if line != "\n"
        if parse and counter == 0
          str = line
        else
          str += line
        end
        
        if line =~ /^if|^loop|^fun/
          parse = false
          counter += 1
        end
        
        if line =~ /^endif|^endloop|^endfun/
          parse = true
          counter -= 1
        end
        
        if parse and counter == 0
          begin
            @nibla.parse(str).eval
          rescue Exception => error
            puts error.message
            break
          end
        end
      end
    end
  end

  def exit?(str)
    ["bye"].include?(str.chomp)
  end
  
  def start
    print "Nibla <> "
    str = gets
    @nibla.logger.level = Logger::WARN
    if exit?(str)
      puts "Bye Bye"
    elsif str =~ /^open\s+.+\.nibla/
      eval_file(str)
      start
    else
      begin
        @nibla.parse(str).eval
      rescue Exception => error
        puts error.message
      end
      start
    end
  end
end

Nibla.new.start
