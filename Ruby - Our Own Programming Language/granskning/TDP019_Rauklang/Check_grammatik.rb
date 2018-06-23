

pattern_done = /^<(.+)>$/
pattern_2 = /<(.+)>/
done = []
not_done = []
words = []

src = File.open "Grammatik.txt"

src.readlines.each do |line|
  words += line.split
  regex = pattern_done =~ line
  done << "#{$1}" if regex
  
end

words.each do |word|
  regex = pattern_2 =~ word
  if regex
    found = false
    word.gsub!(/[<>]/,'')
    if (done.include? word)
      found = true
    end
    #done.each do |word2|
     # if word == word2
      #  found = true
      #end
    #end
    if (found == false)
      not_done << word
    end
  end
end
puts not_done


