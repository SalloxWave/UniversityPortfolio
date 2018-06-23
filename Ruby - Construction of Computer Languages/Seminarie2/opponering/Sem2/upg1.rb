# /(?<=\d\. )([a-zA-Z_]*)\s*(\d*)\s*(\d*)\s*(\d*)\s*(\d*)\s*(\d*)\s*-\s*(\d*)\s*(\d*)$/


class FootballReader

  attr_reader :db

  def initialize(file)
    @db = Hash.new
    content = File.open(file,"rb").read[/(?<=<pre>)(.*)(?=<\/pre>)/m]
    content.each_line do |line|
        if line =~ /(?<=\d. )(.*)*$/
            $&[/(\S*)\s*(\d*)\s*(\d*)\s*(\d*)\s*(\d*)\s*(\d*)\s*-\s*(\d*)\s*(\d*)/]
            @db[$1] = ($6.to_i - $7.to_i).abs
        end
    end
  end

  def least_diff_goals
      @db.min_by{|k,v| v}[0]
  end

  def sorted_diff_goals
      @db.sort {|a,b| b[1]<=>a[1]}
  end
end


class WeatherReader

  attr_reader :db

  def initialize(file)
    @db = Hash.new
    content = File.open(file,"rb").read[/(?<=<pre>)(.*)(?=<\/pre>)/m]
    content.each_line do |line|
        line[/^\s*([0-9*]+)\s*([0-9*]+)\s*([0-9*]+)/]
        if $1 != nil
            @db[$1] = ($2.to_i - $3.to_i).abs
        end
    end
  end

  def least_diff_temperature
      @db.min_by{|k,v| v}[0]
  end

  def sorted_diff_temperature
      @db.sort {|a,b| b[1]<=>a[1]}
  end

end
