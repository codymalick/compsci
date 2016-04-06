TITLE Assignment4-Fibb						(main.asm)

; Author: Cody Malick
; Course/Project ID: CS 271, Assignment 4-A
; Date Created: 8/2/2014
; Date Last Modified: 8/2/2014
; Description: Fibonacci calc, copied code from assignment 2

INCLUDE Irvine32.inc

.data

;Text to be displayed throughout the program
prog_title BYTE "Assignment4-A",0dh,0ah,0 
my_name BYTE "Cody Malick",0dh,0ah,0
term_mess BYTE "**END OF PROGRAM**",0dh,0ah,0
prompt1 BYTE "Values are stored in ebx",0dh,0ah,0
space BYTE "     ",0

;Constants
MAX = 30
BREAK = 5
UP_LIM = 46
LO_LIM = 1

;String Arrays
u_name BYTE MAX+1 DUP (?)
in1 DWORD MAX+1 DUP (?)

;Fibonacci Numbers
fib_1 DWORD 1d
fib_2 DWORD 1d

;output counter
count_1 DWORD 0d

.code

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

	mov edx, offset prompt1
	call WriteString
	
	
input:
	;GET USER DATA
	mov edx, 0
	mov in1, 7
	mov eax, 0

validate:
	;check to see if user input is between 1 and 46
	mov eax, in1
	;Upper Limit
	cmp eax, 1
	jl input
	;Lower Limit
	cmp eax, UP_LIM
	jg input
	;If input falls outside of the domain, jump to Input
	mov ecx, in1
	dec ecx

;Start of fibonacci calculations
;This is to handle the first output of "1" and begin incrementing the counter
fib:
	mov eax, fib_1
	;call WriteInt
	inc count_1
	mov edx, offset space
	call WriteString
	mov edx, 0

;Main calculation loop
calc:
	;Clear Registers
	mov eax, 0
	mov ebx, 0

	mov eax, fib_2
	;call WriteInt
		
	;spacing
	mov edx, offset space
	call WriteString
	mov edx, 0

	;Sum the last two numbers
	add eax, fib_1
	mov ebx, fib_2

	call dumpregs

	;Reassign last number to next number
	mov fib_1, ebx
	mov fib_2, eax

	;Increment counter
	inc count_1

	;Check if 5 items are in a line, start new line if it is equal to five
	cmp count_1, BREAK
	jl next
	call Crlf
	mov count_1, 0	
next:
	;Loop back to the beginning of the sequence
	loop calc		
	
;End of the program, says goodbye to the user
last:
	
	call Crlf
exit
main ENDP

END main