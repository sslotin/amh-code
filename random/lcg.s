	.file	"lcg.cc"
# GNU C++17 (GCC) version 10.2.0 (x86_64-pc-linux-gnu)
#	compiled by GNU C version 10.2.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.21-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed:  -D_GNU_SOURCE lcg.cc -march=znver2 -mmmx -mno-3dnow
# -msse -msse2 -msse3 -mssse3 -msse4a -mcx16 -msahf -mmovbe -maes -msha
# -mpclmul -mpopcnt -mabm -mno-lwp -mfma -mno-fma4 -mno-xop -mbmi -mno-sgx
# -mbmi2 -mno-pconfig -mwbnoinvd -mno-tbm -mavx -mavx2 -msse4.2 -msse4.1
# -mlzcnt -mno-rtm -mno-hle -mrdrnd -mf16c -mfsgsbase -mrdseed -mprfchw
# -madx -mfxsr -mxsave -mxsaveopt -mno-avx512f -mno-avx512er -mno-avx512cd
# -mno-avx512pf -mno-prefetchwt1 -mclflushopt -mxsavec -mxsaves
# -mno-avx512dq -mno-avx512bw -mno-avx512vl -mno-avx512ifma -mno-avx512vbmi
# -mno-avx5124fmaps -mno-avx5124vnniw -mclwb -mmwaitx -mclzero -mno-pku
# -mrdpid -mno-gfni -mno-shstk -mno-avx512vbmi2 -mno-avx512vnni -mno-vaes
# -mno-vpclmulqdq -mno-avx512bitalg -mno-avx512vpopcntdq -mno-movdiri
# -mno-movdir64b -mno-waitpkg -mno-cldemote -mno-ptwrite -mno-avx512bf16
# -mno-enqcmd -mno-avx512vp2intersect --param=l1-cache-size=32
# --param=l1-cache-line-size=64 --param=l2-cache-size=512 -mtune=znver2
# -auxbase-strip lcg.s -O3 -Wall -std=c++17 -fverbose-asm
# options enabled:  -fPIC -fPIE -faggressive-loop-optimizations
# -falign-functions -falign-jumps -falign-labels -falign-loops
# -fallocation-dce -fasynchronous-unwind-tables -fauto-inc-dec
# -fbranch-count-reg -fcaller-saves -fcode-hoisting
# -fcombine-stack-adjustments -fcompare-elim -fcprop-registers
# -fcrossjumping -fcse-follow-jumps -fdefer-pop
# -fdelete-null-pointer-checks -fdevirtualize -fdevirtualize-speculatively
# -fdwarf2-cfi-asm -fearly-inlining -feliminate-unused-debug-symbols
# -feliminate-unused-debug-types -fexceptions -fexpensive-optimizations
# -ffinite-loops -fforward-propagate -ffp-int-builtin-inexact
# -ffunction-cse -fgcse -fgcse-after-reload -fgcse-lm -fgnu-unique
# -fguess-branch-probability -fhoist-adjacent-loads -fident -fif-conversion
# -fif-conversion2 -findirect-inlining -finline -finline-atomics
# -finline-functions -finline-functions-called-once
# -finline-small-functions -fipa-bit-cp -fipa-cp -fipa-cp-clone -fipa-icf
# -fipa-icf-functions -fipa-icf-variables -fipa-profile -fipa-pure-const
# -fipa-ra -fipa-reference -fipa-reference-addressable -fipa-sra
# -fipa-stack-alignment -fipa-vrp -fira-hoist-pressure
# -fira-share-save-slots -fira-share-spill-slots
# -fisolate-erroneous-paths-dereference -fivopts -fkeep-static-consts
# -fleading-underscore -flifetime-dse -floop-interchange
# -floop-unroll-and-jam -flra-remat -fmath-errno -fmerge-constants
# -fmerge-debug-strings -fmove-loop-invariants -fomit-frame-pointer
# -foptimize-sibling-calls -foptimize-strlen -fpartial-inlining
# -fpeel-loops -fpeephole -fpeephole2 -fplt -fpredictive-commoning
# -fprefetch-loop-arrays -free -freg-struct-return -freorder-blocks
# -freorder-blocks-and-partition -freorder-functions -frerun-cse-after-loop
# -fsched-critical-path-heuristic -fsched-dep-count-heuristic
# -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
# -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
# -fsched-stalled-insns-dep -fschedule-fusion -fschedule-insns2
# -fsemantic-interposition -fshow-column -fshrink-wrap
# -fshrink-wrap-separate -fsigned-zeros -fsplit-ivs-in-unroller
# -fsplit-loops -fsplit-paths -fsplit-wide-types -fssa-backprop
# -fssa-phiopt -fstack-protector-strong -fstdarg-opt -fstore-merging
# -fstrict-aliasing -fstrict-volatile-bitfields -fsync-libcalls
# -fthread-jumps -ftoplevel-reorder -ftrapping-math -ftree-bit-ccp
# -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-coalesce-vars
# -ftree-copy-prop -ftree-cselim -ftree-dce -ftree-dominator-opts
# -ftree-dse -ftree-forwprop -ftree-fre -ftree-loop-distribute-patterns
# -ftree-loop-distribution -ftree-loop-if-convert -ftree-loop-im
# -ftree-loop-ivcanon -ftree-loop-optimize -ftree-loop-vectorize
# -ftree-parallelize-loops= -ftree-partial-pre -ftree-phiprop -ftree-pre
# -ftree-pta -ftree-reassoc -ftree-scev-cprop -ftree-sink
# -ftree-slp-vectorize -ftree-slsr -ftree-sra -ftree-switch-conversion
# -ftree-tail-merge -ftree-ter -ftree-vrp -funit-at-a-time -funswitch-loops
# -funwind-tables -fverbose-asm -fversion-loops-for-strides
# -fzero-initialized-in-bss -m128bit-long-double -m64 -m80387 -mabm -madx
# -maes -malign-stringops -mavx -mavx2 -mbmi -mbmi2 -mclflushopt -mclwb
# -mclzero -mcx16 -mf16c -mfancy-math-387 -mfma -mfp-ret-in-387 -mfsgsbase
# -mfxsr -mglibc -mieee-fp -mlong-double-80 -mlzcnt -mmmx -mmovbe -mmwaitx
# -mpclmul -mpopcnt -mprfchw -mpush-args -mrdpid -mrdrnd -mrdseed
# -mred-zone -msahf -msha -msse -msse2 -msse3 -msse4 -msse4.1 -msse4.2
# -msse4a -mssse3 -mstv -mtls-direct-seg-refs -mvzeroupper -mwbnoinvd
# -mxsave -mxsavec -mxsaveopt -mxsaves

	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d-"
