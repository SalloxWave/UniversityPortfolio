""" old function define evaluation code part
    #Check if there's a function to define inside the function
    functions = @block.statements.find_all {|stmt| stmt.is_a? Function}
    functions.each do |func|
      #Function in functions means new scope
      Scope.increase
      func.evaluate
      Scope.decrease
    end
"""
""" Convert to array
#token(/\[.*\]/) {|m| m.tr('[]', '').split(',').map(&:to_i)}
"""

""" This causes some problems because how the grammar is built-up
match(:array_call, :assignment_operator, :condition) {|arr_call, assign_op, expr|
  Assignment.new(arr_call, assign_op, expr)
}
match(:var, :assignment_operator, :condition) {|var, assign_op, cond|
  Assignment.new(var, assign_op, cond)
}
"""

""" Old Loop condition, not used and may never be
class LoopCondition
  
  match(Integer, ',', :condition, ',', :assignment)
  match(Integer, ',', :condition, ',')
  
  def initialize(block, count, condition, assignment=nil)
    @block = block
    @count = count
    @condition = condition
    @assignment = assignment
  end

  def evaluate
    Scope.increase
    #Iterate count times
    @count.evaluate.times do
      #Evaluate block
      keyword = @block.evaluate
        if keyword.eql?('restart') || keyword[0].eql?('return'')
          return keyword
        end
      #Break loop if condition is not true
      break if not @condition.evaluate
      #Evaluate assignment if there is one
      @assignment.evaluate if assignment != nil
    end
    Scope.decrease
  end
end
"""

""" Old If class
class If
  def initialize(condition, if_block, elseifs=[], else_block=nil)
    @condition = condition
    @if_block = block
    @elseifs = elseifs
    @else_block = else_block
  end

  def evaluate
    do_else = true
    #Go into if-block
    if condition.evaluate
      @if_block.evaluate
      do_else = false
    else      
      elseifs.each do |elseif|
        #Evaluate first elseif statement evaluating to true
        #and ignore the rest
        if elseif.condition.evaluate
          elseif.evaluate
          do_else = false
          break
        end
      end
    end
    else_block.evaluate if do_else
  end

  def add(condition_statement)
    if condition_statement.is_a?(Array)
      condition_statement.each do |stmt|
        if stmt.is_a?()
      end
    elsif condition_statement.is_a?(Array)
      @elseifs = condition_statement
    else
      @else_block = condition_statement
    end
    return self
  end
end
"""