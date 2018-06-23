"""
Stores all variables, where the index of the array represents the
scope where the variable is declared.
The dictionary maps the variable name to its value
"""
$variables = [{}]

"""
Stores all functions, where the index of the array represents the
scope where the function is declared.
The dictionary maps the function name to its function object
"""
$functions = [{}]

class Scope
  @@level = 0
  def Scope.current_level
    @@level
  end

  def Scope.current_level=(rhs)
    @@level = rhs
  end

  def Scope.get_variables
      """ 
      Get all variables from current scope up to global scope
      If duplicate is found, use the variable at highest level
      """

      result = Hash.new
      $variables.reverse_each do |vars|
          result.merge!(vars)
      end
      return result
  end

  def Scope.get_level_by_name(varname)
    """
    Goes through scope from most highest to lowest and returns 
    the first scope level where the specified variable name was found.
    If no variable is found, return current scope
    """

    level = 0
    $variables.each do |scope|
      if scope.has_key? varname
        return level 
      end
      level+=1
    end
    return current_level
  end

  def Scope.get_level_by_fun_name(funname)
    """
    Goes through function scope from global to current scope and returns
    the first scope level where the function is found. Raise error if no functions
    was found.
    """
    
    level = 0
    $functions[0..@@level].each do |scope|
      if scope.has_key? funname
        return level
      end
      level+=1
    end
    #The specified function name was not found
    raise NameError.new("Function \"#{funname}\" was not found in the current scope")
  end

  def Scope.increase
    """ 
    Increase scope level by 1 and prepare dictionaries for
    functions and variables in that scope
    """

    @@level+=1
    $variables[@@level] = Hash.new
    
    #To not override already defined functions in that scope
    if $functions[@@level] == nil
      $functions[@@level] = Hash.new
    end
  end

  def Scope.decrease
    """
    Decrease scope level by 1 and the variables on that scope.
    """

    #Delete all variables in highest scope level before decreasing level
    $variables.pop
    @@level-=1
  end

  def Scope.set(new_level)
    """
    Set scope at a specific level and delete or create variables
    depending on scope level.
    """

    #Decrease scope
    if new_level < @@level
      count = @@level - new_level - 1
      count.times do 
        Scope.decrease
      end    
    #Increase scope
    else
      count = new_level - @@level
      count.times do
        Scope.increase
      end
    end      
  end
end

class Program
  def initialize(statements)
    @statements = statements
  end
  def run
    @statements.each do |statement|
      old_level = Scope.current_level
      statement.evaluate
      #Make sure current level is back where it started after each statement
      if (Scope.current_level != old_level)
        Scope.current_level = old_level
      end
    end
  end
end

class Variable
  attr_reader :name
  def initialize(name)
    @name = name
  end

  def evaluate
    #Try to find name of variable
    if Scope.get_variables.has_key?(@name)
      Scope.get_variables[@name]
    else
      raise NameError.new("The variable \"#{@name}\" does not exists in the current scope")
    end
  end
end

class Expression
  attr_reader :left_expr
  def initialize(left_expr, aritm_op='', right_expr=nil)
    @left_expr = left_expr
    @aritm_op = aritm_op
    @right_expr = right_expr
  end

  def evaluate
    #Expression only has data
    if @aritm_op.eql?('') || @right_expr.eql?(nil)
      #Expression is alone and needs to be evaluated
      if @left_expr.is_a?(Variable) || 
        @left_expr.is_a?(FunctionCall) || 
        @left_expr.is_a?(StringConcatenation) ||
        @left_expr.is_a?(Null) ||
        @left_expr.is_a?(ArrayCall)
        @left_expr.evaluate
      else
        #Return data
        @left_expr
      end
    else
      #Evaluate left and right side recursively
      eval("@left_expr.evaluate#{@aritm_op}@right_expr.evaluate")
    end
  end
end

