#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

$scope = [{}] #Här är en lista som lagrar variabler med vilket scope dem ligger i som nyckel
$functions = {} #Här lagras alla funktioner som deklareras
$func_stmt = {} #Här lagras funktionens stmt lista
$scope_count = 0 #För att hålla koll på vilket scope vi befinner oss i

#Funktion för att öppna ett scope
def openscope 
  $scope << {}
  $scope_count += 1
end

#Funktion för att stänga ett scope
def closescope
  $scope.pop
  $scope_count -= 1
end

#Funktion för att leta upp en Variabel
def look_up(variable)
  i = $scope_count
  while i >= 0
    if $scope[i][variable] != nil
      return $scope[i][variable]
    end
    i -= 1
  end
<<<<<<< HEAD
  if $scope[0][variable] == nil
    abort("Variabeln '#{variable}' finns inte!")
=======
  if $scope[0][var] == nil
    abort("Variabeln '#{var}' finns inte!")
>>>>>>> c4a44890738d5014633fbb1a21132da0f2ee8775
  end
end

#Funktion för att få ut vilket scope en variable finns
def get_var_scope(variable)
  i = $scope_count
  while i >= 0
    if $scope[i][variable] != nil
      return i
    end
    i -= 1
  end
  if $scope[0][variable] == nil
    return $scope_count
  end
end

class Final_stmt
  def initialize(stmts)
    @stmts = stmts
  end

  def eval
    if @stmts != nil
      if @stmts.is_a?(Array)
        for stmt in @stmts
          stmt.eval
        end
      else
        @stmts.eval
      end
    end
  end
end

class Multi_stmt
  def initialize (stmt, stmt_list)
    @stmt = stmt
    @stmt_lst = stmt_list
  end
<<<<<<< HEAD

  def eval()
    @stmt.eval
    @stmt_lst.eval
=======
  def eval
    puts "hejeh"
    bool = [false, nil]
    for i in @stmt_lst
      if i.is_a?(Return)
        bool = [true, i.eval]
      end
      break if bool[0] == true
      i.eval if bool[0] == false
    end
    return bool
>>>>>>> c4a44890738d5014633fbb1a21132da0f2ee8775
  end
end

class Assign
  def initialize(name, add_expr)
    @name = name
    @add_expr = add_expr
  end

  def eval
    @scope = get_var_scope(@name.name)
    if @scope == $scope_count
      $scope[$scope_count][@name.name] = @add_expr.eval
    else
      $scope[@scope][@name.name] = @add_expr.eval
    end
  end
end

class Atom 
  def initialize(value)
    @atom = value
  end

  def eval
    return @atom
  end
end


class Name
  attr_reader :name
  def initialize(name)
    @name = name
  end   

  def eval
    if @name.is_a?(FuncDef)
      @name.eval
    else
      return look_up(@name)
    end
  end
end


class While
  def initialize(bool_expr, stmt_lst)
    @bool_expr = bool_expr
    @stmt_lst = stmt_lst
  end
  def eval()
    openscope()
    while(@bool_expr.eval)
      value = @stmt_lst.eval
      if value.is_a?(Return)
        closescope()
        return value
        break
      end
    end
    closescope()
  end
end


class For
  def initialize(assign, bool_expr, name, add_expr, stmt_list)
    @assign = assign
    @bool_expr = bool_expr
    @name = name
    @add_expr = add_expr
    @stmt_lst = stmt_list
  end
  def eval
    @assign.eval
    while @bool_expr.eval
      a = Assign.new(@name, @add_expr)
      a.eval
      @stmt_lst.eval
    end
  end
end


class Print
  def initialize(print_value)
    @print = print_value
  end
  def eval
    puts @print.eval
  end
end


class Return
  attr_accessor :return
  def initialize(return_value)
    @return = return_value
  end
  def eval

    return @return.eval
  end
end


