	.file	"benchmark.cc"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"stof"
.LC4:
	.string	"%.6f x %d\n"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC5:
	.string	"basic_string::_M_construct null not valid"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB6:
	.section	.text.startup,"ax",@progbits
.LHOTB6:
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB10296:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA10296
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
	subq	$88, %rsp
	.cfi_def_cfa_offset 144
	movq	%fs:40, %rax
	movq	%rax, 72(%rsp)
	xorl	%eax, %eax
	cmpl	$1, %edi
	jg	.L33
	movl	.LC0(%rip), %ebp
	movl	$256, %r13d
.L2:
	movl	%r13d, %ebx
	imull	%r13d, %ebx
	movslq	%ebx, %rbx
	salq	$2, %rbx
	movq	%rbx, %rdi
.LEHB0:
	call	_Znam@PLT
	movq	%rbx, %rdi
	movq	%rax, %r15
	call	_Znam@PLT
	movq	%rbx, %rdi
	movq	%rax, %r14
	call	_Znam@PLT
	movq	%rbx, %rdx
	xorl	%esi, %esi
	movq	%rax, %rdi
	movq	%rax, %r12
	call	memset@PLT
	call	clock@PLT
	vmovd	%ebp, %xmm6
	xorl	%ebp, %ebp
	vmulss	.LC2(%rip), %xmm6, %xmm5
	movq	%rax, %rbx
	vmovss	%xmm5, 12(%rsp)
	jmp	.L19
	.p2align 4
	.p2align 3
.L34:
	movl	%r13d, %ecx
	movq	%r12, %rdx
	movq	%r14, %rsi
	movq	%r15, %rdi
	call	_Z6matmulPKfS0_Pfi@PLT
	incl	%ebp
.L19:
	call	clock@PLT
	vmovss	12(%rsp), %xmm3
	vxorps	%xmm2, %xmm2, %xmm2
	subq	%rbx, %rax
	vcvtsi2ssq	%rax, %xmm2, %xmm0
	vcomiss	%xmm0, %xmm3
	ja	.L34
	call	clock@PLT
	vxorps	%xmm4, %xmm4, %xmm4
	movl	%ebp, %esi
	leaq	.LC4(%rip), %rdi
	subq	%rbx, %rax
	vcvtsi2ssq	%rax, %xmm4, %xmm0
	vmulss	.LC3(%rip), %xmm0, %xmm1
	vcvtsi2ssl	%ebp, %xmm4, %xmm0
	movl	$1, %eax
	vdivss	%xmm0, %xmm1, %xmm0
	vcvtss2sd	%xmm0, %xmm0, %xmm0
	call	printf@PLT
	movq	72(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L35
	addq	$88, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
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
	ret
.L33:
	.cfi_restore_state
	movl	%edi, %ebx
	movq	8(%rsi), %rdi
	movq	%rsi, %rbp
	movl	$10, %edx
	xorl	%esi, %esi
	call	strtol@PLT
	movl	%eax, %r13d
	cmpl	$2, %ebx
	je	.L22
	movq	16(%rbp), %rbx
	leaq	48(%rsp), %r12
	leaq	32(%rsp), %r15
	movq	%r12, 32(%rsp)
	testq	%rbx, %rbx
	je	.L3
	movq	%rbx, %rdi
	call	strlen@PLT
	movq	%rax, %r14
	movq	%rax, 24(%rsp)
	cmpq	$15, %rax
	ja	.L36
	cmpq	$1, %rax
	jne	.L6
	movzbl	(%rbx), %eax
	leaq	24(%rsp), %rbp
	movb	%al, 48(%rsp)
.L7:
	movq	24(%rsp), %rax
	movq	32(%rsp), %rdx
	movq	%rax, 40(%rsp)
	movb	$0, (%rdx,%rax)
	movq	32(%rsp), %r14
	call	__errno_location@PLT
	movq	%rbp, %rsi
	movl	(%rax), %r15d
	movl	$0, (%rax)
	movq	%rax, %rbx
	movq	%r14, %rdi
	call	strtof@PLT
	vmovd	%xmm0, %ebp
	cmpq	24(%rsp), %r14
	je	.L37
	movl	(%rbx), %eax
	cmpl	$34, %eax
	je	.L38
	testl	%eax, %eax
	jne	.L10
	movl	%r15d, (%rbx)
.L10:
	movq	32(%rsp), %rdi
	cmpq	%r12, %rdi
	je	.L2
	movq	48(%rsp), %rax
	leaq	1(%rax), %rsi
	call	_ZdlPvm@PLT
	jmp	.L2
.L6:
	leaq	24(%rsp), %rbp
	testq	%rax, %rax
	je	.L7
	movq	%r12, %rdi
	jmp	.L5
.L36:
	leaq	24(%rsp), %rbp
	movq	%r15, %rdi
	xorl	%edx, %edx
	movq	%rbp, %rsi
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm@PLT
	movq	%rax, %rdi
	movq	%rax, 32(%rsp)
	movq	24(%rsp), %rax
	movq	%rax, 48(%rsp)
.L5:
	movq	%r14, %rdx
	movq	%rbx, %rsi
	call	memcpy@PLT
	jmp	.L7
.L3:
	leaq	.LC5(%rip), %rdi
	call	_ZSt19__throw_logic_errorPKc@PLT
.LEHE0:
.L22:
	movl	.LC0(%rip), %ebp
	jmp	.L2
.L37:
	leaq	.LC1(%rip), %rdi
.LEHB1:
	call	_ZSt24__throw_invalid_argumentPKc@PLT
.L35:
	call	__stack_chk_fail@PLT
.L38:
	leaq	.LC1(%rip), %rdi
	call	_ZSt20__throw_out_of_rangePKc@PLT
.LEHE1:
.L23:
	movq	%rax, %rbp
	jmp	.L13
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA10296:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE10296-.LLSDACSB10296
.LLSDACSB10296:
	.uleb128 .LEHB0-.LFB10296
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB10296
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L23-.LFB10296
	.uleb128 0
.LLSDACSE10296:
	.section	.text.startup
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC10296
	.type	main.cold, @function
main.cold:
.LFSB10296:
.L13:
	.cfi_def_cfa_offset 144
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	cmpl	$0, (%rbx)
	jne	.L14
	movl	%r15d, (%rbx)
.L14:
	movq	32(%rsp), %rdi
	cmpq	%r12, %rdi
	je	.L30
	movq	48(%rsp), %rax
	leaq	1(%rax), %rsi
	vzeroupper
	call	_ZdlPvm@PLT
.L16:
	movq	%rbp, %rdi
.LEHB2:
	call	_Unwind_Resume@PLT
.LEHE2:
.L30:
	vzeroupper
	jmp	.L16
	.cfi_endproc
.LFE10296:
	.section	.gcc_except_table
.LLSDAC10296:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC10296-.LLSDACSBC10296
.LLSDACSBC10296:
	.uleb128 .LEHB2-.LCOLDB6
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
.LLSDACSEC10296:
	.section	.text.unlikely
	.section	.text.startup
	.size	main, .-main
	.section	.text.unlikely
	.size	main.cold, .-main.cold
.LCOLDE6:
	.section	.text.startup
.LHOTE6:
	.p2align 4
	.type	_GLOBAL__sub_I_main, @function
_GLOBAL__sub_I_main:
.LFB11928:
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
.LFE11928:
	.size	_GLOBAL__sub_I_main, .-_GLOBAL__sub_I_main
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_main
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	1065353216
	.align 4
.LC2:
	.long	1232348160
	.align 4
.LC3:
	.long	897988541
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.hidden	__dso_handle
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