class Condition
  def initialize(left_expr, operator='', right_expr=nil)
    @left_expr = left_expr
    @operator = operator
    @right_expr = right_expr
  end

  def evaluate
    #Condition only has single expression
    if @operator.eql?('') || @right_expr.eql?(nil)
      #Return data of variable, or false if the data is either 0 or null
      if @left_expr.evaluate.eql?(0) || @left_expr.evaluate.eql?(nil)
        false
      else
        @left_expr.evaluate
      end
    else
      #Evaluate left and right side recursively
      eval("@left_expr.evaluate#{@operator}@right_expr.evaluate")
    end
  end
end

class Assignment
  def initialize(var, assign_op, expression)
    @var = var
    @assign_op = assign_op
    @expression = expression
  end

  def get_var_name
    @var.name
  end
  
  def evaluate
    """
    Set variable to expression's value, or change value of variable 
    if the variable already exists
    """    

    #Targeted global variables could change when for example
    #evaluating a function call since the function call
    #is reassigning the global variables.
    old_var = $variables.map(&:clone)
    if @var.is_a?(ArrayCall)
      eval("old_var[Scope.current_level][@var.name][@var.index]#{@assign_op}@expression.evaluate")
    else
      eval("old_var[Scope.get_level_by_name(@var.name)][@var.name]#{@assign_op}@expression.evaluate")
    end
    #Set back variables after assigning
    $variables = old_var.map(&:clone)
  end
end

class AssignmentIncrement
  def initialize(var, increment_op)
    @var = var
    @increment_op = increment_op
  end

  def evaluate
    #Either increment plus or minus
    if @increment_op.eql?('++')
      Assignment.new(@var, '+=', Expression.new(1)).evaluate
    else
      Assignment.new(@var, '-=', Expression.new(1)).evaluate
    end
  end
end

class StringConcatenation
  def initialize(expr1, expr2)
    @expr1 = expr1
    @expr2 = expr2
  end

  def evaluate
    "#{@expr1.evaluate}#{@expr2.evaluate}"
  end
end

class Output
  def initialize(expr)
    @expr = expr
  end
  def evaluate
    #Arrays should be printed as a whole
    if not @expr.evaluate.is_a?(Array)
      puts @expr.evaluate
    else
      print @expr.evaluate
      puts
    end
  end
end

class Input
  def initialize(var, assign_op, expr='')
    @var = var
    @assign_op = assign_op
    @expr = expr
  end

  def evaluate
    print @expr.evaluate if @expr != ''
    input = $stdin.gets.chomp
    #Assign variable to value of input
    Assignment.new(@var, @assign_op, Expression.new(input)).evaluate
  end
end

class Block
  def initialize(statements)
    @statements = statements
  end

  def evaluate
    @statements.each do |statement|
      keyword = statement.evaluate
      if keyword.eql?("restart") || 
        keyword.is_a?(Array) && keyword[0].eql?("return")
        return keyword
      end
    end
  end
end

class If
  attr_reader :condition
  def initialize(condition, block)
    @condition = condition
    @block = block
  end

  def evaluate
    #Open new scope, evaluate block and close scope
    Scope.increase
    keyword = @block.evaluate
    Scope.decrease
    if keyword != nil
      if keyword.eql?("restart") || keyword[0].eql?("return")
        return keyword
      end
    end
  end
end

class ConditionStatement
  def initialize(if_stmt, elseifs = [], else_block = nil)
    @if_stmt = if_stmt
    @elseifs = elseifs
    @else_block = else_block
  end

  def evaluate
    do_else = true
    #Go into if-block
    if @if_stmt.condition.evaluate
      do_else = false
      keyword = @if_stmt.evaluate
      if keyword != nil
        if keyword.eql?("restart") || keyword[0].eql?("return")
          return keyword
        end
      end
    else
      @elseifs.each do |elseif|
        #Evaluate first elseif-block whoose condition evaluates to true
        if elseif.condition.evaluate
          keyword = elseif.evaluate
          if keyword != nil
            if keyword.eql?("restart") || keyword[0].eql?("return")
              return keyword
            end
          end
          do_else = false
          break
        end
      end
    end
    
    if @else_block != nil && do_else
      Scope.increase
      keyword = @else_block.evaluate
      if keyword != nil
        if keyword.eql?("restart") || keyword[0].eql?("return")
          return keyword
        end
      end
      Scope.decrease
    end
  end
