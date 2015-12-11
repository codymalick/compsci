;program calculates the result of the quadratic equation by taking in vaules
;for a, b, and c

extern printf
extern scanf
extern sqrt

SECTION .data

fmt: db "%f",0
d_fmt: db "%lf",0
s_fmt: db "%s",0
below_zero: db "Square root of (b^2-4ac) < 0",10,0
prompt_a: db "Enter a value for 'a'", 10, 0
prompt_b: db "Enter a value for 'b'", 10, 0
prompt_c: db "Enter a value for 'c'", 10, 0
x_equals: db "X = ",0
empty_string: db " ", 10, 0
;user_in: dq 0.0
a: dq 0.0
b: dq 6.0
c: dq 6.0
b2: dq 0.0
below_root: dq 0.0
four: dq 4.0
two: dq 2.0
two_a: dq 0.0
n_one: dq -1.0
n_b: dq 0.0


result_1: dq 0
result_2: dq 0

SECTION .text

	global main
main:
	finit

	;prompt for value of a
	push prompt_a
	call printf
	add esp, 4

	;get user data
	push a
	push d_fmt
	call scanf
	add esp, 8

	;prompt for b
	push prompt_b
	call printf
	add esp, 4

	;get value for b
	push b
	push d_fmt
	call scanf
	add esp, 8

	;prompt c
	push prompt_c
	call printf
	add esp, 4
	
	;get c value
	push c
	push d_fmt
	call scanf
	add esp, 8

	;calculate b^2
	fld qword [b]
	fmul qword [b]
	fstp qword [b2]
	
	;calculate 2a
	fld qword [a]
	fmul qword [two]
	fstp qword [two_a]

	;calculate 4ac
	fld qword [a]
	fmul qword [c]
	fmul qword [four]
	fstp qword [below_root]

	;subtract 4ac from b^2
	fld qword [b2]
	fsub qword [below_root]
	fstp qword [below_root]

	;check if the value under the radical is negative
	fld qword [below_root]
	fcomp qword [result_1]
	fnstsw ax
	sahf
	jb less_zero


	;square root the function
	fld qword [below_root]
	fsqrt
	fstp qword [below_root]

	;calculate -b
	fld qword [n_one]	
	fmul qword [b]
	fstp qword [n_b]

	;subtract square root from -b
	fld qword [n_b]
	fsub qword [below_root]
	fstp qword [result_1]	

	;divide by 2a
	fld qword [result_1]
	fdiv qword [two_a]
	fstp qword [result_1]

	;print first result
	push x_equals
	call printf
	add esp, 4

	push dword [result_1+4]
	push dword [result_1]
	push dword fmt
	call printf
	add esp, 12

	;break line for neatness
	push empty_string
	call printf
	add esp, 4

	;add square root to -b
	fld qword [n_b]
	fadd qword [below_root]
	fstp qword [result_2]
	
	;divide by 2a
	fld qword [result_2]
	fdiv qword [two_a]
	fstp qword [result_2]

	;print second result
	push x_equals
	call printf
	add esp, 4	

	push dword [result_2+4]
	push dword [result_2]
	push dword fmt
	call printf
	add esp, 12

	push empty_string
	call printf
	add esp, 4


	jmp end_l

;case sqrt is an imaginary number
less_zero:
	push dword below_zero
	push s_fmt
	call printf
	add esp, 8
end_l:
ret