class FuncDef
  attr_reader :call_lst, :stmt_lst
  def initialize(name, call_list, stmt_list)
    @name = name
    @call_lst = call_list
    @stmt_lst = stmt_list
  end

  def eval
    if $functions[@name.name] == nil
      $functions[@name.name] = @call_lst
      $func_stmt[@name.name] = @stmt_lst
    else
<<<<<<< HEAD
      abort("Functionen '#{name.name}' finns redan!")
=======
      abort("Functionen #{name.name} finns inte!")
>>>>>>> c4a44890738d5014633fbb1a21132da0f2ee8775
    end
  end
end


class FuncCall
  def initialize(name, call_list)
    @name = name
    @call_list = call_list
  end

  def eval
    if $functions[@name.name] != nil
      openscope()
      for i in(0..@call_list.length - 1)
<<<<<<< HEAD
        a = Assign.new($functions[@name.name][i], @call_list[i])
        a.eval
      end
      return $func_stmt[@name.name].eval
    else
      abort("Functionen '#{name.name}' finns inte!")
=======
        Assign.new($functions[@name.name][i], @call_list[i]).eval
      end
      return_bool = $func_stmt[@name.name].eval
      puts return_bool
      if return_bool[0]
        closescope()
        return return_bool[1]
    end
    else
      abort("Functionen '#{name}' finns inte!")
>>>>>>> c4a44890738d5014633fbb1a21132da0f2ee8775
    end
    closescope()
  end
end


class AddExpr
  def initialize(value1, op, value2)
    @value1 = value1
    @operator = op
    @value2 = value2
  end

  def eval
    if @value1.eval.is_a?(Fixnum) and @value2.eval.is_a?(Fixnum)
      if @operator.eql? "+"
        return(@value1.eval + @value2.eval)
      elsif @operator.eql? "-"
        return(@value1.eval - @value2.eval)
      end
    elsif @value1.eval.is_a?(String) or @value2.eval.is_a(String)
      if @operator.eql? "+"
        return(@value1.eval.to_s + @value2.eval.to_s)
      elsif @operator.eql? "-"
        return(@value1.eval.to_s - @value2.eval.to_s)
      end
    end
  end
end


class MultExpr
  def initialize(value1, op, value2)
    @value1 = value1
    @operator = op
    @value2 = value2
  end

  def eval
    if @operator.eql? "*" 
      return (@value1.eval * @value2.eval) 
    elsif @operator.eql? "/" 
      return (@value1.eval / @value2.eval) 
    end
  end
end


class BoolExpr
  def initialize(bool_expr, comp_op , bool_expr1 )
    @bool_expr = bool_expr
    @bool_expr1 = bool_expr1
    @op = comp_op
  end
  
  def eval
    if @op.eql? "||"
      return ((@bool_expr.eval == true) or (@bool_expr1.eval == true))
    elsif @op.eql? "&&" 
      return ((@bool_expr.eval == true) and (@bool_expr1.eval == true))
    elsif @op.eql? "<"
      return (@bool_expr.eval < @bool_expr1.eval) 
    elsif @op.eql? ">"
      return (@bool_expr.eval > @bool_expr1.eval)
    elsif @op.eql? "<="
      return (@bool_expr.eval <= @bool_expr1.eval) 
    elsif @op.eql? ">="
      return (@bool_expr.eval >= @bool_expr1.eval)
    elsif @op.eql? "!="
      return (@bool_expr.eval != @bool_expr1.eval)
    elsif @op.eql? "=="
      return (@bool_expr.eval == @bool_expr1.eval)
    end
  end
end


class If
  def initialize(bool, stmt_lst, else_stmt)
    @bool_expr = bool
    @stmt_list = stmt_lst
    @else_stmt = else_stmt
  end

  def eval
    openscope()
    if @bool_expr.eval == true
      
      return @stmt_list.eval
    elsif @else_stmt.is_a?(If) 
      return @else_stmt.eval
    elsif @else_stmt.is_a?(Else)
      return @else_stmt.eval
    end
    closescope()
  end
end

class Else
  def initialize(stmt_lst)
    @stmt_list = stmt_lst
  end

  def eval
    @stmt_list.eval
  end
end