end

class Restart
  def evaluate
    return "restart"
  end
end

class Start
  def initialize(statements)
    @statements = statements
  end

  def evaluate
    Scope.increase
    while true do
      restart = false
      @statements.each do |statement|
        #Evaluate statementand look for keyword
        keyword = statement.evaluate
        if keyword != nil
          if keyword.eql?("restart")
            #Stop current block
            restart = true
            break
          elsif keyword[0].eql?("return")
            return keyword
          end
        end
      end
      #Continue next iteration of while loop, or break out
      if(restart)        
        next
      end
      break
    end
    #Decrease scope level when all statements in start-block has been evaluated
    Scope.decrease
  end
end

class Return
  def initialize(expression)
    @expression = expression
  end
  def evaluate
    ["return", @expression.evaluate]
  end
end

class Function
  attr_reader :block

  def initialize(name, block, parameters=[])
    @name = name
    @block = block
    @parameters = parameters
  end

  def set_parameter_data(para_data)
    #Get all default assignments
    def_parameters = @parameters.select {|para| para.is_a?(Assignment)}

    #Assign all default parameter by evaluating the assignment objects
    def_parameters.each do |para|
      para.evaluate
    end

    i = 0
    #Make sure there is no arrays in arrays
    para_data = para_data.flatten
    para_data.each do |data|
      #Assign parameter name to specified parameter data (expression)
      if not @parameters[i].is_a?(Assignment)
        Assignment.new(Variable.new(@parameters[i]),"=", data).evaluate            
      else
        Assignment.new(Variable.new(@parameters[i].get_var_name),"=",data).evaluate
      end
      i+=1
    end

    #Remove all variables that doesn't belong
    $variables[Scope.current_level].each do |key, value|
      @parameters.each do |para|
        if not para.is_a?(Assignment)
          #This must be one of the worst ugly fixes we've seen
          if not @parameters.include?(key) and 
            para.is_a?(Assignment) and
            @parameters.include?(Assignment.new(Variable.new(para.get_var_name),"=",data))
            $variables[Scope.current_level].delete(key)
          end
        end
      end
    end
  end

  def evaluate
    #Connect name connected to yourself (function) at current scope
    $functions[Scope.current_level][@name] = self
  end
end

class FunctionCall
  def initialize(name, parameter_data=[])
    @name = name
    @parameter_data = parameter_data
  end
  def evaluate
    old_level = Scope.current_level

    #Get level where the function called is
    level = Scope.get_level_by_fun_name(@name)

    #Save old variables (variables before calling function)
    old_variables = $variables.map(&:clone)

    #Increase scope to 1 if the call was on a global level
    Scope.increase if Scope.current_level < 1

    #Set parameters to called function (scope is where the function was called)
    $functions[level][@name].set_parameter_data(@parameter_data)

    #After assigning variables you can go inside the function in scope 1
    Scope.set(1)

    #Call function block
    keyword = $functions[level][@name].block.evaluate
    
    #Check if there were any old variables
    if old_variables.any? {|scope| not scope.empty?}
      #Set back variables
      $variables = old_variables.map(&:clone)      
    end

    #Set back old level if it has changed
    if Scope.current_level != old_level
      Scope.set(old_level)
    end

    if keyword[0].eql?("return")
      #Return the expression's value you returned
      return keyword[1]
    end
  end
end

