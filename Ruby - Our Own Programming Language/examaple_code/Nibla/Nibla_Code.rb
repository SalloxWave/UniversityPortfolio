#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

class Scope
  def initialize
    @@scope_counter = 1
    @@scope = Hash.new
  end

  def Scope.setScope(scope)
    @@scope = scope
  end

  def Scope.getScope
    @@scope
  end

  def Scope.getCounter
    @@scope_counter
  end

  def Scope.newScope
    scope = Hash.new
    @@scope_counter += 1
    scope[@@scope_counter] = Scope.getScope
    return scope
  end
  
  def Scope.setOldScope(scope)
    Scope.setScope(scope[@@scope_counter])
    @@scope_counter -= 1
  end
end

Scope.new

class Final_eval
  def initialize(stmts)
    @stmts = stmts
  end
  
  def eval
    if @stmts.respond_to? "each"
      @stmts.each {|stmt| stmt.eval}
    else
      @stmts.eval
    end
  end
end

class MultiStmt
  def initialize(stmts)
    @stmts = stmts
  end

  def eval
    for i in @stmts
      value = i.eval
      if value == 'break'
        return 'break'
      elsif value.class == Array
        if value[0] == 'return'
          return value
        end
      end
    end
  end
end

class FunctionDef
  def initialize(stmt, para = [])
    @para = para
    @stmt = stmt
    @scope = Hash.new
  end

  def eval    
    value = @stmt.eval
    if value.class == Array
      if value[0] == 'return'
        return value
      end
    end
  end

  def getPara
    if @para == nil
      return []
    else
      return @para
    end
  end
end

class FunctionCall
  def initialize(name, para = [])
    @name = name
    @para = para
  end

  def eval
    @scope = Scope.newScope
    Scope.setScope(@scope)
    paras = @name.eval.getPara
    if paras.length == @para.length
      for i in 0..(@para.length - 1)
        if @para[i].eval.class == Fixnum
          SetVar.new(Variable.new(paras[i]), Digit.new(@para[i].eval)).eval
        else
          SetVar.new(Variable.new(paras[i]), String.new(@para[i].eval)).eval
        end
      end
      value = @name.eval.eval
      if value.class == Array
        if value[0] == 'return'
          Scope.setOldScope(@scope)
          return value[1]
        end
      end
    else
      raise "Expected #{paras.length} parameters but got #{@para.length}"
    end
    Scope.setOldScope(@scope)
  end
end

class If
  def initialize(rel_expr, stmt, else_stmt)
    @rel_expr = rel_expr
    @stmt = stmt
    @else_stmt = else_stmt
    @scope = Hash.new
  end

  def eval
    @scope = Scope.newScope
    Scope.setScope(@scope)
    if @rel_expr.eval
      value = @stmt.eval
      if value == 'break'
        Scope.setOldScope(@scope)
        return 'break'
      elsif value.class == Array
        if value[0] == 'return'
          Scope.setOldScope(@scope)
          return value
        end
      end
    else
      value = @else_stmt.eval
      if value == 'break'
        Scope.setOldScope(@scope)
        return 'break'
      elsif value.class == Array
        if value[0] == 'return'
          Scope.setOldScope(@scope)
          return value
        end
      end
    end
    Scope.setOldScope(@scope)
  end
end

class Loop1
  def initialize(var, direction, range, stmt)
    @var = var
    @direction = direction
    @range = range
    @stmt = stmt
    @scope = Hash.new
  end
 
  def eval
    @scope = Scope.newScope
    Scope.setScope(@scope)

    name = @var.name.var
    value = @var.eval
    
    if @direction == 'upto'
      value.upto(@range.eval) do
        stmt_value = @stmt.eval
        if stmt_value == 'break'
          break
        elsif stmt_value.class == Array
          if stmt_value[0] == 'return'
            Scope.setOldScope(@scope)
            return stmt_value
          end
        end
        value += 1
        SetVar.new(Variable.new(name), Digit.new(value)).eval
      end
    else
      value.downto(@range.eval) do
        stmt_value = @stmt.eval
        if stmt_value == 'break'
          break
        elsif stmt_value.class == Array
          if stmt_value[0] == 'return'
            Scope.setOldScope(@scope)
            return stmt_value
          end
        end
        value -= 1
        SetVar.new(Variable.new(name), Digit.new(value)).eval
      end    
    end

    Scope.setOldScope(@scope)
  end
end

class Loop2
  def initialize(comp_expr, stmt)
    @comp_expr = comp_expr
    @stmt = stmt
    @scope = Hash.new
  end

  def eval
    @scope = Scope.newScope
    Scope.setScope(@scope)

    while @comp_expr.eval
      value = @stmt.eval
      if value == 'break'
        break
      elsif value.class == Array
        if value[0] == 'return'
          Scope.setOldScope(@scope)
          return value
        end
      end
    end

    Scope.setOldScope(@scope)
  end
end

class Loop3
  def initialize(iter_var, iter_stmt, stmt)
    @iter_var = iter_var
    @iter_stmt = iter_stmt
    @stmt = stmt
    @scope = Hash.new
  end

  def eval
    @scope = Scope.newScope
    Scope.setScope(@scope)
    
    if @iter_stmt.eval.class == Fixnum
      raise "Error: Can not iterate through numbers"
    else
      name = @iter_var.var

      for i in 0..(@iter_stmt.eval.length - 1)
        SetVar.new(Variable.new(name), String.new(@iter_stmt.eval[i])).eval
        value = @stmt.eval
        if value == 'break'
          break
        elsif value.class == Array
          if value[0] == 'return'
            Scope.setOldScope(@scope)
            return value
          end
        end
      end
    end
    
    Scope.setOldScope(@scope)
  end
