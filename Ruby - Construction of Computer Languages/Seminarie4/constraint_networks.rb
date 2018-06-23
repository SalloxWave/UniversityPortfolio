#!/usr/bin/env ruby

# ----------------------------------------------------------------------------
#  Unidirectional constraint network for logic gates
# ----------------------------------------------------------------------------

# This is a simple example of a constraint network that uses logic gates. 
# There are three classes of gates: AndGate, OrGate, and NotGate. 
# Connections between gates are modelled as the class Wire.

require 'logger'

class BinaryConstraint

  def initialize(input1, input2, output)
    @input1=input1
    @input1.add_constraint(self)
    @input2=input2
    @input2.add_constraint(self)
    @output=output
    new_value()
  end
  
end

class AndGate < BinaryConstraint
  
  def new_value
    sleep 0.2
    @output.value=(@input1.value and @input2.value)
  end
  
end

class OrGate < BinaryConstraint
    
  def new_value
    sleep 0.2
    @output.value=(@input1.value or @input2.value)
  end
  
end

class NotGate
  
  def initialize(input, output)
    @input=input
    @input.add_constraint(self)
    @output=output
    new_value
  end
  
  def new_value
    sleep 0.2
    @output.value=(not @input.value)
  end
  
end

class Wire
  
  attr_accessor :name
  attr_reader :value

  def initialize(name, value=false)
    @name=name
    @value=value
    @constraints=[]
    @logger=Logger.new(STDOUT)
  end
  
  def log_level=(level)
    @logger.level=level
  end
  
  def add_constraint(gate)
    @constraints << gate
  end
  
  def value=(value)
    @logger.debug("#{name} = #{value}")
    @value=value
    @constraints.each { |c| c.new_value }
  end
  
end

# When you use test_constraints, it will prompt you for input before
# proceeding. That way you can analyze what happens in the code before
# you go on. You only need to press 'Enter' to continue.

def test_constraints
  a=Wire.new('a')
  b=Wire.new('b')
  c=Wire.new('c')

  # If you want to see when c changes value, set the log_level of c to
  # Logger::DEBUG
  c.log_level=Logger::DEBUG

  puts "Testing the AND gate"
  andGate=AndGate.new(a, b, c)
  a.value=true
  gets
  b.value=true
  gets
  a.value=false
  gets

  a=Wire.new('a')
  b=Wire.new('b')
  c=Wire.new('c')
  puts "Testing the OR gate"
  orGate=OrGate.new(a, b, c)
  a.value=false
  gets  
  b.value=false
  gets
end

# ----------------------------------------------------------------------------
#  Bidirectional constraint network for arithmetic constraints
# ----------------------------------------------------------------------------

# In the example above, our constraint network was unidirectional.
# That is, changes could not propagate from the output wire to the
# input wires. However, to model equation systems such as the
# correlation betwen the two units of measurement Celsius and
# Fahrenheit, we need to propagate changes from either end to the
# other.

module PrettyPrint

  # To make printouts of connector objects easier, we define the
  # inspect method so that it returns the value of to_s. This method
  # is used by Ruby when we display objects in irb. By defining this
  # method in a module, we can include it in several classes that are
  # not related by inheritance.

  def inspect
    "#<#{self.class}: #{to_s}>"
  end

end

# This is the base class for Adder and Multiplier.

class ArithmeticConstraint

  include PrettyPrint

  attr_accessor :a,:b,:out
  attr_reader :logger,:op,:inverse_op

  def initialize(a, b, out)
    @logger=Logger.new(STDOUT)
    @a,@b,@out=[a,b,out]
    [a,b,out].each { |x| x.add_constraint(self) }
  end
  
  def to_s
    "#{a} #{op} #{b} == #{out}"
  end
  
  def new_value(connector)
    if [a,b].include?(connector) and a.has_value? and b.has_value? and 
        (not out.has_value?) then 
      # Inputs changed, so update output to be the sum of the inputs
      # "send" means that we send a message, op in this case, to an
      # object.
      val=a.value.send(op, b.value)
      logger.debug("#{self} : #{out} updated")
      out.assign(val, self)
    elsif [out].include?(connector) then                                ## HERE STARTS OUR FIX!
      # if <c> has value, but either <a> or <b> does not:
      # take <c> inverse_op <a|b>
      if ( not a.has_value? )
        val=out.value.send(inverse_op, b.value)
        logger.debug("#{self} : #{a} updated")
        a.assign(val, self)
      end
      if ( not b.has_value? )
        val=out.value.send(inverse_op, a.value)
        logger.debug("#{self} : #{b} updated")
        b.assign(val, self)
      end                                                               ## HERE ENDS OUR FIX!
    end
    self
  end

  def get_op
    return self.op
  end
  
  # A connector lost its value, so propagate this information to all
  # others
  def lost_value(connector)
    ([a,b,out]-[connector]).each { |connector| connector.forget_value(self) }
  end
  
end

class Adder < ArithmeticConstraint
  
  def initialize(*args)
    super(*args)
    @op,@inverse_op=[:+,:-]
  end
end


class Multiplier < ArithmeticConstraint

  def initialize(*args)
    super(*args)
    @op,@inverse_op=[:*,:/]
  end
end


class ContradictionException < Exception
end

# This is the bidirectional connector which may be part of a constraint network.

class Connector
    
  include PrettyPrint

  attr_accessor :name,:value

  def initialize(name, value=false)
    self.name=name
    @has_value=(not value.eql?(false))
    @value=value
    @informant=false
    @constraints=[]
    @logger=Logger.new(STDOUT)
  end

  def add_constraint(c)
    @constraints << c
  end
    
  # Values may not be set if the connector already has a value, unless
  # the old value is retracted.
  def forget_value(retractor)
    if @informant==retractor then
      @has_value=false
      @value=false
      @informant=false
      @logger.debug("#{self} lost value")
      others=(@constraints-[retractor])
      @logger.debug("Notifying #{others}") unless others == []
      others.each { |c| c.lost_value(self) }
      "ok"
    else
      @logger.debug("#{self} ignored request")
    end
  end

  def has_value?
    @has_value
  end
  
  # The user may use this procedure to set values
  def user_assign(value)
    forget_value("user")
    assign value,"user"
  end
  
  def assign(v,setter)
      if not has_value? then
        @logger.debug("#{name} got new value: #{v}")
        @value=v
        @has_value=true
        @informant=setter
        (@constraints-[setter]).each { |c| c.new_value(self) }
        "ok"
      else
        if value != v then
          raise ContradictionException.new("#{name} already has value #{value}.\nCannot assign #{name} to #{v}")
      end
    end
  end
  
  def to_s
    name
  end

end

class ConstantConnector < Connector
  
  def initialize(name, value)
    super(name, value)
    if not has_value?
      @logger.warn "Constant #{name} has no value!"
    end
  end
  
  def value=(val)
    raise ContradictionException.new("Cannot assign a constant a value!")
  end
end
  
# This is a simple test of the constraint network

def test_adder
  a = Connector.new("a")
  b = Connector.new("b")
  c = Connector.new("c")
  Adder.new(a, b, c)
  a.user_assign(10)
  b.user_assign(5)
  puts "c = "+c.value.to_s
  a.forget_value "user"
  c.user_assign(20)
  # a should now be 15
  puts "a = "+a.value.to_s
end