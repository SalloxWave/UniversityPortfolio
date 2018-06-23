class Person
  def initialize(car, zip, license_age, gender, age)
    @car = car
    @zip = zip
    @license_age = license_age
    @gender = gender
    @age = age
  end
  
  def evaluate_policy(filename)
    @score = 0
    instance_eval(File.read(filename))
    
    if @gender == "Man" && @license_age < 3
      @score = @score * 0.9
    end
    
    if @car == "Volvo" && @zip.to_s[/\A58/]
      @score = @score * 1.2
    end
    
    @score.round(2)
  end

  def method_missing(method_name, *args)
    if args[0].to_s[/-/]
      first, second = args[0].split("-")
      if instance_variable_get(:"@#{method_name}") >= first.to_i && instance_variable_get(:"@#{method_name}") <= second.to_i
        @score += args[1]
      end
    else
      if instance_variable_get(:"@#{method_name}") == args[0]
        @score += args[1]
      end
    end  
  end
end

=begin
  def car(brand, points)
    if @car == brand
      @score += points
    end
  end

  def postnumber(number, points)
    if @postnr == number
      @score += points
    end
  end

  def license_age(year_range, points)
    first, second = year_range.split("-")
    if @license_age >= first.to_i && @license_age <= second.to_i
      @score += points
    end
  end

  def gender(gender, points)
    if @gender == gender
      @score += points
    end
  end
  
  def age(age_range, points)
    first, second = age_range.split("-")
    if @age >= first.to_i && @age <= second.to_i
      @score += points
    end
  end
=end
