print:
  pusha
  mov ah, 0x0e
  _printStart:
  cmp byte [bx], 0
  je _printEnd
  mov al, [bx]
  int 0x10
  inc bx
  jmp _printStart
  _printEnd:
  popa
  ret

print_nl:
  push ax
  mov ah, 0xe
  mov al, 0xa
  int 0x10
  mov al, 0xd
  int 0x10
  pop ax
  ret