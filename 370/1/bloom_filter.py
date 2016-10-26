#!/usr/bin/python

import sys

def split_file(in_file):
	print(in_file)
	content = []
	with open(in_file) as f:
		content = f.read().splitlines()

	#for val in content:
	#	print(val)
	return content

def bloom3():
	pass

def bloom5():
	pass

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

main()
