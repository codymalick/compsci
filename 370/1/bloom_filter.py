#!/usr/bin/python

import sys

def read_dictionary(dict_file):
	print(dict_file)
	content = []
	with open(dict_file) as f:
		content = f.read().splitlines()

	for val in content:
		print(val)
	return

def read_input():
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

	read_dictionary(sys.argv[2])

main()
