class Key_Words
  def Key_Words.load(filename)
    @db = Hash.new
    File.readlines(filename).each do |line|
      #Regex to fetch word inside quotation mark  
      regex = /"(.*?)"/
      #Set DSL word to key and corresponding Ruby word to value
      @db[line.scan(regex)[0][0]] = line.scan(regex)[1][0]
    end
    return @db
  end
end