end

class Break
  def eval
    'break'
  end
end

class Return
  def initialize(expr = nil)
    @expr = expr
  end
  
  def eval
    ['return', @expr.eval]
  end
end

class SetVar
  attr_reader :name
  def initialize(name, expr)
    @name = name
    @expr = expr
    @scope = Hash.new
  end
  
  def eval
    @scope = Scope.getScope
    @namedVar = @name.var
    value = @expr
    if @expr.class != FunctionDef
      value = @expr.eval
    end

    counter = Scope.getCounter
    while counter > 1
      if @scope[counter].has_key? @namedVar
        @scope[counter][@namedVar] = value
        return
      else
        @scope = @scope.values[0]
        @scope = Hash[*@scope.collect {|x| [x]}.flatten]
        counter -= 1
      end
    end
    
    @scope = Scope.getScope
    @scope[@namedVar] = value
  end
end

class AddList
  def initialize(name, expr)
    @name = name
    @expr = expr
  end

  def eval
    @scope = Scope.getScope
    @namedVar = @name.var

    if @name.eval.class != Array
      raise "Can only add values in lists"
    end

    value = @expr
    if @expr.class != FunctionDef
      value = @expr.eval
    end

    counter = Scope.getCounter
    
    if counter == 1
      if @scope.has_key? @namedVar
        @scope[@namedVar] = @name.get_value << value
        return
      end
    else
      while counter > 1
        if @scope[counter].has_key? @namedVar
          @scope[counter][@namedVar] = @name.get_value << value
          return
        else
          @scope = @scope.values[0]
          @scope = Hash[*@scope.collect {|x| [x]}.flatten]
          counter -= 1
        end 
      end
    end

    raise "#{@namedVar} can not add #{value}"
  end
end

class Output
  def initialize(value)
    @value = value
  end
  
  def eval
    if @value.class == Array
      for i in @value
        puts check_var(i)
      end
    else
      puts check_var(@value)
    end
  end
end

class Input
  def initialize(name, value)
    @name = name
    @value = value
  end

  def eval
    if @value =~ /[0-9]+/
      SetVar.new(@name, Digit.new(instance_eval(@value))).eval
    else
      SetVar.new(@name, String.new(instance_eval(@value))).eval
    end
  end
end

class Relation
  def initialize(left_expr, op, right_expr)
    @left_expr = left_expr
    @right_expr = right_expr
    @op = op
  end

  def eval 
    case @op
    when '==' then return (@left_expr.eval == @right_expr.eval)
    when '!=' then return (@left_expr.eval != @right_expr.eval)
    when '>' then return (@left_expr.eval > @right_expr.eval)
    when '<' then return (@left_expr.eval < @right_expr.eval)
    when '>=' then return (@left_expr.eval >= @right_expr.eval)
    when '<=' then return (@left_expr.eval <= @right_expr.eval)
    end
  end
end

class Expression
  def initialize(var1, op, var2)
    @var1 = var1
    @var2 = var2
    @op = op  
  end

  def eval
    case @op
    when '*' then return (@var1.eval * @var2.eval)
    when '/' then return (@var1.eval / @var2.eval)
    when '+' then return (@var1.eval + @var2.eval)
    when '-' then return (@var1.eval - @var2.eval)
    when '%' then return (@var1.eval % @var2.eval) 
    end
  end
end

class Digit
  def initialize(number)
    @value = number
  end
  
  def eval
    return @value
  end
end

class String
  def initialize(str)
    @value = str
  end

  def eval
    return @value
  end
end

class List
  def initialize(list = nil, pos = nil)
    @list = list
    @pos = pos
  end

  def eval
    if @list == nil
      return []
    end

    return_list = []
    if @pos == nil
      for i in @list
        return_list << i.eval
      end
      return return_list
    else
      if @list.class == Array
        return @list[@pos.eval].eval
      else
        return @list.eval[@pos.eval]
      end
    end
  end
end

class Dict
  def initialize(dict, key = nil, value = nil)
    @dict = dict
    @key = key
    @value = value
  end

  def eval
    if @key != nil and @value != nil
      @dict.eval[@key] = @value.eval
    elsif @key != nil
      return @dict.eval[@key]
    else
      return @dict
    end
  end
end

class Variable
  attr_reader :var
  def initialize(var)
    @var = var
  end

  def eval
    check_var(self)
  end

  def get_value
    return check_var(self)
  end
end

def check_var(var)
  scope = Scope.getScope
  if notvar(var)
    return var.eval
  end  

  if var.class == Variable
    varName = var.var
  else
    varName = var
  end

  if scope.has_key? varName
    return scope[varName]
  elsif scope[Scope.getCounter] != nil
    counter = Scope.getCounter
    while counter != 1
      if scope[counter].has_key? varName
        return scope[counter][varName]
      else
        scope = scope.values[0]
        scope = Hash[*scope.collect {|x| [x]}.flatten]
        counter -= 1
      end
    end    
  end
  
  raise "Error: The variable #{varName} has no value"
end

def notvar(var)
  classes = [Digit, String, Expression, List, Dict, FunctionCall]
  if classes.include?(var.class)
    return true
  end
  return false
end