.LC1:
	.string	"%d\n"
.LC3:
	.string	"%.4f s total time\n"
.LC5:
	.string	"%.2f ns per value\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB10296:
	.cfi_startproc
	pushq	%r13	#
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12	#
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp	#
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx	#
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movl	$1000000, %ebp	#, iftmp.0_13
	subq	$24, %rsp	#,
	.cfi_def_cfa_offset 64
# random.hh:8:     int n = (int) (argc > 1 ? atof(argv[1]) : 1e6);
	cmpl	$1, %edi	#, tmp169
	jle	.L2	#,
# /usr/include/bits/stdlib-float.h:27:   return strtod (__nptr, (char **) NULL);
	movq	8(%rsi), %rdi	# MEM[(char * *)argv_17(D) + 8B], MEM[(char * *)argv_17(D) + 8B]
	xorl	%esi, %esi	#
	call	strtod@PLT	#
# random.hh:8:     int n = (int) (argc > 1 ? atof(argv[1]) : 1e6);
	vcvttsd2sil	%xmm0, %ebp	# tmp171, iftmp.0_13
.L2:
# random.hh:8:     int n = (int) (argc > 1 ? atof(argv[1]) : 1e6);
	movl	$10, %ebx	#, ivtmp_88
	xorl	%esi, %esi	# pretmp_76
# lcg.cc:12:     return state % (r - l) + l;
	movl	$3435973837, %r12d	#, tmp119
	jmp	.L4	#
	.p2align 4
	.p2align 3
.L13:
# lcg.cc:11:     state = ((state * 1103515245U) + 12345U) & 0x7fffffff;
	movl	state(%rip), %esi	# state, pretmp_76
.L4:
# lcg.cc:11:     state = ((state * 1103515245U) + 12345U) & 0x7fffffff;
	imull	$1103515245, %esi, %esi	#, pretmp_76, tmp114
# random.hh:15:         printf("%d-", rng(0, 10));
	leaq	.LC0(%rip), %rdi	#,
# lcg.cc:11:     state = ((state * 1103515245U) + 12345U) & 0x7fffffff;
	addl	$12345, %esi	#, tmp115
# lcg.cc:11:     state = ((state * 1103515245U) + 12345U) & 0x7fffffff;
	andl	$2147483647, %esi	#, _37
# lcg.cc:12:     return state % (r - l) + l;
	movl	%esi, %eax	# _37, _37
