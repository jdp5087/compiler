.data
var1:	
	.int	64
var2:	
	.int	1
var3:
	.ascii "word\n"

	.text
.globl _start

_start:
	movl	$var1,%ecx
	addl	$var2,%ecx
	movl	$4,%eax
	movl	$1,%ebx
	movl	$3,%edx
	int	$0x80
	movl 	$1,%eax
	movl 	$0,%ebx
	int	$0x80
	
