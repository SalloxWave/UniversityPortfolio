#uppgift 10
def username(s)
	/^[a-zA-Z]+: \K[a-zA-Z]+/ =~ s
	return $&
end

#uppgift 12
def regnr(s)
	/[^a-z\d\W_IQV]{3}\d{3}/ =~ s
	unless $&.nil?
		return $&
	else
		return false
	end
end

