vmovdqa     ymm5, YMMWORD PTR [rcx]
vmovdqa     ymm2, YMMWORD PTR [rcx+32]
vpcmpgtd    ymm0, ymm5, ymm1
vpcmpgtd    ymm2, ymm2, ymm1
vpackssdw   ymm0, ymm0, ymm2
vpmovmskb   ecx, ymm0
tzcnt       ecx, ecx
shr         ecx

movsx       rdi, ecx
cmp         ecx, 16
movsx       rdi, DWORD PTR translate[0+rdi*4]
lea         rdi, [rdx+rdi*4]
cmovb       r8d, DWORD PTR [rdi+rsi]

mov         esi, eax
sal         esi, 4
add         eax, esi
lea         esi, [rcx+1+rax]
movsx       rax, esi
sal         rax, 6
lea         rcx, [rdx+rax]

# ---

vmovdqa ymm5, YMMWORD PTR [rsi]
vmovdqa ymm6, YMMWORD PTR [rsi+32]
vpcmpgtd        ymm0, ymm5, ymm1
vpcmpgtd        ymm2, ymm6, ymm1
vpackssdw       ymm0, ymm0, ymm2
vpmovmskb       edx, ymm0
tzcnt   edx, edx
shr     edx

movsx   rdi, edx
cmp     edx, 15
movsx   rdi, DWORD PTR translate[0+rdi*4]
cmovbe  r8d, DWORD PTR [rsi+rdi*4]

add     edx, eax
movsx   rsi, edx
mov     edi, edx
sal     rsi, 6
sal     edi, 4
add     rsi, rcx
lea     edx, [rdx+1+rdi]