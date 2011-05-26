import sys
import random

	
if len(sys.argv) < 3:
	print "Please provide matrix size argument"
	exit(-1)
	
matrix_len = int(sys.argv[1])
routes_to_generate = int(sys.argv[2])

matrix = [[0 for j in xrange(matrix_len)] for i in xrange(matrix_len)]

max_nr_of_routes = matrix_len*(matrix_len-1)


if routes_to_generate > max_nr_of_routes:
	print "Two many routes to generate. Increase number of cities."
	print "With",matrix_len,"cities, you can generate",max_nr_of_routes,\
	"routes."
	exit(-2)
if routes_to_generate < matrix_len:
	print "Two little routes to generate."
	print "Number of routes must be at lease of cities number size."
	exit(-3)



for i in xrange(matrix_len):
	matrix[i][(i+1)%matrix_len] = random.randint(1,9)
	
left_routes_to_gen = routes_to_generate - matrix_len


while(True):
	if left_routes_to_gen == 0:
		break
	fromCity = random.randint(0,matrix_len-1)
	toCity   = random.randint(0,matrix_len-1)
	if fromCity == toCity:
		continue
	if matrix[fromCity][toCity] == 0:
		matrix[fromCity][toCity] = random.randint(1,10)	
		left_routes_to_gen-=1

output = ""	
for i in xrange(matrix_len):
	for j in xrange(matrix_len):
		output+=str(matrix[i][j]) + ' '
	output = output[:-1]
	output+="\n"	

print output
