;this program calculates the area of a circle using user given radius. 

extern printf
extern scanf

SECTION .data
prompt: db "input user data: ",10,0
prompt2: db "Area: ",0
inf: db "%lf",0
fmt: db "%f",10,0

user_in: dd 2.0

area: dq 2.00
pi: dq 3.14159265359
radius: dq 0.0


SECTION .text

	global main
main:
	;prompt user for data
	finit
	push DWORD prompt
	call printf
	add esp, 4
	
	;collect user input
	push radius
	push inf
	call scanf
	add esp, 8
	

	fld QWORD [radius]
	fmul QWORD [radius] ;radius now stored in ST0
	fmul QWORD [pi]
	fstp QWORD [area]
	
	push DWORD prompt2
	call printf
	add esp, 4

	push DWORD [area+4]
	push DWORD [area]
	push DWORD fmt
	call printf
	add esp, 12			
	


ret
