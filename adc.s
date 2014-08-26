	.file	"adc.c"
	.section	.rodata
	.align 4
	.type	A_REG, @object
	.size	A_REG, 4
A_REG:
	.long	1
	.text
	.globl	cpu_adc
	.type	cpu_adc, @function
cpu_adc:
.LFB504:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, %ecx
	movl	%edx, %eax
	movb	%cl, -28(%rbp)
	movb	%al, -32(%rbp)
	movzbl	-28(%rbp), %edx
	movzbl	-32(%rbp), %eax
	addl	%eax, %edx
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	shrb	$4, %al
	andl	$1, %eax
	movzbl	%al, %eax
	addl	%edx, %eax
	movw	%ax, -2(%rbp)
	cmpb	$0, -28(%rbp)
	sete	%dl
	movq	-24(%rbp), %rax
	movl	%edx, %ecx
	sall	$7, %ecx
	movzbl	(%rax), %edx
	andl	$127, %edx
	orl	%ecx, %edx
	movb	%dl, (%rax)
	movq	-24(%rbp), %rax
	movzbl	(%rax), %edx
	andl	$-65, %edx
	movb	%dl, (%rax)
	movzbl	-28(%rbp), %eax
	andl	$15, %eax
	movl	%eax, %edx
	movzbl	-32(%rbp), %eax
	andl	$15, %eax
	addl	%eax, %edx
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	shrb	$4, %al
	andl	$1, %eax
	movzbl	%al, %eax
	addl	%edx, %eax
	cmpl	$15, %eax
	setg	%dl
	movq	-24(%rbp), %rax
	andl	$1, %edx
	movl	%edx, %ecx
	sall	$5, %ecx
	movzbl	(%rax), %edx
	andl	$-33, %edx
	orl	%ecx, %edx
	movb	%dl, (%rax)
	cmpw	$255, -2(%rbp)
	seta	%dl
	movq	-24(%rbp), %rax
	andl	$1, %edx
	movl	%edx, %ecx
	sall	$4, %ecx
	movzbl	(%rax), %edx
	andl	$-17, %edx
	orl	%ecx, %edx
	movb	%dl, (%rax)
	movzwl	-2(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE504:
	.size	cpu_adc, .-cpu_adc
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
