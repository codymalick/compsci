TITLE Assignment1						(main.asm)

; Author: Cody Malick
; Course/Project ID: CS 271, Assignment 1
; Date Created: 6/30/2014
; Date Last Modified: 7/1/2014
; Description: This program demonstrates basic arithmetic operations in assembly as well as register manipulation.
;	Input will be added, subtracted, multiplied, and divided. The results will then be displayed. 

INCLUDE Irvine32.inc

.data
;Text to be displayed throughout the program
prog_title BYTE "Assignment1",0dh,0ah,0 
my_name BYTE "Cody Malick",0dh,0ah,0
instruct BYTE "Enter two values to be added, subtracted,",0
instruct1 BYTE "multiplied and divided from eachother:",0dh,0ah,0
out_sum BYTE "Sum: ",0
out_dif BYTE "Difference: ",0
out_prod BYTE "Product: ",0
out_quo BYTE "Quotient: ",0
out_rem BYTE "Remainder: ",0
term_mess BYTE "**END OF PROGRAM**",0dh,0ah,0
prompt1 BYTE "Value 1: ",0
prompt2 BYTE "Value 2: ",0

;input variables
val1 DWORD ?
val2 DWORD ?

;operation results
sum_result DWORD ?
dif_result DWORD ?
prod_result DWORD ?
quo_result DWORD ?
quo_remain DWORD ?

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

	;Instructions
	mov edx,OFFSET instruct
	call WriteString
	call Crlf
	mov edx,OFFSET instruct1
	call WriteString


	;GET THE DATA
	mov edx,OFFSET prompt1
	call WriteString

	;Read Value
	mov edx, 0
	call ReadInt
	;ReadInt stores the input in eax

	;store first input
	mov val1, eax

	;prompt 2
	mov edx, OFFSET prompt2
	Call WriteString

	;Read Value
	mov edx, 0
	call ReadInt
	mov val2, eax

	;clear registry
	mov eax, 0
	call Crlf

	;CALCULATE THE REQUIRED VALUES

	;Sum
	mov eax, val1
	add eax, val2
	mov sum_result, eax

	;Clear Registers
	mov eax, 0
	mov edx, 0

	;Difference
	mov eax, val1
	sub eax, val2
	mov dif_result, eax
	
	;Clear Registers
	mov eax, 0
	mov edx, 0

	;Product
	mov eax, val1
	mul val2
	mov prod_result, eax

	;Clear Registers
	mov eax, 0
	mov edx, 0
	mov ebx, 0
	;clear ebx because we need it for division

	;eax = dividend
	;ebx = divisor
	;xor edx before division
	;div remainder in edx

	mov eax, val1
	mov ebx, val2
	div ebx
	mov quo_result, eax
	mov quo_remain, edx
	
		
	;DISPLAY RESULTS
	
	;SUM
	mov edx,OFFSET out_sum
	call WriteString
	mov eax, sum_result
	call WriteInt
	call Crlf

	;Clear Registers
	mov eax, 0
	mov edx, 0

	;DIFFERENCE
	mov edx,OFFSET out_dif
	call WriteString
	mov eax, dif_result
	call WriteInt
	call Crlf

	;Clear Registers
	mov eax, 0
	mov edx, 0

	mov edx,OFFSET out_prod
	call WriteString
	mov eax, prod_result
	call WriteInt
	call Crlf
	
	;Clear Registers
	mov eax, 0
	mov edx, 0

	;Quotient
	mov edx,OFFSET out_quo
	call WriteString
	mov eax, quo_result
	call WriteInt
	call Crlf

	;Clear Registers
	mov eax, 0
	mov edx, 0

	;Remainder
	mov edx,OFFSET out_rem
	call WriteString
	mov eax, quo_remain
	call WriteInt
	call Crlf
	
	;clear registers
	mov eax, 0
	mov edx, 0


	;SAY GOODBYE

	mov edx,OFFSET term_mess
	call WriteString
	call Crlf

	exit
main ENDP

END main