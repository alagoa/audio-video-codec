def to_unary(a):
	unary = 0
	for i in range(1, a+1):
		unary = unary + (1 << i)
	return unary

print (bin(to_unary(5)))