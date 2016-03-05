TITLE Assignment2						(main.asm)

; Author: Cody Malick
; Course/Project ID: CS 271, Assignment 2
; Date Created: 7/6/2014
; Date Last Modified: 7/9/2014
; Description: Takes users name and asks for a number between 1 and 46 to calculate the nth fibonacci number in that domain.
;  It then says "goodbye %user" and displays an end of program message. 

INCLUDE Irvine32.inc

.data

;Text to be displayed throughout the program
prog_title BYTE "Assignment2",0dh,0ah,0 
my_name BYTE "Cody Malick",0dh,0ah,0
term_mess BYTE "**END OF PROGRAM**",0dh,0ah,0
goodbye BYTE "Goodbye ",0
name_prompt BYTE "What is your name?",0dh,0ah,0
prompt1 BYTE "How many Fibonacci numbers do you want to display?",0dh,0ah,0
prompt2 BYTE "Please choose a value between 1 and 46",0dh,0ah,0
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
	
	;Get users name
	mov edx,OFFSET name_prompt
	call WriteString
	mov edx, 0
	mov eax, 0
	mov ecx, 0
	mov edx,OFFSET u_name
	mov ecx, MAX
	call ReadString
	
	;INSTRUCTIONS
	mov edx,OFFSET prompt1
	call WriteString
input:
	;GET USER DATA
	mov edx, 0
	mov edx, offset prompt2
	call WriteString
	mov edx, 0

	mov eax, 0
	call ReadInt
	mov in1, eax
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
;If input == 1, output fib_1 and jump to the end of the program
case1:
	cmp ecx, 1
	jl fib
	jg fib
	mov eax, fib_1
	call WriteInt
	call Crlf
	jmp last

;Start of fibonacci calculations
;This is to handle the first output of "1" and begin incrementing the counter
fib:
	mov eax, fib_1
	call WriteInt
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
	call WriteInt
		
	;spacing
	mov edx, offset space
	call WriteString
	mov edx, 0

	;Sum the last two numbers
	add eax, fib_1
	mov ebx, fib_2

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
	mov edx, 0
	mov edx, offset goodbye
	call WriteString
	mov edx, 0
	mov edx, offset u_name
	call WriteString
	call Crlf
	mov edx, 0
	mov edx, offset term_mess
	call WriteString
	call Crlf
exit
main ENDP

END main