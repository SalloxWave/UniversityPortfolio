=begin 
How to use DSL:

Create table data:
	There are five different factors deciding the points that can be used:
	car_brand, zip_code, license_years, gender and age (note that you have to use underscore to separate between words that go together).
	To connect a value to an amount of points you write like this:
	factor "value", "points" where value and points should be changed to what you want. 
	For example you could write: 
	car_brand "BMW", "5"
	age "38-42", "10"

Create rules:
	To create a rule you write like this:
	rule "condition", "modifier(multiplier").
	The condition can use the five factors together with comparison key words and values. For example:
	rule "car brand is 'Opel'", "10"
	rule "age greater than 42 and license years greater than ", "6"
	rule "car_brand starts with 'F'", "3.5"

	Comparison key words usable in condition:
	is, and, starts with, less than, greater than.

Important notes:
	- If a value is used inside a condition, you must use single quotes ('') around that value. See rule examples above.
	- Factors with muliple words insde condition is less strict than outside. They can be pretty much separated with anything but preferable is a space or underscore.
	- Range values are written with numbers combined with hyphen, for example 1-5 or 20-100
=end

car_brand "BMW", "5"
car_brand "Citroen", "4"
car_brand "Fiat", "3"
car_brand "Ford", "4"
car_brand "Mercedes", "5"
car_brand "Nissan", "4"
car_brand "Opel", "4"
car_brand "Volvo", "5"

zip_code "58937", "9"
zip_code "58726", "5"
zip_code "58647", "3"

license_years "0-1", "3"
license_years "2-3", "4"
license_years "4-15", "4,5"
license_years "16-99", "5"

gender "male", "1"
gender "female", "1"

age "18-20", "2.5"
age "21-23", "3"
age "24-26", "3.5"
age "27-29", "4"
age "30-39", "4.5"
age "40-64", "5"
age "65-70", "4"
age "71-99", "3"

rule "car brand is 'volvo' and zip code starts with '58'", "0.9"
rule "gender is 'male' and license years less than 3", "1.2"