;TITLE Assignment4-Fibb						(main.asm)

; Author: Cody Malick
; Course/Project ID: CS 271, Assignment 4-A
; Date Created: 8/2/2014
; Date Last Modified: 8/2/2014
; Description: Fibonacci calc, copied code from assignment 2

%INCLUDE "Along32.asm"

SECTION .data

;Text to be displayed throughout the program
prog_title db "Assignment4-A",0dh,0ah,0 
my_name db "Cody Malick",0dh,0ah,0
term_mess db "**END OF PROGRAM**",0dh,0ah,0
prompt1 db "Values are stored in ebx, ecx is iterator",0dh,0ah,0
space db "     ",0

;Constants
MAX EQU 30
BREAK EQU 5
UP_LIM EQU 46
LO_LIM EQU 1

;String Arrays
;u_name BYTE MAX+1 DUP (?)
u_name TIMES 30 db 0
;in1 DWORD MAX+1 DUP (?)
;in1 TIMES 30 dd 0

;Fibonacci Numbers
fib_1 dd 1d
fib_2 dd 1d
in1 dd 0d

;output counter
count_1 dd 0d

SECTION .text

global main
main:
	;INTRODUCTION

	;Output name and program title
	mov edx, my_name ;offset
	call WriteString

	mov	 edx, prog_title ;offset
	call WriteString
	call Crlf

	mov edx, prompt1 ;offset
	call WriteString
	
	
input:
	;GET USER DATA
	mov edx, 0
	mov DWORD [in1], 7
	mov eax, 0

validate:
	;check to see if user input is between 1 and 46
	mov DWORD eax, in1
	;If input falls outside of the domain, jump to Input
	mov DWORD ecx, [in1]

;Start of fibonacci calculations
;This is to handle the first output of "1" and begin incrementing the counter
fib:
	mov eax, fib_1
	;call WriteInt
	inc DWORD [count_1]
	mov edx, space ;offset
	mov DWORD ebx, 1
	call DumpRegs
	dec ecx
	mov edx, 0

;Main calculation loop
calc:
	;Clear Registers
	mov DWORD eax, 0
	mov DWORD ebx, 0

	mov DWORD eax, [fib_2]
	;call WriteInt
		
	;spacing
	mov edx, space ;offset
	call WriteString
	mov DWORD edx, 0

	;Sum the last two numbers
	add DWORD eax, [fib_1]
	mov DWORD ebx, [fib_2]

	call DumpRegs

	;Reassign last number to next number
	mov DWORD [fib_1], ebx
	mov DWORD [fib_2], eax

	;Loop back to the beginning of the sequence
	loop calc		
	
;End of the program, says goodbye to the user
last:
	
	call Crlf
ret