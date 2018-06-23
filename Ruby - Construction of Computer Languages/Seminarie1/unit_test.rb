#!/usr/bin/env ruby
require 'test-unit'
require './uppgift01'
require './uppgift02'
require './uppgift03'
require './uppgift04'
require './uppgift05'
require './uppgift06'
require './uppgift07'
require './uppgift08'
require './uppgift09'
require './uppgift10'
require './uppgift11'
require './uppgift12'

#Note:
#Search for Uppgift x to find wanted uppgift

class TestFaculty < Test::Unit::TestCase

  #Uppgift 1
  def test_1
    #Check if repeat object has correct amount of count stored
    do_three = Repeat.new(3)
    assert_equal(3, do_three.count)
    do_three = Repeat.new(-1)
    assert_equal(-1, do_three.count)
    do_three = Repeat.new(0)
    assert_equal(0, do_three.count)
  end

  #Uppgift 2
  def test_2
    #Check correct faculty
    assert_equal(6, faculty(3))
    assert_equal(120, faculty(5))
    assert_equal(40320, faculty(8))
    assert_equal(2432902008176640000, faculty(20))

    #Check cases where faculty should be 1 ( <=1 )
    assert_equal(1, faculty(0))
    assert_equal(1, faculty(1))
    assert_equal(1, faculty(-1))
    assert_equal(1, faculty(-3))
  end

  #Uppgift 3
  def test_3
    #Check if correct string are returned
    assert_equal("apelsin", longest_string(["apelsin", "banan", "citron"]))
    assert_equal("123456789", longest_string(["hej", "hej då", "123456789"]))
    assert_equal("banan", longest_string(["banan", "banan", "banan"]))
    assert_equal("  banan  ", longest_string(["  banan  ", "banan", "banan"]))

    #When multiple string have same length, the first found should be returned
    assert_equal("banan1", longest_string(["banan1", "banan2", "banan"]))
    assert_not_equal("banan3", longest_string(["banan1", "banan2", "banan3"]))
  end

  #Uppgift 4
  def test_4
    #Check if correct string are returned
    assert_equal("apelsin", find_it(["apelsin", "banan", "citron"]) { |a,b| a.length > b.length } )
    assert_equal("jonas", find_it(["jonas", "jonas2", "apelsin"]) { |a,b| a.length < b.length } )

    #No errors when having the same length
    assert_nothing_thrown { find_it(["jonas", "jonas", "jonas"]) { |a,b| a.length > b.length } }

    #When all are equal length the first should be returned
    assert_equal("hej1", find_it(["hej1", "hej2", "hej3"]) { |a,b| a.length < b.length } )
    assert_not_equal("hej3", find_it(["hej1", "hej2", "hej3"]) { |a,b| a.length < b.length } )
  end

  #Uppgift 5
  def test_5
    #Check if name has correct names (fullname is with surname first)
    name_test = PersonName.new
    name_test.fullname = "Person pärsonna"
    assert_equal("Pärsonna Person", name_test.fullname)
    assert_equal("Person", name_test.name)
    assert_equal("Pärsonna", name_test.surname)

    #Also check if names get capitalized
    name_test = PersonName.new
    name_test.fullname = "craZy gUy"
    assert_equal("Guy Crazy", name_test.fullname)
    assert_equal("Crazy", name_test.name)
    assert_equal("Guy", name_test.surname)
  end

  #Uppgift 6
  def test_6
    #Check if person has correct name and age
    person = Person.new("aWesomE", "NamesSon", 38)
    assert_equal("Awesome", person.name)
    assert_equal("Namesson", person.surname)
    assert_equal("Namesson Awesome", person.fullname)
    assert_equal(38, person.age)
    assert_equal(1979, person.birthyear)

    #When age is changed to 20
    person.age = 20
    #Both age and birthyear should have changed
    assert_equal(20, person.age)
    assert_equal(1997, person.birthyear)

    #When birthyear is changed to 1900
    person.birthyear = 1900
    #Both age and birthyear should have changed
    assert_equal(1900, person.birthyear)
    assert_equal(117, person.age)

    #Test if default parameters works as intended
    person = Person.new
    assert_equal("Unknown", person.name)
    assert_equal("Unknown", person.surname)
    assert_equal("Unknown Unknown", person.fullname)
    assert_equal(0, person.age)
    assert_equal(Date.today.year, person.birthyear)

    person = Person.new("Alexander", "Jonsson")
    assert_equal(0, person.age)
    assert_equal(Date.today.year, person.birthyear)
    assert_equal("Alexander", person.name)
    assert_equal("Jonsson", person.surname)
    assert_equal("Jonsson Alexander", person.fullname)
  end

  #Uppgift 7
  def test_7
    #Test first two fibonacci numbers which should be 1
    assert_equal(1, 1.fib)
    assert_equal(1, 2.fib)
    #Test various fibonacci numbers
    assert_equal(2, 3.fib)
    assert_equal(5, 5.fib)
    assert_equal(21, 8.fib)
    assert_equal(144, 12.fib)
  end

  #Uppgift 8
  def test_8
    #Test various normal cases
    assert_equal("LOL", "Laugh out Loud".acronym)
    assert_equal("DWIM", "Do what I mean!!".acronym)
    assert_equal("A", "awesome".acronym)
    assert_equal("", "".acronym)

    #Check if non-letters are removed correctly
    assert_equal("MNIN", "!ello my name is () !! name".acronym)
    assert_equal("TWBT", "!¤ $! ?hi this will BE TWBT".acronym)
    assert_equal("OTT", "1 23 3three one two three 1 38 2".acronym)
  end

  #Uppgift 9
  #Want to go negative steps?
  def test_9
    #Test rotating arrays one step
    arr = [1,2,3]
    arr.rotate_left
    assert_equal([2,3,1], arr)

    arr = [6,5,4,3,2,1]
    arr.rotate_left
    assert_equal([5,4,3,2,1,6], arr)

    arr = [1]
    arr.rotate_left
    assert_equal([1], arr)

    #Test rotating various amount of steps on various arrays
    arr = [1,2,3,4]
    arr.rotate_left(4)
    assert_equal([1,2,3,4], arr)

    arr = [1,2,3,4]
    arr.rotate_left(2)
    assert_equal([3,4,1,2], arr)

    arr = [1,2]
    arr.rotate_left(3)
    assert_equal([2,1], arr)

    arr = [1,2]
    arr.rotate_left(4)
    assert_equal([1,2], arr)
  end

  #Uppgift 10
  def test_10
    #Try different strings and see if you can find the correct username
    assert_equal("alejo720", get_username("USERNAME: alejo720"))
    assert_equal("Brian", get_username("annoying text trying to ruin the day!! !!USERNAME: Brian"))
    assert_equal("Iamaname", get_username("userName USERNAMe UserName  !?!?! USERNAME:imnotausername USERNAME: Iamaname"))
    #Try different strings that should not return usable data
    assert_equal("", get_username("USERNAME: "))
    assert_equal("", get_username("Hello world!"))
  end

  #Uppgift 11
  def test_11
    #Try a normal tag string
    html="<body><html></html></body>"
    assert_equal(["body", "html", "html", "body"], tag_names(html))

    #Try tag string with various problems
    html = "<table cellspacing=\"0\" cellpadding=\"0\">
 <body>
 <tr style=\"height: 56px;\">
  <td id=\"projectalign\" style=\"padding-left: 0.5em;\">
   <div id=\"projectname\">My Project /hello>
   </div>
  </td>
 </tr>
 </body>
</table>
</div>"
    assert_equal(["table", "body", "tr", "td", "div", "div", "td", "tr", "body", "table", "div"], tag_names(html))

    #Check that the tag is a valid tag
    html = "/hello>"
    assert_not_equal(["hello"], tag_names(html))
    html = "notatag <meneither!!!! <iamatag>"
    assert_not_equal(["iamatag"], tag_names(html))
  end

  #Uppgift 12
  def test_12
    #Try various cases that should work 
    assert_equal("FMA297" ,regnr("Min bil heter FMA297"))
    assert_equal("RTJ737" ,regnr("RTJ737öäöasd123   annoying text"))
    assert_equal("ABC123" ,regnr("ABC123"))

    #Try not allowed reg numbers
    assert_equal(false, regnr("XQT784"))
    assert_equal(false, regnr("!MA998"))
    assert_equal(false, regnr("ABCD12"))
    assert_equal(false, regnr("123ABC"))
    assert_equal(false, regnr("1234AB"))
    assert_equal(false, regnr("IJK123"))

    #Try multiple reg numbers in same string
    assert_equal("ABC123", regnr("ABC123CBA321"))
    assert_equal("ALE123", regnr("ALE123AND38"))
  end
end