# lcg.cc:11:     state = ((state * 1103515245U) + 12345U) & 0x7fffffff;
	movl	%esi, state(%rip)	# _37, state
# lcg.cc:12:     return state % (r - l) + l;
	imulq	%r12, %rax	# tmp119, tmp118
	shrq	$35, %rax	#, tmp116
	leal	(%rax,%rax,4), %eax	#, tmp123
	addl	%eax, %eax	# tmp124
	subl	%eax, %esi	# tmp124, tmp125
# random.hh:15:         printf("%d-", rng(0, 10));
	xorl	%eax, %eax	#
	call	printf@PLT	#
# random.hh:14:     for (int i = 0; i < 10; i++)
	decl	%ebx	# ivtmp_88
	jne	.L13	#,
# random.hh:17:     clock_t start = clock();
	call	clock@PLT	#
	movq	%rax, %rbx	# tmp172, start
# random.hh:19:     for (int i = 0; i < n; i++)
	testl	%ebp, %ebp	# iftmp.0_13
	jle	.L8	#,
	movl	state(%rip), %eax	# state, state_lsm.12
# random.hh:19:     for (int i = 0; i < n; i++)
	xorl	%ecx, %ecx	# i
# random.hh:10:     int checksum = 0;
	xorl	%r12d, %r12d	# checksum
# lcg.cc:12:     return state % (r - l) + l;
	movl	$3435973837, %edi	#, tmp142
	.p2align 4
	.p2align 3
.L6:
# lcg.cc:11:     state = ((state * 1103515245U) + 12345U) & 0x7fffffff;
	imull	$1103515245, %eax, %eax	#, state_lsm.12, tmp137
# random.hh:19:     for (int i = 0; i < n; i++)
	incl	%ecx	# i
# lcg.cc:11:     state = ((state * 1103515245U) + 12345U) & 0x7fffffff;
	addl	$12345, %eax	#, tmp138
# lcg.cc:11:     state = ((state * 1103515245U) + 12345U) & 0x7fffffff;
	andl	$2147483647, %eax	#, state_lsm.12
# lcg.cc:12:     return state % (r - l) + l;
	movl	%eax, %edx	# state_lsm.12, state_lsm.12
	movl	%eax, %esi	# state_lsm.12, tmp148
	imulq	%rdi, %rdx	# tmp142, tmp141
	shrq	$35, %rdx	#, tmp139
	leal	(%rdx,%rdx,4), %edx	#, tmp146
	addl	%edx, %edx	# tmp147
	subl	%edx, %esi	# tmp147, tmp148
# random.hh:20:         checksum ^= rng(0, 10);
	xorl	%esi, %r12d	# tmp148, checksum
# random.hh:19:     for (int i = 0; i < n; i++)
	cmpl	%ecx, %ebp	# i, iftmp.0_13
	jne	.L6	#,
	movl	%eax, state(%rip)	# state_lsm.12, state
.L5:
# random.hh:22:     float seconds = float(clock() - start) / CLOCKS_PER_SEC;
	call	clock@PLT	#
# random.hh:24:     printf("%d\n", checksum);
	movl	%r12d, %esi	# checksum,
	leaq	.LC1(%rip), %rdi	#,
# random.hh:22:     float seconds = float(clock() - start) / CLOCKS_PER_SEC;
	movq	%rax, %r13	# tmp173, _2
# random.hh:24:     printf("%d\n", checksum);
	xorl	%eax, %eax	#
	call	printf@PLT	#
# random.hh:22:     float seconds = float(clock() - start) / CLOCKS_PER_SEC;
	subq	%rbx, %r13	# start, tmp160
# random.hh:22:     float seconds = float(clock() - start) / CLOCKS_PER_SEC;
	vxorps	%xmm2, %xmm2, %xmm2	# tmp178
# random.hh:25:     printf("%.4f s total time\n", seconds);
	leaq	.LC3(%rip), %rdi	#,
# random.hh:22:     float seconds = float(clock() - start) / CLOCKS_PER_SEC;
	vcvtsi2ssq	%r13, %xmm2, %xmm0	# tmp160, tmp178, tmp175
# random.hh:22:     float seconds = float(clock() - start) / CLOCKS_PER_SEC;
	vdivss	.LC2(%rip), %xmm0, %xmm0	#, tmp161, seconds
# random.hh:25:     printf("%.4f s total time\n", seconds);
	movl	$1, %eax	#,
	vcvtss2sd	%xmm0, %xmm0, %xmm0	# seconds, _5
	vmovsd	%xmm0, 8(%rsp)	# _5, %sfp
	call	printf@PLT	#
