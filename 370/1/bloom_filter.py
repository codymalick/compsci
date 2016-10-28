#!/usr/bin/python

import sys
import math
import hashlib

def split_file(in_file):
	print(in_file)
	content = []
	with open(in_file) as f:
		content = f.read().splitlines()

	return content

# bloom3() takes a dictionary, input file, and output file, and runs the dict
# through a set of 3 hashes, flipping bits where hash lands. Then, it runs the
# input through the same set of hashes and the same array, flagging any input
# that collides on all three hashes as a bad password.
def bloom3(input, dictionary, output3):
	print("Bloom3 Starting...")
	#Pick an arbitrarily large array
	b_array_size = (len(dictionary)+1000000)
	b_array = [0] * b_array_size
	print("Array size: ", len(b_array))

	for x in dictionary:
		b_array[md5_hash(x, b_array_size)] = 1
		b_array[sha1_hash(x, b_array_size)] = 1	
		b_array[sha224_hash(x, b_array_size)] = 1

	#process input file
	print(input)
	f = open(output3, 'w')
	for x in input:
		#Password check array, if all bits are flipped, then it was flagged
		check_arr = [0,0,0]
	
		if b_array[md5_hash(x, b_array_size)] == 1:
			check_arr[0] = 1
			
		if b_array[sha1_hash(x, b_array_size)] == 1:
			check_arr[1] = 1
		
		if b_array[sha224_hash(x, b_array_size)] == 1:
			check_arr[2] = 1
		
		#print(check_arr)
		if check_arr[0] == 1 and check_arr[1] == 1 and check_arr[2] == 1:
			f.write("maybe\n")
		else:
			f.write("no\n")
	f.close()
	return

# Hex value to integer
def to_int(hex):
	return int(hex, 16)

def get_index(out, size):
	return out % size

# Hashes with md5 and returns an index
def md5_hash(str_in, arr_size):
	# load the array with hashed values
	md5out = hashlib.md5(str_in)

	# get the integer value as an index
	md5int = to_int(md5out.hexdigest())

	# get index from value modulus arr_size
	index = get_index(md5int, arr_size)
	return 	index

# sha1 hash
def sha1_hash(str_in, arr_size):
	sha1out = hashlib.sha1(str_in)
	sha1int = to_int(sha1out.hexdigest())
	index = get_index(sha1int, arr_size)
	return index

# sha224 hash
def sha224_hash(str_in, arr_size):
	sha224out = hashlib.sha224(str_in)
	sha224int = to_int(sha224out.hexdigest())
	index = get_index(sha224int, arr_size)
	return index

# sha512 hash
def sha512_hash(str_in, arr_size):
	sha512out = hashlib.sha512(str_in)
	sha512int = to_int(sha512out.hexdigest())
	index = get_index(sha512int, arr_size)
	return index


# sha384 hash
def sha384_hash(str_in, arr_size):
	sha384out = hashlib.sha384(str_in)
	sha384int = to_int(sha384out.hexdigest())
	index = get_index(sha384int, arr_size)
	return index



def bloom5(input, dictionary, output5):
	print("Bloom5 Starting...")
	#Pick an arbitrarily large array
	b_array_size = (len(dictionary)+1000000)
	b_array = [0] * b_array_size
	print("Array size: ", len(b_array))

	for x in dictionary:
		b_array[md5_hash(x, b_array_size)] = 1
		b_array[sha1_hash(x, b_array_size)] = 1	
		b_array[sha224_hash(x, b_array_size)] = 1
		b_array[sha384_hash(x, b_array_size)] = 1
		b_array[sha512_hash(x, b_array_size)] = 1


	#process input file
	print(input)
	f = open(output5, 'w')
	for x in input:
		#Password check array, if all bits are flipped, then it was flagged
		check_arr = [0,0,0,0,0]
		if b_array[md5_hash(x, b_array_size)] == 1:
			check_arr[0] = 1
		if b_array[sha1_hash(x, b_array_size)] == 1:
			check_arr[1] = 1
		if b_array[sha224_hash(x, b_array_size)] == 1:
			check_arr[2] = 1
		if b_array[sha384_hash(x, b_array_size)] == 1:
			check_arr[3] = 1
		if b_array[sha512_hash(x, b_array_size)] == 1:
			check_arr[4] = 1

		print(check_arr)
		if check_arr[0] == 1 and check_arr[1] == 1 and check_arr[2] == 1 and check_arr[3] == 1 and check_arr[4] == 1:
			f.write("maybe\n")
		else:
			f.write("no\n")
	f.close()
	return

def main():
	print("Hello!")
	print('Number of arguments:', len(sys.argv), 'arguments.')
	print('Argument List:', str(sys.argv))
	if len(sys.argv) < 8:
		print("Whoops!")
		exit(-1)
	else:
		print("It's working!")

	dictionary = split_file(sys.argv[2])
	user_in = split_file(sys.argv[4])
	output3 = sys.argv[6]
	output5 = sys.argv[7]
	bloom3(user_in, dictionary, output3)
	bloom5(user_in, dictionary, output5)
main()
