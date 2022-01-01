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