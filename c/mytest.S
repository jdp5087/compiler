.section	.bss

.section	.data

.section	.text

.globl	_start

_start:
	movl	$5,%eax
	push	%eax
	movl	$9,%eax
	push	%eax
	movl	$8,%eax
	push	%eax
	movl	$2,%eax
	xor	%edx,%edx
	movl	%eax,%ebx
	movl	(%esp),%eax
	addl	$4,%esp
	idivl	%ebx
	imul	(%esp),%eax
	addl	$4,%esp
	addl	(%esp),%eax
	addl	$4,%esp
	movl	%eax, %ebx
	movl	$1, %eax
	int	$0x80