# random.hh:26:     printf("%.2f ns per value\n", 1e9 * seconds / n);
	vmovsd	8(%rsp), %xmm0	# %sfp, _5
# random.hh:26:     printf("%.2f ns per value\n", 1e9 * seconds / n);
	vxorpd	%xmm3, %xmm3, %xmm3	# tmp179
# random.hh:26:     printf("%.2f ns per value\n", 1e9 * seconds / n);
	vmulsd	.LC4(%rip), %xmm0, %xmm0	#, _5, tmp164
# random.hh:26:     printf("%.2f ns per value\n", 1e9 * seconds / n);
	vcvtsi2sdl	%ebp, %xmm3, %xmm1	# iftmp.0_13, tmp179, tmp176
	leaq	.LC5(%rip), %rdi	#,
	movl	$1, %eax	#,
	vdivsd	%xmm1, %xmm0, %xmm0	# tmp166, tmp164, tmp167
	call	printf@PLT	#
# random.hh:29: }
	addq	$24, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	xorl	%eax, %eax	#
	popq	%rbx	#
	.cfi_def_cfa_offset 32
	popq	%rbp	#
	.cfi_def_cfa_offset 24
	popq	%r12	#
	.cfi_def_cfa_offset 16
	popq	%r13	#
	.cfi_def_cfa_offset 8
	ret	
.L8:
	.cfi_restore_state
# random.hh:10:     int checksum = 0;
	xorl	%r12d, %r12d	# checksum
	jmp	.L5	#
	.cfi_endproc
.LFE10296:
	.size	main, .-main
	.text
	.p2align 4
	.globl	_Z4seedi
	.type	_Z4seedi, @function
_Z4seedi:
.LFB10297:
	.cfi_startproc
# lcg.cc:7:     state = s;
	movl	%edi, state(%rip)	# tmp83, state
# lcg.cc:8: }
	ret	
	.cfi_endproc
.LFE10297:
	.size	_Z4seedi, .-_Z4seedi
	.p2align 4
	.globl	_Z3rngii
	.type	_Z3rngii, @function
_Z3rngii:
.LFB10298:
	.cfi_startproc
# lcg.cc:11:     state = ((state * 1103515245U) + 12345U) & 0x7fffffff;
	imull	$1103515245, state(%rip), %eax	#, state, tmp93
# lcg.cc:12:     return state % (r - l) + l;
	subl	%edi, %esi	# l, tmp97
# lcg.cc:11:     state = ((state * 1103515245U) + 12345U) & 0x7fffffff;
	addl	$12345, %eax	#, tmp95
# lcg.cc:11:     state = ((state * 1103515245U) + 12345U) & 0x7fffffff;
	andl	$2147483647, %eax	#, _6
# lcg.cc:12:     return state % (r - l) + l;
	cltd
# lcg.cc:11:     state = ((state * 1103515245U) + 12345U) & 0x7fffffff;
	movl	%eax, state(%rip)	# _6, state
# lcg.cc:12:     return state % (r - l) + l;
	idivl	%esi	# tmp97
# lcg.cc:12:     return state % (r - l) + l;
	leal	(%rdx,%rdi), %eax	#, tmp96
# lcg.cc:13: }
	ret	
	.cfi_endproc
.LFE10298:
	.size	_Z3rngii, .-_Z3rngii
	.section	.text.startup
	.p2align 4
	.type	_GLOBAL__sub_I_main, @function
_GLOBAL__sub_I_main:
.LFB11930:
	.cfi_startproc
	subq	$8, %rsp	#,
	.cfi_def_cfa_offset 16
# /usr/include/c++/10.2.0/iostream:74:   static ios_base::Init __ioinit;
	leaq	_ZStL8__ioinit(%rip), %rdi	#,
	call	_ZNSt8ios_base4InitC1Ev@PLT	#
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi	#,
	leaq	__dso_handle(%rip), %rdx	#,
	leaq	_ZStL8__ioinit(%rip), %rsi	#,
# lcg.cc:13: }
	addq	$8, %rsp	#,
	.cfi_def_cfa_offset 8
# /usr/include/c++/10.2.0/iostream:74:   static ios_base::Init __ioinit;
	jmp	__cxa_atexit@PLT	#
	.cfi_endproc
.LFE11930:
	.size	_GLOBAL__sub_I_main, .-_GLOBAL__sub_I_main
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_main
	.globl	state
	.bss
	.align 4
	.type	state, @object
	.size	state, 4
state:
	.zero	4
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC2:
	.long	1232348160
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC4:
	.long	0
	.long	1104006501
	.hidden	__dso_handle
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
