require './rdparse.rb'

class Termer
  
  def initialize
    @termerParser = Parser.new("dice roller") do
      token(/\s+/)
      token(/\d+/) {|m| m.to_i }
      token(/./) {|m| m }

      
      # STATEMENTS
      start :statements do
        match(:simle_statement, ';')
        match(:simple_statement, ';', :statements)
        match(:compound_statement)
        match(:compound_statement, :statements)
        match(:class)
        match(:class, :statements)
        match(:fun_declaration)
        match(:fun_declaration, :statements)
      end
      start :statement do
        match(:simple_statement)
        match(:compound_statement)
      end

      # STATEMENTS REQUIRING SEMICOLON
      start :simple_statement do
        match(:expression)
        match(:assignment)
        match(:IO)
      end

      # STATEMENTS WITH BLOCKS THAT DOES NOT REQUIRE SEMICOLON
      start :compound_statement do
        match(:condition_statement)
        match(:loop_statement)
        match(:start_statement)
      end

      # EXPRESIONS
      start :expression do
        match(:data)
        match(:name)
        match(:function_call)
        match(:expression, :arithmetic_operator, :expression)
        match(:condition)
        match(:data, '.', :expression)
      end

      # FUNCTION CALL
      start :function_call do
        match(:name, '.', :function_call)
        match(:name, '.', :name)
      end

      # ASSIGNMENTS
      start :assignment do
        match(:name, :assignment_operator, :expression)
        match(:name, :increment_operator)
      end

      # CONDITION STATEMENTS
      start :condition_statement do
        match(:if_statement)
        match(:if_statement, :else_statement)
        match(:if_statement, :elseif_statement, :else_statement)
      end

      # IF STATEMENTS
      start :if_statement do
        match('if', '(', :condition, ')', :block)
      end
      start :elseif_statement do
        match('elseif', '(', :condition, ')', :block)
        match('elseif', '(', :condition, ')', :block, :elseif_statement)
      end
      start :else_statement do
        match('else', :block)
      end

      # CONDITIONS
      start :condition do
        match(:expression)
        match(:condition, :logical_operator, :condition)
        match(:expression, :condition_operator, :expression)
      end

      # FUNCTION DECLARATION
      start :fun_declaration do
        match('fun', :name, '(', ')', :fun_block)
        match('fun', :name, '(', :parameter_list, ')', :fun_block)
      end
      # FUNCTION BLOCKS
      start :fun_block do
        match(:block)
        match('{', :statements, :return_statement, '}')
      end

      # RETURN STATEMENTS
      start :return_statement do
        match('return', :name)
        match('return', :data)
      end

      # PARAMETER LIST
      start :parameter_list do
        match(:name)
        match(:default_assignment)
        match(:name, ',', :parameter_list)
      end

      # DEFAULT ASSIGNMENT
      start :default_assignment do
        match(:name, '=', :data)
      end

      # BLOCK
      start :block do
        match('{', :statements, '}')
      end

      # LOOP STATEMENTS
      start :loop_statement do
        match('loop', :block)
        match('loop', '(', :loop_parameters, ')', :block)
      end
      # LOOP PARAMETERS
      start :loop_parameters do
        match(Integer, ',', :name)
        match(Integer, ',', :name, ',', :assignment)
        match(Integer, ',', :condition, ',', :assignment)
        match(:condition)
        match(Array, ',', :name) # ARRAY !?!?!?!?
      end

      # IO
      start :IO do
        match(:name, :assignment_operator, 'input', '(', String, ')')
        match('output', '(', :data, '(', String, ')', ')') # HELT FEL!?!?!?
      end

      # START STUFF
      start :start_statement do
        match('start', :start_block, 'stop')
      end
      start :start_block do
        match(:statements)
        match(:statements, 'restart')
        match(:statements, 'restart', :statements)
      end

      # CLASS STUFF
      start :class do
        match('class', :name, :class_block)
      end
      start :class_block do
        match('{', :class_statements, '}')
      end
      start :class_statements do
        match(:class_statement)
        match(:class_statement, :class_statements)
      end
      start :class_statement do
        match(:class_constructor)
        match(:access_modifier, :class_attr_block)
        match(:access_modifier, :class_fun_block)
      end
      start :class_constructor do
        match('constructor', '(', :parameter_list, ')', :block)
        match('auto', 'constructor', '(', :parameter_list, ')')
      end
      start :class_attr_block do
        match('{', :class_attr_statements, '}')
      end
      start :class_attr_statements do
        match(:class_attr_statement, ';', :class_attr_statements)
        match(:class_attr_statement, ';')
        match(:default_assignment, ';')
        match(:name, ';')
      end
      start :class_fun_block do
        match('fun', '{', :class_fun_statements, '}')
      end
      start :class_fun_statements do
        match(:class_fun, :class_fun_statements)
        match(:class_fun)
      end
      start :class_fun do
        match(:name, '(', ')', :fun_block)
        match(:name, '(', :parameter_list, ')', :fun_block)
      end

      # ACCESS MODIFIER
      start :access_modifier do
        match('getter')
        match('setter')
        match('private')
        match('public')
        match('static')
      end

      
      # EXTRA STUFF
      start :arithmetic_operator do
        match('+')
        match('-')
        match('*')
        match('/')
        match('%')
        match('^')
      end
      start :assignment_operator do
        match('=')
        match('+=')
        match('-=')
        match('*=')
        match('/=')
        match('%=')
      end
      start :increment_operator do
        match('++')
        match('--')
      end
      start :condition_operator do
        match('!=')
        match('not', 'equals')
        match('<')
        match('>')
        match('<=')
        match('>=')
        match('==')
        match('equals')
      end
      start :logical_operator do
        match('&')
        match('and')
        match('"') # HELT FEL ???? VARFÖR !?!?!?
        match('or')
        match('!')
        match('not')
      end

      start :data do # LÄGGA TILL MESTADELS AV DETTA
        match(String)
        match(Integer)
        match(Bool)
        match(Decimal)
        match(Char)
        match(Null)
        match(Array)
      end

      start :name do
        match(Identifier) # LÄGGA TILL DENNA
      end
    end
  end
  
  def done(str)
    ["quit","exit","bye",""].include?(str.chomp)
  end
  
  def parse(filepath = nil)
    #Parse code from file
    if (filepath != nil)
      code = File.open(filepath)
      puts "#{@termerParser.parse code}"
    #Parse interactive
    else
      print "[Termer] "
      str = gets
      if done(str)
        puts "Bye."
      else
        puts "=> #{@termerParser.parse str}"
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

Termer termer = Termer.new
#Parse from filename
if (ARGS[0] != nil)
  termer.parse(ARGS[0])
else
  termer.parse
end
