; 2D polynomials calculator
; calculates the delta, x1, x2
; will fail to provide a good result for x most of the time because we don't have float
; simple showcase of added neg, sqrt functions.

print_a4:
push :
push 1
call put
push  
push 1
call put
push a4
push 2
call put
push \n
push 1
call put
ret

main:
mov a8, 1   ;x1

;ax² + bx + c
mov a1, 2   ;a
mov a2, 5   ;b
mov a3, -3  ;c

;delta
;b² - 4ac
push a1
push a2
mul a2, a2  ;b²
mul a1, a3  ;a*c
mul a1, 4   ;4ac
sub a2, a1  ;b² - 4ac
mov a4, a2  ;a4 is delta
pop a1
pop a2

push d
push 1
call put
jmp print_a4

cmp a4, 0
jnb 1       ;if delta < 0, we exit
end

calculate_x:
;(-b-sqrt(delta))/(2ab)

; -b
push a2
neg a2

; a5 will be 2ab
mov a5, 2
mul a5, a1
mul a5, a2

; sqrt(delta)
push a4
sqrt a4

;-b+-sqrt(delta)
cmp a8, 1   ;x1 ?
je 2
add a2, a4  ;x2
jmp 1
sub a2, a4  ;x1

;a2/2ab
div a2, a5

push x
push 1
call put
push a8
push 2
call put
push a4
mov a4, a2
jmp print_a4
pop a4

pop a2
pop a4

add a8, 1
cmp a8, 2
je calculate_x

end