I wrote this assignment in python on flip. It should run on any of the three
flip servers, flip1/2/3. Because python is not a compiled language, I did not
include a makefile. 

To run this assignment, simple use the following command in the working
directory:

python bloom_filter.py -d dictionary.txt -i sample_input.txt -o output3.txt output5.txt

Where:
	dictionary.txt is the dictionary to fill the bloom_filter array
	input.txt is the passwords to be checked for in the filter
	output3.txt is the output for the three hash bloom filter
	output5.txt is the output for the five hash bloom filter

Answers to Questions
----------------------
a) Here are the five hash functions I used an their output lengths:
	md5: 128 bits, cryptographic
	sha1: 160 bits, cryptographic
	sha224: 224 bits, cryptographic
	sha384: 384 bits, cryptographic
	sha512: 512 bits, cryptographic

The reason I chose these hashes is that they are all cryptographic (some are
retired and broken, sha1 and md5), and work in our use case of hashing words
and outputing a value that can be used as an index.

The size of the array is important. The bigger the array, the less false
positives we get. In order to pick an array of sufficient size, we can use the
formula provided to use in class:
	false positive probability = (1-e^(-kB/N))^k

Where:
	e is euler's number
	k is the number of hash functions
	B is the number of bad passwords
	N is the size of our array

using this, we can find the optimum size of our array by setting N to an
arbitrarily large number, say 10,000,000, and solving for our false positive
rate. In the case of 10,000,000 the rate with k=3, and B=623518, is:
	probability of false positive = .0049653

If we want an even lower false positive rate, we can further increase the size
of the array, as well as the number of hashes our program uses. With that same
array size, and five hashes instead of three:
	p(false positive) = .0013784

With an array size of 20,000,000:
	p(false positive) = .0000626


Given a false positive rate so small, an array size of 20,000,000 would work
well for our use case here. 

I used an array size of sizeOf(dictionary) + 20,000,000 to ensure that the
array could always hold more than the number of passwords in the dictionary
file.

b) My average password parsing times (which you can see in the output of the
program) are:

3 Hashes: 1.28481123183e-05 seconds
5 Hashes: 2.36829121908e-05 seconds

The filter with three hash functions performs better because it only has to
parse the password three times instead of five. It should also be noted that
the five hash function contains the larger hash functions sha384 and sha512
which could contribute to more exagerrated slowdown.

c) The probability of a false positive in my bloom filter is described by the
following equations:
	3 hash function = (1-e^(-3*623518/20623518))^3 = .0006519029
	5 hash function = (1-e^(-5*623518/20623518))^5 = .0000543526

d) As I stated in part a, you can reduce false positive rate by increasing the
number of hash functions and/or increasing the size of the array.
