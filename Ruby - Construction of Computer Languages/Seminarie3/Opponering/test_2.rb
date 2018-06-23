# -*- coding: utf-8 -*-
require 'test/unit'
require_relative './rdparse'
class TC_MyTest < Test::Unit::TestCase

  def setup
    @lang = LogicExpression.new
    @lang.log false
    @lang.logicExpr("( set x true )")
    @lang.logicExpr("( set y false )")
    @lang.logicExpr("( set z true )")
    @lang.logicExpr("( set b false )")
  end

#######################################################
  def test_1
    assert_equal(true, @lang.container["x"])
  end
  
  def test_2
    assert_equal(false, @lang.container["y"])
  end
  
  def test_3
    assert_equal(true, @lang.container["z"])
  end

  def test_4
    assert_equal(false, @lang.logicExpr('(and y x)'))
  end

  def test_5
    assert_equal(false, @lang.logicExpr('(and z y)'))
  end

  def test_6
    assert_equal(true, @lang.logicExpr('(and x z)'))
  end

  def test_7
    assert_equal(false, @lang.logicExpr('(and y b)'))
  end

  def test_8
    assert_equal(false, @lang.logicExpr('(not x)'))
  end
  def test_9
    assert_equal(true, @lang.logicExpr('(or y x)'))
  end
end
