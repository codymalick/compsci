TITLE Assignment4-Sieve						(main.asm)

; Author: Cody Malick
; Course/Project ID: CS 271, Assignment 4-C
; Date Created: 8/4/2014
; Date Last Modified: 8/5/2014
; Description: Sieve

INCLUDE Irvine32.inc

.data

;Text to be displayed throughout the program
prog_title BYTE "Assignment4-C-Sieve",0dh,0ah,0 
my_name BYTE "Cody Malick",0dh,0ah,0
term_mess BYTE "**END OF PROGRAM**",0dh,0ah,0
debug BYTE "got this far",0dh,0ah,0

;Integer Variables
one DWORD 1d
ival DWORD 0d
val1 DWORD 0d
val2 DWORD 0d

;Constants
LIST_SIZE = 100000

;Number Arrays
num_list DWORD LIST_SIZE DUP (?)

.code

fill_array PROC
	push ebp
	mov ebp, esp

	;edi is starting location
	mov edi, TYPE num_list
	;ebx is size of each item
	mov ebx, TYPE num_list

	mov eax, 1
	mov ecx, LIST_SIZE

l1:
	mov [num_list+edi], eax
	add eax, 1
	add edi, ebx
	loop l1

	pop ebp
	ret
fill_array ENDP
	
find_primes PROC
	push ebp
	mov ebp, esp

	mov edi, 0
	mov eax, [num_list+edi]

check_num:
	;move again for loop
	mov eax, [num_list+edi]
	cmp eax, one
	jg case_above

case_1:
	add edi, 4
	jmp check_num

case_above:
	mov eax, [num_list+edi]
	call WriteDec
	call Crlf
	;mov values for it_prime, we don't want these back
	mov val1, edi
	pusha
	call it_prime
	popa


	mov eax, [num_list+edi]
	mov ebx, [num_list+edi]
	mul ebx
	add edi, 4
	cmp eax, LIST_SIZE


	jl check_num

print_value:

	mov eax, [num_list+edi]
	cmp eax, one
	je skip_1
	call WriteDec
	call Crlf
skip_1:
	add edi, 4

	cmp edi, LIST_SIZE*4
	jl print_value

	pop ebp
	ret
find_primes ENDP

it_prime PROC
	push ebp
	mov ebp, esp

	;initial value (2, 3, 5, etc)
	mov edi, val1

	mov ebx, edi

	;eax is location doubled, 2 = 16, 3 = 24
	mov eax, 2
	mul edi
	;mov doubled size into edi
	mov edi, eax
	mov eax, ebx
	;ebx contains 1 for setting
	mov ebx, 1
l2:
	;mov 1 into value
	mov [num_list+edi], ebx
	add edi, eax
	cmp edi, LIST_SIZE*4
	jg end_prime
	jmp l2
end_prime:
	pop ebp
	ret
it_prime ENDP


main PROC
	call Clrscr
	;This would only have a use, it seems, when there is something coming before this program is executed.

	;INTRODUCTION

	;Output name and program title
	mov edx,OFFSET my_name
	call WriteString

	mov	 edx,OFFSET prog_title
	call WriteString
	call Crlf

;part 1
;set the values of the array
p1:
	pusha 
	call fill_array
	popa
	
	pusha
	call find_primes
	popa





	call Crlf

exit
main ENDP

END main