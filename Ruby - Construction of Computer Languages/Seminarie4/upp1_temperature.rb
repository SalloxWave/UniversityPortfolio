#!/usr/bin/env ruby
# ----------------------------------------------------------------------------
#  Assignment
# ----------------------------------------------------------------------------

# Uppgift 1 inför fjärde seminariet innebär två saker:
# - Först ska ni skriva enhetstester för Adder och Multiplier. Det är inte
#   helt säkert att de funkar som de ska. Om ni med era tester upptäcker
#   fel ska ni dessutom korrigera Adder och Multiplier.
# - Med hjälp av Adder och Multiplier m.m. ska ni sedan bygga ett nätverk som
#   kan omvandla temperaturer mellan Celsius och Fahrenheit. (Om ni vill
#   får ni ta en annan ekvation som är ungefär lika komplicerad.)

# Ett tips är att skapa en funktion celsius2fahrenheit som returnerar
# två Connectors. Dessa två motsvarar Celsius respektive Fahrenheit och 
# kan användas för att mata in temperatur i den ena eller andra skalan.

require './constraint_networks.rb'

#Class used for creating constraint network for convertion between celsius and fahrenheit
class TemperatureConstraint
  include PrettyPrint

  attr_accessor :c, :f
  attr_reader :logger

  def initialize(c, f)
    @logger=Logger.new(STDOUT)
    @c,@f = [c,f]
    [c,f].each { |x| x.add_constraint(self) }
  end
  
  def to_s
    "9*#{c} = 5*(#{f}-32)"
  end
  
  def new_value(connector)
    ## 9*c = 5*(f-32)
    if [c].include?(connector) and (not f.has_value?) then
      ## f = (9*c/5)+32
      val= ((9*c.value)/5)+32
      logger.debug("#{self} : #{f} updated")
      f.assign(val, self)
    elsif [f].include?(connector) and (not c.has_value?) then
      ## c = (5*(f-32))/9
      val= (5*(f.value-32))/9
      logger.debug("#{self} : #{c} updated")
      c.assign(val, self)
    end
    self
  end
  
  # A connector lost its value, so propagate this information to all
  # others
  def lost_value(connector)
    ([c,f]-[connector]).each { |connector| connector.forget_value(self) }
  end
  
end

def celsius2fahrenheit
  # Klistra in er kod här.
  c = Connector.new('c')
  f = Connector.new('f')
  TemperatureConstraint.new(c, f)
  return c, f
end

# Ni kan då använda funktionen så här:



#c,f=celsius2fahrenheit
#f.user_assign 0


# irb(main):1988:0> c,f=fahrenheit2celsius
# <någonting returneras>
# irb(main):1989:0> c.user_assign 100
# D, [2007-02-08T09:15:01.971437 #521] DEBUG -- : c ignored request
# D, [2007-02-08T09:15:02.057665 #521] DEBUG -- : c got new value: 100
# D, [2007-02-08T09:15:02.058046 #521] DEBUG -- : c * 9 == 9c : 9c updated
# D, [2007-02-08T09:15:02.058209 #521] DEBUG -- : 9c got new value: 900
# D, [2007-02-08T09:15:02.058981 #521] DEBUG -- : f-32 * 5 == 9c : f-32 updated
# D, [2007-02-08T09:15:02.059156 #521] DEBUG -- : f-32 got new value: 180
# D, [2007-02-08T09:15:02.059642 #521] DEBUG -- : f-32 + 32 == f : f updated
# D, [2007-02-08T09:15:02.059792 #521] DEBUG -- : f got new value: 212
# "ok"
# irb(main):1990:0> f.value
# 212
# irb(main):1991:0> c.user_assign 0
# D, [2007-02-08T09:15:19.433621 #521] DEBUG -- : c lost value
# D, [2007-02-08T09:15:19.501880 #521] DEBUG -- : Notifying c * 9 == 9c
# D, [2007-02-08T09:15:19.502214 #521] DEBUG -- : 9 ignored request
# D, [2007-02-08T09:15:19.502380 #521] DEBUG -- : 9c lost value
# D, [2007-02-08T09:15:19.502527 #521] DEBUG -- : Notifying f-32 * 5 == 9c
# D, [2007-02-08T09:15:19.502701 #521] DEBUG -- : f-32 lost value
# D, [2007-02-08T09:15:19.502863 #521] DEBUG -- : Notifying f-32 + 32 == f
# D, [2007-02-08T09:15:19.503031 #521] DEBUG -- : 32 ignored request
# D, [2007-02-08T09:15:19.503427 #521] DEBUG -- : f lost value
# D, [2007-02-08T09:15:19.503570 #521] DEBUG -- : 5 ignored request
# D, [2007-02-08T09:15:19.503699 #521] DEBUG -- : c got new value: 0
# D, [2007-02-08T09:15:19.503860 #521] DEBUG -- : c * 9 == 9c : 9c updated
# D, [2007-02-08T09:15:19.503963 #521] DEBUG -- : 9c got new value: 0
# D, [2007-02-08T09:15:19.504111 #521] DEBUG -- : f-32 * 5 == 9c : f-32 updated
# D, [2007-02-08T09:15:19.504210 #521] DEBUG -- : f-32 got new value: 0
# D, [2007-02-08T09:15:19.504356 #521] DEBUG -- : f-32 + 32 == f : f updated
# D, [2007-02-08T09:15:19.534416 #521] DEBUG -- : f got new value: 32
# "ok"
# irb(main):1992:0> f.value
# 32
# irb(main):1993:0> c.forget_value "user"
# D, [2007-02-08T09:19:56.754866 #521] DEBUG -- : c lost value
# D, [2007-02-08T09:19:56.842475 #521] DEBUG -- : Notifying c * 9 == 9c
# D, [2007-02-08T09:19:56.844665 #521] DEBUG -- : 9 ignored request
# D, [2007-02-08T09:19:56.844855 #521] DEBUG -- : 9c lost value
# D, [2007-02-08T09:19:56.845021 #521] DEBUG -- : Notifying f-32 * 5 == 9c
# D, [2007-02-08T09:19:56.845195 #521] DEBUG -- : f-32 lost value
# D, [2007-02-08T09:19:56.845363 #521] DEBUG -- : Notifying f-32 + 32 == f
# D, [2007-02-08T09:19:56.845539 #521] DEBUG -- : 32 ignored request
# D, [2007-02-08T09:19:56.845664 #521] DEBUG -- : f lost value
# D, [2007-02-08T09:19:56.845790 #521] DEBUG -- : 5 ignored request
# "ok"
# irb(main):1994:0> f.user_assign 100
# D, [2007-02-08T09:20:14.367288 #521] DEBUG -- : f ignored request
# D, [2007-02-08T09:20:14.465708 #521] DEBUG -- : f got new value: 100
# D, [2007-02-08T09:20:14.466057 #521] DEBUG -- : f-32 + 32 == f : f-32 updated
# D, [2007-02-08T09:20:14.466261 #521] DEBUG -- : f-32 got new value: 68
# D, [2007-02-08T09:20:14.466436 #521] DEBUG -- : f-32 * 5 == 9c : 9c updated
# D, [2007-02-08T09:20:14.466547 #521] DEBUG -- : 9c got new value: 340
# D, [2007-02-08T09:20:14.466714 #521] DEBUG -- : c * 9 == 9c : c updated
# D, [2007-02-08T09:20:14.468579 #521] DEBUG -- : c got new value: 37
# "ok"
# irb(main):1995:0> c.value
# 37
