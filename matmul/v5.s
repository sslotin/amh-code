	.file	"v5.cc"
	.text
	.p2align 4
	.globl	_Z5alloci
	.type	_Z5alloci, @function
_Z5alloci:
.LFB15342:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	leal	0(,%rdi,4), %r12d
	movl	$64, %edi
	movslq	%r12d, %r12
	movq	%r12, %rsi
	call	aligned_alloc@PLT
	movq	%r12, %rdx
	xorl	%esi, %esi
	movq	%rax, %rdi
	call	memset@PLT
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE15342:
	.size	_Z5alloci, .-_Z5alloci
	.p2align 4
	.globl	_Z6kernelPfPDv8_fS1_iiiii
	.type	_Z6kernelPfPDv8_fS1_iiiii, @function
_Z6kernelPfPDv8_fS1_iiiii:
.LFB15343:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	movl	%ecx, %r15d
	pushq	%r13
	pushq	%r12
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	movq	%rsi, %r13
	movq	%rdx, %r14
	pushq	%rbx
	movl	$384, %edx
	xorl	%esi, %esi
	movl	%r8d, %r12d
	andq	$-32, %rsp
	subq	$448, %rsp
	.cfi_offset 3, -56
	movl	24(%rbp), %ebx
	movq	%rdi, 8(%rsp)
	movl	%r9d, 28(%rsp)
	leaq	32(%rsp), %rdi
	movq	%fs:40, %rax
	movq	%rax, 440(%rsp)
	xorl	%eax, %eax
	call	memset@PLT
	movl	%r15d, %eax
	movl	28(%rsp), %r9d
	imull	%ebx, %eax
	movl	%eax, 20(%rsp)
	cmpl	16(%rbp), %r9d
	jge	.L5
	movl	%r9d, %r8d
	leal	(%rax,%rbx,2), %esi
	movq	8(%rsp), %r10
	cltq
	imull	%ebx, %r8d
	leal	(%rbx,%rsi), %edi
	movslq	%r9d, %r11
	vxorps	%xmm3, %xmm3, %xmm3
	leal	(%rbx,%rdi), %ecx
	addq	%rax, %r11
	movslq	%esi, %rsi
	vmovaps	%ymm3, %ymm4
	leal	(%rbx,%rcx), %edx
	movl	%ecx, 28(%rsp)
	vmovaps	%ymm3, %ymm5
	vmovaps	%ymm3, %ymm6
	movl	%r8d, %ecx
	movl	16(%rbp), %r8d
	movl	%edx, 24(%rsp)
	leaq	(%r10,%r11,4), %rdx
	addl	%r12d, %ecx
	vmovaps	%ymm3, %ymm7
	vmovaps	%ymm3, %ymm8
	vmovaps	%ymm3, %ymm9
	vmovaps	%ymm3, %ymm10
	vmovaps	%ymm3, %ymm11
	vmovaps	%ymm3, %ymm12
	vmovaps	%ymm3, %ymm13
	vmovaps	%ymm3, %ymm14
	subq	%rax, %rsi
	decl	%r8d
	subl	%r9d, %r8d
	movslq	%edi, %r9
	movslq	24(%rsp), %rdi
	addq	%r11, %r8
	subq	%rax, %r9
	leaq	4(%r10,%r8,4), %r11
	movslq	28(%rsp), %r8
	movslq	%ebx, %r10
	subq	%rax, %rdi
	subq	%rax, %r8
	.p2align 4
	.p2align 3
.L6:
	testl	%ecx, %ecx
	leal	7(%rcx), %eax
	vbroadcastss	(%rdx), %ymm2
	cmovns	%ecx, %eax
	addl	%ebx, %ecx
	sarl	$3, %eax
	cltq
	salq	$5, %rax
	vmovaps	0(%r13,%rax), %ymm1
	vmovaps	32(%r13,%rax), %ymm0
	vfmadd231ps	%ymm1, %ymm2, %ymm14
	vfmadd231ps	%ymm0, %ymm2, %ymm13
	vbroadcastss	(%rdx,%r10,4), %ymm2
	vfmadd231ps	%ymm2, %ymm1, %ymm12
	vfmadd231ps	%ymm2, %ymm0, %ymm11
	vbroadcastss	(%rdx,%rsi,4), %ymm2
	vfmadd231ps	%ymm2, %ymm1, %ymm10
	vfmadd231ps	%ymm2, %ymm0, %ymm9
	vbroadcastss	(%rdx,%r9,4), %ymm2
	vfmadd231ps	%ymm2, %ymm1, %ymm8
	vfmadd231ps	%ymm2, %ymm0, %ymm7
	vbroadcastss	(%rdx,%r8,4), %ymm2
	vfmadd231ps	%ymm2, %ymm1, %ymm6
	vfmadd231ps	%ymm2, %ymm0, %ymm5
	vbroadcastss	(%rdx,%rdi,4), %ymm2
	addq	$4, %rdx
	vfmadd231ps	%ymm2, %ymm1, %ymm4
	vfmadd231ps	%ymm2, %ymm0, %ymm3
	cmpq	%r11, %rdx
	jne	.L6
	vmovaps	%ymm14, 32(%rsp)
	vmovaps	%ymm13, 64(%rsp)
	vmovaps	%ymm12, 96(%rsp)
	vmovaps	%ymm11, 128(%rsp)
	vmovaps	%ymm10, 160(%rsp)
	vmovaps	%ymm9, 192(%rsp)
	vmovaps	%ymm8, 224(%rsp)
	vmovaps	%ymm7, 256(%rsp)
	vmovaps	%ymm6, 288(%rsp)
	vmovaps	%ymm5, 320(%rsp)
	vmovaps	%ymm4, 352(%rsp)
	vmovaps	%ymm3, 384(%rsp)
