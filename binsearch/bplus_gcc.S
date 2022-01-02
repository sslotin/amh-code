lower_bound(int):
        mov     rcx, QWORD PTR btree[rip]
        dec     edi
        vmovd   xmm1, edi
        vpbroadcastd    ymm1, xmm1
        vmovdqa ymm3, YMMWORD PTR [rcx+35651712]
        vmovdqa ymm4, YMMWORD PTR [rcx+35651744]
        vpcmpgtd        ymm0, ymm3, ymm1
        vpcmpgtd        ymm2, ymm4, ymm1
        vpackssdw       ymm0, ymm0, ymm2
        vpmovmskb       eax, ymm0
        tzcnt   eax, eax
        shr     eax
        mov     esi, eax
        sal     esi, 4
        mov     edx, esi
        add     eax, esi
        lea     rdx, [rcx+35651264+rdx*4]
        vmovdqa ymm5, YMMWORD PTR [rdx]
        vmovdqa ymm2, YMMWORD PTR [rdx+32]
        vpcmpgtd        ymm0, ymm5, ymm1
        vpcmpgtd        ymm2, ymm2, ymm1
        vpackssdw       ymm0, ymm0, ymm2
        vpmovmskb       edx, ymm0
        tzcnt   edx, edx
        shr     edx
        add     edx, eax
        mov     esi, edx
        sal     esi, 4
        mov     eax, esi
        add     edx, esi
        lea     rax, [rcx+35644416+rax*4]
        vmovdqa ymm6, YMMWORD PTR [rax]
        vmovdqa ymm2, YMMWORD PTR [rax+32]
        vpcmpgtd        ymm0, ymm6, ymm1
        vpcmpgtd        ymm2, ymm2, ymm1
        vpackssdw       ymm0, ymm0, ymm2
        vpmovmskb       eax, ymm0
        tzcnt   eax, eax
        shr     eax
        add     eax, edx
        mov     esi, eax
        sal     esi, 4
        mov     edx, esi
        add     eax, esi
        lea     rdx, [rcx+35528256+rdx*4]
        vmovdqa ymm7, YMMWORD PTR [rdx]
        vmovdqa ymm2, YMMWORD PTR [rdx+32]
        vpcmpgtd        ymm0, ymm7, ymm1
        vpcmpgtd        ymm2, ymm2, ymm1
        vpackssdw       ymm0, ymm0, ymm2
        vpmovmskb       edx, ymm0
        tzcnt   edx, edx
        shr     edx
        add     edx, eax
        mov     esi, edx
        sal     esi, 4
        mov     eax, esi
        add     edx, esi
        lea     rax, [rcx+33554432+rax*4]
        vmovdqa ymm3, YMMWORD PTR [rax]
        vmovdqa ymm2, YMMWORD PTR [rax+32]
        vpcmpgtd        ymm0, ymm3, ymm1
        vpcmpgtd        ymm2, ymm2, ymm1
        vpackssdw       ymm0, ymm0, ymm2
        vpmovmskb       eax, ymm0
        tzcnt   eax, eax
        shr     eax
        add     eax, edx
        sal     eax, 4
        mov     edx, eax
        lea     rdx, [rcx+rdx*4]
        vmovdqa ymm4, YMMWORD PTR [rdx+32]
        vpcmpgtd        ymm0, ymm4, ymm1
        vmovmskps       esi, ymm0
        vmovdqa ymm0, YMMWORD PTR [rdx]
        sal     esi, 8
        mov     edx, esi
        vpcmpgtd        ymm1, ymm0, ymm1
        vmovmskps       edi, ymm1
        or      edx, edi
        or      edx, 65536
        tzcnt   edx, edx
        add     eax, edx
        mov     eax, DWORD PTR [rcx+rax*4]
        vzeroupper
        ret