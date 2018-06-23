@@x = [{}, {a:1, b:2, c:3, d:4}]
@@x2 = [{}, {a:100, b:200, c:300, d:400}]
def test
    @@x = @@x2.map(&:clone)
    return 1000
end

print @@x
puts

@@x[1][:b] = test

print @@x
puts