;this program approximates the value of e

extern printf
extern scanf

SECTION .data

fmt: dq "%lf",0
p_fmt: dq "%f",10,0
prompt: db "Enter a precision value: ",0
output: db "e approximation: ",0 

empty_string: db " ",10,0

precision: dq 0.0
counter: dq 2.0
prod: dq 1.0
one: dq 1.0
two: dq 2.0
result: dq 1.0
zero: dq 0.0

e_val: dq 2.71828182845904523536 

SECTION .text

	global main
main:
	finit
	
	push prompt
	call printf
	add esp, 4

	push precision
	push fmt
	call scanf
	add esp, 8

	fld qword [two]
	fstp qword [result]

;start calculation
start_approx:

	fld qword [counter]
	fmul qword [prod]
	fstp qword [prod]

	fld1
	fdiv qword [prod]
	fadd qword [result]
	fstp qword [result]
	
	fld qword [e_val]
	fsub qword [result]
	fcomp qword [precision]
	fnstsw ax
	sahf
	jnae end_prog

	fld qword [counter]
	fadd qword [one]
	fstp qword [counter]
	jmp start_approx	
		
;if precision input = 1, then return 1+1/1 = 2
end_prog:
	
	push output
	call printf
	add esp, 4
	
	push dword [result+4]
	push dword [result]
	push p_fmt
	call printf
	add esp, 12

	push empty_string
	call printf
	add esp, 4

ret

