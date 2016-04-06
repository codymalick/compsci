TITLE Assignment3						(main.asm)

; Author: Cody Malick
; Course/Project ID: CS 271, Assignment 3
; Date Created: 7/21/2014
; Date Last Modified: 7/21/2014
; Description: 

INCLUDE Irvine32.inc

.data

;Text to be displayed throughout the program
prog_title BYTE "Assignment3",0dh,0ah,0 
my_name BYTE "Cody Malick",0dh,0ah,0
term_mess BYTE "**END OF PROGRAM**",0dh,0ah,0
goodbye BYTE "Goodbye ",0
name_prompt BYTE "What is your name?",0dh,0ah,0
name_hello BYTE "Hello ",0
exclaim BYTE "!",0dh,0ah,0
prompt1 BYTE "Please enter a numbers between 0 and 100. The program will stop ",0dh,0ah,0
prompt2 BYTE "asking for input once you input a negative number. After that, the program will",0dh,0ah,0
prompt3 BYTE "output the following: # of entered values, sum of values entered, average",0dh,0ah,0
prompt4 BYTE "Enter value between 0 and 100, negative number to end loop: ",0
prompt5 BYTE "Value entered was too large!",0dh,0ah,0
number_input BYTE "Number of values entered: ",0
sum_string BYTE "Sum of values entered: ",0
av_string BYTE "Average of values entered: ",0

;Integer Variables
user_in DWORD 0d
num_in DWORD 0d
sum_in DWORD 0d
av_in DWORD 0d
checker DWORD 0d

;CONSTANTS
MAX = 30
UP_LIM = 100
LO_LIM = 0

;String Arrays
u_name BYTE MAX+1 DUP (?)

.code

;Handles the introduction section, displays instructions and prompts user for their name
intro PROC
	
	;Get users name
	mov edx,OFFSET name_prompt
	call WriteString
	mov edx, 0
	mov eax, 0
	mov ecx, 0
	mov edx,OFFSET u_name
	mov ecx, MAX
	call ReadString

	;Great User
	mov edx, offset name_hello
	call WriteString
	mov edx, offset u_name
	call WriteString
	mov edx, offset exclaim
	call WriteString

	;instructions
	mov edx, offset prompt1
	call WriteString
	mov edx, offset prompt2
	call WriteString
	mov edx, offset prompt3
	call WriteString
	call Crlf
	ret
intro ENDP


;checks whether or not a number entered is valid, and sets the checker Variable
valid PROC
	mov checker, 0
	mov eax, 0
	cmp user_in, UP_LIM
	jg big
	cmp user_in, LO_LIM
	jl small
	jmp correct
;number is too big, set checker to +1
big:
	mov checker, 1
	jmp last
;number is negative, set checker to -1
small:
	mov checker, -1
	jmp last
;number is between 0 and 100, checker is 0
correct:
	mov eax, user_in
	add sum_in, eax
	inc num_in
last:
	ret
valid ENDP

;gets users integer input
get_int PROC
	push ebp
	mov ebp, esp
	mov eax, 0
	mov edx, 0
	mov edx, offset prompt4
	call WriteString

	call ReadInt
	mov user_in, eax

	pop ebp
	ret
get_int ENDP

;prints out all the required information, average, number entered, sum of all values
end_program PROC
	push ebp
	mov ebp, esp

	;number of values entered
	mov eax, 0
	mov edx, 0
	mov eax, num_in
	mov edx, offset number_input
	call WriteString
	call WriteInt
	call Crlf

	;sum of values entered
	mov eax, 0
	mov edx, 0
	mov eax, sum_in
	mov edx, offset sum_string
	call WriteString
	call WriteInt
	call Crlf

	;average of values entered
	mov edx, 0
	mov eax, 0
	mov ebx, 0

	mov eax, sum_in
	div num_in
	mov av_in, eax

	mov edx, 0
	mov ebx, 0

	mov edx, offset av_string
	call WriteString
	call WriteInt
	call Crlf

	pop ebp
	ret
end_program ENDP



main PROC
	call Clrscr
	;This would only have a use, it seems, when there is something coming before this program is executed.
	mov edx,OFFSET my_name
	call WriteString

	mov	 edx,OFFSET prog_title
	call WriteString
	call Crlf
	
	;clear general use registers
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0

	pusha
	call intro
	popa

input:

	;clear general use registers
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	
	;get user input
	call get_int

	;check to see if it's valid and check for negative input
	call valid

	;depending on checker value, jump to apropriate label
	mov eax, checker
	cmp eax, 0
	jl end_input
	cmp eax, 0
	jg b_input
	jmp input

;if the value is over 100
b_input:
	mov edx, offset prompt5
	call WriteString
	jmp input

;if the value is negative
end_input:
	
	call Crlf
	;print out the required variables
	call end_program

	;say goodbye to the user
	mov edx, offset goodbye
	call WriteString
	mov edx, offset u_name
	call WriteString
	mov edx, offset exclaim
	call WriteString

	call Crlf

exit
main ENDP

END main