	.text
# internal switch(expr) routine
# %rsi = switch table, %rax = expr

switch:
        pushq   %rsi
        movq    %rdx, %rsi
        movq    %rax, %rbx
        cld
        lodsq
        movq    %rax, %rcx
next:
        lodsq
        movq    %rax, %rdx
        lodsq
        cmpq    %rdx, %rbx
        jnz     no
        popq    %rsi
        jmp     *%rax
no:
        loop    next
        lodsq
        popq    %rsi
        jmp     *%rax

L8:
	.byte	37
	.byte	100
	.byte	32
	.byte	37
	.byte	100
	.byte	10
	.byte	0
L9:
	.byte	37
	.byte	99
	.byte	32
	.byte	37
	.byte	99
	.byte	10
	.byte	0
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	addq	$-32,%rsp
	movq	$2, %r10
	movl	%r10d, -4(%rbp)
	movq	$3, %r10
	movl	%r10d, -8(%rbp)
	movslq	-8(%rbp), %r10
	movq	%r10, %rdx
	movslq	-4(%rbp), %r10
	movq	%r10, %rsi
	leaq	L8(%rip), %r10
	movq	%r10, %rdi
	call	printf@PLT
	movq	%rax, %r10
	movq	$102, %r10
	movb	%r10b, -12(%rbp)
	leaq	-12(%rbp), %r10
	movq	%r10, -20(%rbp)
	movq	-20(%rbp), %r10
	movzbq	(%r10), %r10
	movq	%r10, %rdx
	movzbq	-12(%rbp), %r10
	movq	%r10, %rsi
	leaq	L9(%rip), %r10
	movq	%r10, %rdi
	call	printf@PLT
	movq	%rax, %r10
	movq	$0, %r10
	movl	%r10d, %eax
	jmp	L7
L7:
	addq	$32,%rsp
	popq	%rbp
	ret
