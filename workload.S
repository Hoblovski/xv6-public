# Initial process execs /init.
# This code runs in user space.

#include "syscall.h"
#include "traps.h"

.globl start
start:
  pushl $0
  pushl $0
  pushl $0

loop:
  call summod
  pushl %eax
  pushl $0
  mov $SYS_report, %eax
  int $T_SYSCALL
  addl $8, %esp

  jmp loop


  .globl  summod
  .type summod, @function
summod:
  pushl %ebp
  movl  %esp, %ebp
  pushl %ebx
  subl  $16, %esp
  movl  $0, -12(%ebp)
  movl  $0, -8(%ebp)
  jmp .L4
.L5:
  movl  -8(%ebp), %eax
  imull -8(%ebp), %eax
  movl  %eax, %ebx
  movl  -8(%ebp), %edx
  movl  %edx, %eax
  sall  $2, %eax
  leal  (%eax,%edx), %ecx
  movl  $2021161081, %edx
  movl  %ecx, %eax
  imull %edx
  sarl  $3, %edx
  movl  %ecx, %eax
  sarl  $31, %eax
  subl  %eax, %edx
  movl  %edx, %eax
  leal  (%ebx,%eax), %ecx
  movl  $1152921497, %edx
  movl  %ecx, %eax
  imull %edx
  sarl  $28, %edx
  movl  %ecx, %eax
  sarl  $31, %eax
  subl  %eax, %edx
  movl  %edx, %eax
  imull $1000000007, %eax, %eax
  subl  %eax, %ecx
  movl  %ecx, %eax
  addl  %eax, -12(%ebp)
  addl  $1, -8(%ebp)
.L4:
  cmpl  $7999999, -8(%ebp)
  #cmpl  $999, -8(%ebp)
  jle .L5
  cmpl  $-1077224173, -12(%ebp)
  setne %al
  movzbl  %al, %eax
  addl  $16, %esp
  popl  %ebx
  popl  %ebp
  ret