.L5:
	movl	20(%rsp), %esi
	leal	7(%rsi,%r12), %eax
	addl	%r12d, %esi
	cmovns	%esi, %eax
	sarl	$3, %eax
	cltq
	salq	$5, %rax
	leaq	(%r14,%rax), %rdx
	leaq	32(%r14,%rax), %rax
	vmovaps	(%rdx), %ymm3
	vaddps	32(%rsp), %ymm3, %ymm0
	vmovaps	%ymm0, (%rdx)
	vmovaps	(%rax), %ymm4
	leal	1(%r15), %edx
	vaddps	64(%rsp), %ymm4, %ymm0
	imull	%ebx, %edx
	vmovaps	%ymm0, (%rax)
	leal	7(%r12,%rdx), %eax
	addl	%r12d, %edx
	cmovns	%edx, %eax
	sarl	$3, %eax
	cltq
	salq	$5, %rax
	leaq	(%r14,%rax), %rdx
	leaq	32(%r14,%rax), %rax
	vmovaps	(%rdx), %ymm5
	vaddps	96(%rsp), %ymm5, %ymm0
	vmovaps	%ymm0, (%rdx)
	vmovaps	(%rax), %ymm6
	leal	2(%r15), %edx
	vaddps	128(%rsp), %ymm6, %ymm0
	imull	%ebx, %edx
	vmovaps	%ymm0, (%rax)
	leal	7(%r12,%rdx), %eax
	addl	%r12d, %edx
	cmovns	%edx, %eax
	sarl	$3, %eax
	cltq
	salq	$5, %rax
	leaq	(%r14,%rax), %rdx
	leaq	32(%r14,%rax), %rax
	vmovaps	(%rdx), %ymm7
	vaddps	160(%rsp), %ymm7, %ymm0
	vmovaps	%ymm0, (%rdx)
	vmovaps	(%rax), %ymm3
	leal	3(%r15), %edx
	vaddps	192(%rsp), %ymm3, %ymm0
	imull	%ebx, %edx
	vmovaps	%ymm0, (%rax)
	leal	7(%r12,%rdx), %eax
	addl	%r12d, %edx
	cmovns	%edx, %eax
	sarl	$3, %eax
	cltq
	salq	$5, %rax
	leaq	(%r14,%rax), %rdx
	leaq	32(%r14,%rax), %rax
	vmovaps	(%rdx), %ymm4
	vaddps	224(%rsp), %ymm4, %ymm0
	vmovaps	%ymm0, (%rdx)
	vmovaps	(%rax), %ymm5
	leal	4(%r15), %edx
	vaddps	256(%rsp), %ymm5, %ymm0
	imull	%ebx, %edx
	vmovaps	%ymm0, (%rax)
	leal	7(%r12,%rdx), %eax
	addl	%r12d, %edx
	cmovns	%edx, %eax
	addl	$5, %r15d
	sarl	$3, %eax
	imull	%r15d, %ebx
	cltq
	salq	$5, %rax
	leaq	(%r14,%rax), %rdx
	leaq	32(%r14,%rax), %rax
	vmovaps	(%rdx), %ymm6
	vaddps	288(%rsp), %ymm6, %ymm0
	vmovaps	%ymm0, (%rdx)
	vmovaps	(%rax), %ymm7
	vaddps	320(%rsp), %ymm7, %ymm0
	vmovaps	%ymm0, (%rax)
	leal	7(%r12,%rbx), %eax
	addl	%r12d, %ebx
	cmovs	%eax, %ebx
	sarl	$3, %ebx
	movslq	%ebx, %rax
	salq	$5, %rax
	leaq	(%r14,%rax), %rdx
	leaq	32(%r14,%rax), %rax
	vmovaps	(%rdx), %ymm3
	vaddps	352(%rsp), %ymm3, %ymm0
	vmovaps	%ymm0, (%rdx)
	vmovaps	(%rax), %ymm4
	vaddps	384(%rsp), %ymm4, %ymm0
	vmovaps	%ymm0, (%rax)
	movq	440(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L11
	vzeroupper
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L11:
	.cfi_restore_state
	vzeroupper
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE15343:
	.size	_Z6kernelPfPDv8_fS1_iiiii, .-_Z6kernelPfPDv8_fS1_iiiii
	.p2align 4
	.globl	_Z6matmulPKfS0_Pfi
	.type	_Z6matmulPKfS0_Pfi, @function
_Z6matmulPKfS0_Pfi:
.LFB15344:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rdi, %rbp
	movq	%rsi, %rbx
	subq	$88, %rsp
	.cfi_def_cfa_offset 144
	movq	%rdx, 64(%rsp)
	leal	5(%rcx), %edx
	movl	%ecx, 52(%rsp)
	movslq	%edx, %rax
	sarl	$31, %edx
	imulq	$715827883, %rax, %rax
	shrq	$32, %rax
	subl	%edx, %eax
	movl	%ecx, %edx
	leal	(%rax,%rax,2), %eax
	addl	$15, %edx
	leal	(%rax,%rax), %edi
	leal	30(%rcx), %eax
	cmovns	%edx, %eax
	movl	%edi, 60(%rsp)
	andl	$-16, %eax
	imull	%eax, %edi
	movl	%eax, (%rsp)
	movl	%edi, %r12d
	movl	$64, %edi
	sall	$2, %r12d
	movslq	%r12d, %r12
	movq	%r12, %rsi
	call	aligned_alloc@PLT
	movq	%r12, %rdx
	xorl	%esi, %esi
	movq	%rax, %rdi
	movq	%rax, %r13
	call	memset@PLT
	movq	%r12, %rsi
	movl	$64, %edi
	call	aligned_alloc@PLT
	movq	%r12, %rdx
	xorl	%esi, %esi
	movq	%rax, %rdi
	movq	%rax, %r14
	call	memset@PLT
	movq	%r12, %rsi
	movl	$64, %edi
	call	aligned_alloc@PLT
	xorl	%esi, %esi
	movq	%r12, %rdx
	movq	%rax, %rdi
	movq	%rax, %r15
	call	memset@PLT
	movl	52(%rsp), %esi
	testl	%esi, %esi
	jle	.L13
	movslq	(%rsp), %r9
	movq	%r13, %r11
	movq	%r14, %r10
	leal	0(,%rsi,4), %r12d
	xorl	%ecx, %ecx
	movq	%r15, 8(%rsp)
	movq	%r14, 16(%rsp)
	movq	%r13, 24(%rsp)
	movq	%rbx, %r14
	movslq	%r12d, %r12
	movl	%ecx, %r13d
	movq	%r11, %rbx
	movq	%r10, %r15
	leaq	0(,%r9,4), %r8
.L15:
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	movq	%r12, %rdx
	movq	%r9, 40(%rsp)
	movq	%r8, 32(%rsp)
	call	memcpy@PLT
	movq	%r14, %rsi
	movq	%r15, %rdi
	movq	%r12, %rdx
	incl	%r13d
	addq	%r12, %rbp
	call	memcpy@PLT
	movq	32(%rsp), %r8
	addq	%r12, %r14
	movq	40(%rsp), %r9
	addq	%r8, %rbx
	addq	%r8, %r15
	cmpl	52(%rsp), %r13d
	jne	.L15
	movl	(%rsp), %esi
	movq	8(%rsp), %r15
	movq	16(%rsp), %r14
	movq	24(%rsp), %r13
	testl	%esi, %esi
	jle	.L27
	movl	60(%rsp), %eax
	testl	%eax, %eax
	jle	.L17
.L29:
	movq	%r14, %rax
	movl	$0, 24(%rsp)
	movq	%r15, %r14
	movq	%rax, %r15
.L24:
	movl	24(%rsp), %eax
	movl	(%rsp), %edx
	movl	$0, 72(%rsp)
	addl	$64, %eax
	cmpl	%edx, %eax
	movl	%eax, 76(%rsp)
	cmovle	%eax, %edx
	movl	%edx, 16(%rsp)
.L16:
	movl	72(%rsp), %edi
	movl	60(%rsp), %edx
	movl	%edi, %eax
	movl	%edi, 56(%rsp)
	addl	$120, %edi
	cmpl	%edi, %edx
	movl	%edi, 72(%rsp)
	cmovle	%edx, %edi
	movl	%edi, 32(%rsp)
	cmpl	%eax, %edi
	jle	.L18
	movl	16(%rsp), %edx
	cmpl	%edx, 24(%rsp)
	jge	.L18
	movl	$0, 40(%rsp)
	.p2align 4
	.p2align 3
.L20:
	movl	40(%rsp), %eax
	movl	52(%rsp), %ecx
	movl	56(%rsp), %ebx
	movl	%eax, %r12d
	addl	$240, %eax
	cmpl	%eax, %ecx
	movl	%eax, 40(%rsp)
	cmovle	%ecx, %eax
	movl	%eax, 8(%rsp)
	.p2align 4
	.p2align 3
.L21:
	movl	24(%rsp), %ebp
	movl	%ebp, %eax
	movl	%r12d, %ebp
	movl	%eax, %r12d
	.p2align 4
	.p2align 3
.L19:
	movl	(%rsp), %eax
	movl	%r12d, %r8d
	movq	%r13, %rdi
	movl	%ebp, %r9d
	movl	%ebx, %ecx
	movq	%r14, %rdx
	movq	%r15, %rsi
	addl	$16, %r12d
	pushq	%rax
	.cfi_def_cfa_offset 152
	movl	16(%rsp), %eax
	pushq	%rax
	.cfi_def_cfa_offset 160
	call	_Z6kernelPfPDv8_fS1_iiiii
	popq	%rdi
	.cfi_def_cfa_offset 152
	popq	%r8
	.cfi_def_cfa_offset 144
	cmpl	16(%rsp), %r12d
	jl	.L19
	movl	%ebp, %r12d
	addl	$6, %ebx
	cmpl	%ebx, 32(%rsp)
	jg	.L21
	movl	40(%rsp), %esi
	cmpl	%esi, (%rsp)
	jg	.L20
.L18:
	movl	72(%rsp), %edx
	cmpl	%edx, 60(%rsp)
	jg	.L16
	movl	76(%rsp), %eax
	movl	%eax, 24(%rsp)
	cmpl	%eax, (%rsp)
	jg	.L24
	movl	52(%rsp), %r10d
	movq	%r15, %rax
	movq	%r14, %r15
	movslq	(%rsp), %r9
	movq	%rax, %r14
	testl	%r10d, %r10d
	jle	.L25
.L17:
	movl	52(%rsp), %eax
	leal	0(,%rax,4), %r12d
	movslq	%r12d, %r12
.L27:
	movslq	52(%rsp), %rbp
	movq	64(%rsp), %rcx
	leaq	0(,%r9,4), %rax
	movq	%r13, 8(%rsp)
	movq	%rax, (%rsp)
	xorl	%ebx, %ebx
	salq	$2, %rbp
	movq	%rbp, %r13
	movq	%r15, %rbp
.L26:
	movq	%rbp, %rsi
	movq	%rcx, %rdi
	movq	%r12, %rdx
	incl	%ebx
	call	memcpy@PLT
	addq	(%rsp), %rbp
	movq	%rax, %rcx
	addq	%r13, %rcx
	cmpl	52(%rsp), %ebx
	jl	.L26
	movq	8(%rsp), %r13
.L25:
	movq	%r13, %rdi
	call	_ZdaPv@PLT
	movq	%r14, %rdi
	call	_ZdaPv@PLT
	addq	$88, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movq	%r15, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	jmp	_ZdaPv@PLT
.L13:
	.cfi_restore_state
	movl	(%rsp), %ecx
	testl	%ecx, %ecx
	jle	.L25
	movl	60(%rsp), %edx
	testl	%edx, %edx
	jg	.L29
	jmp	.L25
	.cfi_endproc
.LFE15344:
	.size	_Z6matmulPKfS0_Pfi, .-_Z6matmulPKfS0_Pfi
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.type	_GLOBAL__sub_I__Z5alloci, @function
_GLOBAL__sub_I__Z5alloci:
.LFB16977:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	leaq	_ZStL8__ioinit(%rip), %rdi
	call	_ZNSt8ios_base4InitC1Ev@PLT
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	leaq	__dso_handle(%rip), %rdx
	leaq	_ZStL8__ioinit(%rip), %rsi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	__cxa_atexit@PLT
	.cfi_endproc
.LFE16977:
	.size	_GLOBAL__sub_I__Z5alloci, .-_GLOBAL__sub_I__Z5alloci
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__Z5alloci
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.hidden	__dso_handle
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
