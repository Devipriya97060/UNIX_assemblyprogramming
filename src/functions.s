.data
.text
.global gcd
.global gcd_loop
.global gcd_return

gcd:
  cmp $rax, %rbx
  je  gcd_return

  jmp gcd_loop

gcd_loop:
  movq  %rax, %r8
  movq  %rbx, %rcx
  cqo
  idivq %rax
  movq  %rdx, %rax
  movq  %r8, %rbx

  cmp   $0, %rax
  jne   gcd_loop

  movq  %rbx, %rcx
  jmp   gcd_return

gcd_return:
  movq  %rbx, %rax
  ret