class Loop
  """ Regular loop (at least in our definition):
  loop ( <block> )
  loop ( <expression> )
  loop ( <expression> , <var> )
  loop ( <expression> , <var> , <assignment> )
  """

  def initialize(block, count=nil, varname=nil, assignment=nil)
    @block = block
    @count = count
    @varname = varname
    @assignment = assignment
  end

  def evaluate
    Scope.increase

    #Infinite loop with block
    #loop ( <block> )
    if @count == nil
      loop do 
        keyword = @block.evaluate 
        if keyword != nil
          if keyword.eql?("restart") || keyword[0].eql?("return")
            return keyword
          end
        end
      end

    #Evaluate block count times
    #loop ( <expression> )
    elsif @varname == nil
      @count.evaluate.times do 
        keyword = @block.evaluate
        if keyword != nil
          if keyword.eql?("restart") || keyword[0].eql?("return")
            return keyword
          end
        end
      end

    #loop ( <expression> , <var> ) (Array loop)
    elsif @count.evaluate.is_a?(Array)
      #Create an Array loop instead with same block and count as array
      keyword = LoopArray.new(@block, @count, @varname).evaluate
      if keyword != nil
        if keyword.eql?("restart") || keyword[0].eql?("return")
          return keyword
        end
      end

    else
      #Create variable with varname and set to 0
      Assignment.new(Variable.new(@varname), "=", Expression.new(0)).evaluate

      #Evaluate block count times
      @count.evaluate.times do
        keyword = @block.evaluate
        if keyword != nil
          if keyword.eql?("restart") || keyword[0].eql?("return")
            return keyword
          end
        end
        #loop ( <expression> , <var> )
        if @assignment == nil
          #Increase value of variable by 1 for each iteration
          AssignmentIncrement.new(Variable.new(@varname), "++").evaluate

        #loop ( <expression> , <var> , <assignment> )
        else
          #Increase value based on specific assignment
          @assignment.evaluate
        end        
      end
    end

    Scope.decrease
  end
end

class LoopWhile
  """
  loop ( <condition> )
  """
  
  def initialize(block, condition)
    @block = block
    @condition = condition
  end

  def evaluate
    Scope.increase

    while @condition.evaluate do
      keyword = @block.evaluate
      if keyword != nil
        if keyword.eql?("restart") || keyword[0].eql?("return")
          return keyword
        end
      end
    end

    Scope.decrease
  end
end

class LoopFor
  """
  loop ( <assignment>, <condition>, <assignment> )
  """

  def initialize(block, initial_assignment, condition, assignment)
    @block = block
    @initial_assignment = initial_assignment
    @condition = condition
    @assignment = assignment
  end

  def evaluate
    Scope.increase

    #Create initial variable
    @initial_assignment.evaluate

    continue = true
    while continue do
      #Evaluate for-block
      keyword = @block.evaluate
      if keyword != nil
        if keyword.eql?("restart") || keyword[0].eql?("return")
          return keyword
        end
      end

      #Evaluate assignment
      @assignment.evaluate

      #Evaluate loop condition
      continue = @condition.evaluate
    end

    Scope.decrease
  end
end

class LoopArray
  def initialize(block, array, varname)
    @block = block
    @array = array
    @varname = varname
  end

  def evaluate
    Scope.increase

    @array.evaluate.each do |item|
      #Create variable with specified name and value from array
      Assignment.new(Variable.new(@varname), '=', Expression.new(item)).evaluate

      #Evaluate block
      keyword = @block.evaluate       
      if keyword != nil
        if keyword.eql?("restart") || keyword[0].eql?("return")
          return keyword
        end
      end
    end
  end
end

class ArrayCall
  def initialize(var, index_expr)
    @var = var
    @index_expr = index_expr
  end

  def index
    @index_expr.evaluate
  end

  def name
    @var.name
  end

  def evaluate
    begin
      #Get array's value from global scope
      if $variables[Scope.get_level_by_name(@var.name)][@var.name][@index_expr.evaluate] != nil
        $variables[Scope.get_level_by_name(@var.name)][@var.name][@index_expr.evaluate]
      else
        raise IndexError.new
      end
    rescue => e
      if e.is_a?(IndexError)
        raise IndexError.new("\"#{@var.name}\": Index out of range (Index: #{index})")
      end
      raise NameError.new("Array \"#{@var.name}\" can't be found in the current scope")
    end
  end
end

class Null
  def evaluate
    nil
  end
end