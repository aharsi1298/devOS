[org 0x1000]
[bits 32]

_start:
pushfd
pushfd
xor DWORD [esp], 0x00200000
popfd
pushfd
pop eax
xor eax, [esp]
popfd
and eax, 0x00200000
shr eax, 21
push eax

jmp next
;mov al, 0xA0
;out 0x1F6, al
;mov al, 0
;out 0x1F2, al
;out 0x1F3, al
;out 0x1F4, al
;out 0x1F5, al
;mov al, 0xEC
;out 0x1F7, al
;in 0x1F7, al
;cmp al, 0
;je error1
;loop1:
;in 0x1F7, al
;and al, 0x80
;shr 7
;cmp al, 0
;jne loop1
;in 0x1F4, al
;cmp al, 0
;jne error1
;in 0x1F5, al
;cmp al, 0
;jne error1
;loop2:
;in 0x1F7, al
;mov ah, al
;and ah, 0x8
;shr ah, 3
;cmp ah, 0
;jne next
;and al, 0x1
;cmp al, 0
;jne next
;jmp loop2
next:

;jmp setup_paging
return:
;call enable_paging

return2:
call kernel_main
pop eax
jmp $

%ifdef COMMENT
enable_paging:
mov eax, 0x1000
mov cr3, eax

mov eax, cr0
or eax, 0x80000001
mov cr0, eax
jmp return2

setup_paging:
    mov eax, 0
    mov [i], eax
    for1start:
    mov ecx, [i]
    cmp DWORD [i], 1024
    jnl for1end
    mov eax, [i]
    shl eax, 12
    add eax, 0x1101B
    mov ebx, [i]
    shl ebx, 2
    add ebx, 0x10000
    mov [ebx], eax
    mov eax, 0
    mov [j], eax
    for2start:
    mov edx, [j]
    cmp DWORD [j], 1024
    jnl for2end
    mov eax, [i]
    shl eax, 22
    mov ebx, [j]
    shl ebx, 12
    add eax, ebx
    add eax, 0x1B
    mov ebx, [j]
    shl ebx, 2
    add ebx, 0x11000
    mov ecx, [i]
    shl ecx, 12
    add ebx, ecx
    mov [ebx], eax
    inc DWORD [j]
    jmp for2start
    for2end:
    inc DWORD[i]
    jmp for1start
    for1end:
    jmp return

i:
dd 0
j:
dd 0
%endif ; Comment
kernel_main: