require './rdparse.rb'
require './classes.rb'

class Termer  
  def initialize
    @termerParser = Parser.new("Termer") do
      #Match string concatenation
      token(/(\s\.\s|\s\.)/) {|m| m}
      #Ignore whitespaces and comments
      token(/\s+/)
      token(/\/\/\/.*\/\/\//m) #"m" stands for multiline
      token(/\/\/.*/)
      token(/\+\+/) {|m| m}      
      token(/\-\-/) {|m| m}
      token(/\+=/) {|m| m}
      token(/-=/) {|m| m}
      token(/\*=/) {|m| m}
      token(/\/=/) {|m| m}
      token(/%=/) {|m| m}
      token(/\^=/) {|m| m}
      token(/!=/) {|m| m}
      token(/<=/) {|m| m}
      token(/>=/) {|m| m}
      token(/==/) {|m| m}
      token(/\d+\.\d+/) {|m| m.to_f } #Float
      token(/\d+/) {|m| m.to_i } #Integers
      token(/"[^"]*"/) {|m| m} #Strings
      token(/^[A-Za-z_][A-Za-z\d_]*/) {|m| m} #Variables
      token(/w+/) {|m| m} #Other words
      token(/./) {|m| m }

      start :program do
        match(:statements) {|statements| Program.new(statements)}
      end

      #Statements should always return a list
      rule :statements do
        match(:function_declaration, :statements) {|fun_decl, statements| [fun_decl] + statements}
        match(:function_declaration) {|fun_decl| [fun_decl]}        
        match(:compound_statement, :statements) {|comp_statement, statements| 
          [comp_statement] + statements
        }
        match(:compound_statement) {|comp_statement| [comp_statement]}
        match(:simple_statement, ';', :statements) {|statement,_,statements| 
          [statement] + statements
        }
        match(:simple_statement, ';') {|statement,_| [statement] }
      end

      rule :simple_statement do
        match(:return_statement)
        match(:IO)
        match(:assignment)
        match(:expression)
      end

      #Basically statements without semi-colon
      rule :compound_statement do
        match('restart') { Restart.new }
        match(:start_statement)
        match(:loop_statement)
        match(:condition_statement)
      end

      rule :return_statement do
        match('return', :expression) {|_,expression,_| Return.new(expression) }
      end

      rule :IO do
        match('output', '(', :expression, ')') {|_,_,expr,_| Output.new(expr)}
        match('output', '(', :condition, ')') {|_,_,cond,_| Output.new(cond)}
        match(:var, :assignment_operator, 'input', '(', ')'){|var, assign_op,_,_,_| 
          Input.new(var, assign_op)
        }
        match(:var, :assignment_operator, 'input', '(', :expression, ')') {
          |var, assign_op,_,_,expr,_| 
          Input.new(var, assign_op, expr)
        }
      end

      rule :assignment do        
        match(:array_call, :assignment_operator, :expression) {|arr_call, assign_op, expr|
          Assignment.new(arr_call, assign_op, expr)
        }
        match(:var, :assignment_operator, :expression) {|var, assign_op, expr|
          Assignment.new(var, assign_op, expr)
        }
        match(:array_call, :increment_operator) {|arr_call, incr_op| 
          AssignmentIncrement.new(arr_call, incr_op)
        }
        match(:var, :increment_operator) {|var, incr_op| 
          AssignmentIncrement.new(var, incr_op)
        }
      end

      rule :condition do
        match(:condition, :logical_operator, :comparison) {|expr1, logical_op, expr2|
          Condition.new(expr1, logical_op, expr2)
        }
        match(:comparison)
      end

      rule :comparison do
        match(:comparison, :comparison_operator, :condition_data) {|expr1, cond_op, expr2|
          Condition.new(expr1, cond_op, expr2)
        }
        match(:condition_data)
      end

      rule :condition_data do
        match(:negation_operator, :condition) {|_,expr|
          Condition.new(expr, "!=", Expression.new(true))
        }
        match(:expression) {|expr| 
          Condition.new(expr) 
        }
        match('(', :condition, ')') {|_,cond,_| cond}
      end

      rule :expression do
        match(:arithmetic_expression)
        #String concatenation
        match(:expression, /(\s\.\s|\s\.)/, :expression) {|expr1,_, expr2| 
          Expression.new(StringConcatenation.new(expr1, expr2)) 
        }
      end

      rule :arithmetic_expression do        
        match(:arithmetic_expression, :add_operator, :multi_expression) {
          |expr1, op, expr2|
          Expression.new(expr1, op, expr2)
        }
        match(:multi_expression)
      end

      rule :multi_expression do
        match(:multi_expression, :multi_operator, :simple_expression) {|expr1, op, expr2|
          Expression.new(expr1, op, expr2)
        }
        match(:simple_expression)
      end
      
      rule :simple_expression do
        match(:function_call) {|fun_call| Expression.new(fun_call)}
        match(:array_call) {|arr_call| Expression.new(arr_call)}
        match(:data) {|data|
          Expression.new(data)
        }
        match(:var) {|var|
          Expression.new(var)
        }
        match("-", :simple_expression) {|_, expr|
          Expression.new(expr,'*', Expression.new(-1))
        }
        match("+", :simple_expression) {|_,expr|
          Expression.new(expr)
        }
        match('(', :arithmetic_expression, ')') {|_, expr, _| expr}
      end

      rule :function_call do
        match(:var, '(', :function_call_parameters, ')') {|var,_,param_data,_|
          FunctionCall.new(var.name, param_data)
        }
        match(:var, '(', ')') {|var,_,_| FunctionCall.new(var.name)}        
      end

      rule :function_call_parameters do
        match(:expression, ',', :function_call_parameters) {|expression,_,func_call|
          [expression] + [func_call]
        }
        match(:expression) {|expression| [expression] }        
      end

      rule :array_call do
        match(:var, '[', :expression ,']') {|var,_,index,_| ArrayCall.new(var, index) }
      end

      rule :data do
        match(/"[^"]*"/) {|m| m.tr('"','')}
        match(Float) {|float| float}
        match(Integer) {|integer| integer}
        match(Array) {|array| array}
        match('null') { Null.new }
        match('true') {true}
        match('false') {false}
        match(:array)
      end

      rule :array do
        match('[', :array_data, ']') {|_, array,_| array}
      end

      rule :array_data do
        match(:expression, ',', :array_data) {|expr,_,arr_data| [[expr.evaluate] + arr_data].flatten }
        match(:expression) {|expr| [expr.evaluate]}
      end

      rule :var do
        match(/^[A-Za-z_][A-Za-z\d_]*/) do |var|
          Variable.new(var)
        end
      end

      rule :start_statement do
       match('start', :start_block, 'stop') {|_, block, _| Start.new(block) }
      end

      rule :start_block do
        match(:statements) {|statements| statements}
      end

      rule :block do
        match('{', :statements, '}') {|_,statements,_| Block.new(statements)}
      end

      rule :loop_statement do
        #Regular loops (at least according to our definition)
        match('loop', :block) {|_, block| Loop.new(block)}
        match('loop', '(', :expression, ')',:block) {|_,_,count,_,block| 
          Loop.new(block, count)
        }

        #This matches array loops as well (loop(array, item))
        match('loop', '(', :expression, ',', :var, ')',:block) {
          |_,_,expr,_,var,_,block| Loop.new(block, expr, var.name)
        }
        match('loop', '(', :expression, ',', :var, ',', :assignment,')',:block) {
          |_,_,count,_,var,_,assignment,_,block| Loop.new(block, count, var.name, assignment) 
        }
        
        #While loop
        match('loop', '(', :condition, ')', :block) {|_,_,condition,_,block| 
          LoopWhile.new(block, condition)
        }

        #For-loop
        match('loop', '(', :assignment, ',', :condition, ',', :assignment, ')', :block) {
          |_,_,init_assign,_,condition,_,assignment,_,block| 
          LoopFor.new(block, init_assign, condition, assignment)
        }
      end

      rule :condition_statement do
        match(:if_statement, :elseif_statement, :else_statement) {
          |if_stmt, elseif_stmt, else_stmt|
          ConditionStatement.new(if_stmt, elseif_stmt, else_stmt)
        }
        match(:if_statement, :elseif_statement) {|if_stmt, elseif_stmt|
          ConditionStatement.new(if_stmt, elseif_stmt)
        }
        match(:if_statement, :else_statement) {|if_stmt, else_stmt|
          #Empty array since there are no else-ifs
          ConditionStatement.new(if_stmt, [], else_stmt)
        }
        match(:if_statement) {|if_stmt| ConditionStatement.new(if_stmt)}
      end

      rule :if_statement do
        match('if', '(', :condition, ')', :block) {|_,_,condition,_,block|
          If.new(condition, block)
        }
      end   
    
      #Returns list of if-statements
      rule :elseif_statement do
        match('elseif', '(', :condition, ')', :block, :elseif_statement) {
          |_,_,condition,_, block, elseif_stmt|
          [If.new(condition, block)] + elseif_stmt
        }
        match('elseif', '(', :condition, ')', :block) {|_,_,condition,_,block|
          [If.new(condition, block)]
        }
      end

      rule :else_statement do
        match('else', :block) {|_, block| block}
      end

      rule :function_declaration do
        match('fun', :var, '(', ')', :block) {|_, var,_,_,block|
          Function.new(var.name, block)
        }
        match('fun', :var, '(', :parameter_list, ')', :block) {|_,var,_,params,_,block|
          Function.new(var.name, block, params)
        }
      end

      rule :parameter_list do
        match(:var, ',', :parameter_list) {|var,_,params| [var.name] + params }
        match(:default_parameter, ',', :parameter_list) {|def_para,_,params| 
          [def_para] + params
        }
        match(:default_parameter) {|def_para| [def_para]}
        match(:var) {|var| [var.name]}
      end

      rule :default_parameter do
        match(:var, '=', :data) {|var, _, data|
          Assignment.new(var, '=', Expression.new(data))
        }
      end

      rule :fun_block do
        match('{', :statements, :return_statement, :statements, '}') {
          |_,statements, return_stmt,statements2,_|
          #Create with combined list of statements
          Block.new(statements + [return_stmt] + statements2)
        }
        match('{', :return_statement, '}') {
          |_, return_stmt, _|
          Block.new([return_stmt])
        }
        match(:block)
      end

      #---------OPERATORS---------
      rule :add_operator do
        match('+') {|op| op }
        match('-') {|op| op }
      end

      rule :multi_operator do
        match('^') {|op| '**' }
        match('*') {|op| op }
        match('/') {|op| op }
        match('%') {|op| op }
      end

      rule :assignment_operator do
        match('=') {|op| op }
        match('+=') {|op| op }
        match('-=') {|op| op }
        match('*=') {|op| op }
        match('/=') {|op| op }
        match('%=') {|op| op }
        match('^=') { '**=' }
      end

      rule :increment_operator do
        match('++') {|op| op}
        match('--') {|op| op}
      end

      rule :logical_operator do
        match('&') {'&&'}
        match('and') {'&&'}
        match('|') {'||'}
        match('or') {'||'}
      end

      rule :comparison_operator do
        match('!=') {|op| op}
        match('not', 'equals') {'!='}
        match('==') {|op| op}
        match('equals') {'=='}
        match('<=') {|op| op}
        match('>=') {|op| op}
        match('<') {|op| op}
        match('>') {|op| op}
      end

      rule :negation_operator do
        match('not') {|m| m}
        match('!') {|m| m}
      end
    end
  end
  
  def done(str)
    ["quit","exit","bye",""].include?(str.chomp)
  end
  
  def parse(filepath = nil)
    #Parse code from file
    if (filepath != nil)
      if not File.file?(filepath)
        puts "Can't find file '#{filepath}'"
      else
        code = File.read(filepath)
        #Parse and run the program
        program = @termerParser.parse code
        program.run if program != nil
      end
    #Parse interactive
    else
      print "[Termer] "
      str = gets
      if done(str)
        puts "Bye."
      #Shortcut to print current variables
      elsif str.chomp.eql?("pv")
        print "#{@@variables}\n"
        parse
      else
        #Parse and run the program
        program = @termerParser.parse str
        program.run if program != nil
        #Parse next statement
        parse
      end  
    end
  end

  def log(state = true)
    if state
      @termerParser.logger.level = Logger::DEBUG
    else
      @termerParser.logger.level = Logger::WARN
    end
  end
end

#Only run code when running file from terminal
if __FILE__==$0
  termer = Termer.new
  #Parse from filename
  if (ARGV[0] != nil)
    termer.parse(ARGV[0])    
  else
    #Parse interactively
    termer.parse
  end
end