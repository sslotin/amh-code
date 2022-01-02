lower_bound(int):                       # @lower_bound(int)
        mov     rax, qword ptr [rip + btree]
        dec     edi
        vmovd   xmm0, edi
        vpbroadcastd    ymm0, xmm0
        vmovdqa ymm1, ymmword ptr [rax + 35651712]
        vmovdqa ymm2, ymmword ptr [rax + 35651744]
        vpcmpgtd        ymm1, ymm1, ymm0
        vpcmpgtd        ymm2, ymm2, ymm0
        vpackssdw       ymm1, ymm1, ymm2
        vpmovmskb       ecx, ymm1
        tzcnt   ecx, ecx
        shr     ecx
        mov     edx, ecx
        shl     edx, 4
        vmovdqa ymm1, ymmword ptr [rax + 4*rdx + 35651264]
        vmovdqa ymm2, ymmword ptr [rax + 4*rdx + 35651296]
        add     edx, ecx
        vpcmpgtd        ymm1, ymm1, ymm0
        vpcmpgtd        ymm2, ymm2, ymm0
        vpackssdw       ymm1, ymm1, ymm2
        vpmovmskb       esi, ymm1
        tzcnt   esi, esi
        shr     esi
        add     esi, edx
        mov     ecx, esi
        shl     ecx, 4
        vmovdqa ymm1, ymmword ptr [rax + 4*rcx + 35644416]
        vmovdqa ymm2, ymmword ptr [rax + 4*rcx + 35644448]
        add     ecx, esi
        vpcmpgtd        ymm1, ymm1, ymm0
        vpcmpgtd        ymm2, ymm2, ymm0
        vpackssdw       ymm1, ymm1, ymm2
        vpmovmskb       edx, ymm1
        tzcnt   edx, edx
        shr     edx
        add     edx, ecx
        mov     ecx, edx
        shl     ecx, 4
        vmovdqa ymm1, ymmword ptr [rax + 4*rcx + 35528256]
        vmovdqa ymm2, ymmword ptr [rax + 4*rcx + 35528288]
        add     ecx, edx
        vpcmpgtd        ymm1, ymm1, ymm0
        vpcmpgtd        ymm2, ymm2, ymm0
        vpackssdw       ymm1, ymm1, ymm2
        vpmovmskb       esi, ymm1
        tzcnt   esi, esi
        shr     esi
        add     esi, ecx
        mov     ecx, esi
        shl     ecx, 4
        vmovdqa ymm1, ymmword ptr [rax + 4*rcx + 33554432]
        vmovdqa ymm2, ymmword ptr [rax + 4*rcx + 33554464]
        add     ecx, esi
        vpcmpgtd        ymm1, ymm1, ymm0
        vpcmpgtd        ymm2, ymm2, ymm0
        vpackssdw       ymm1, ymm1, ymm2
        vpmovmskb       edx, ymm1
        tzcnt   edx, edx
        shr     edx
        add     ecx, edx
        shl     ecx, 4
        vmovdqa ymm2, ymmword ptr [rax + 4*rcx + 32]
        vmovdqa ymm1, ymmword ptr [rax + 4*rcx]
        vpcmpgtd        ymm2, ymm2, ymm0
        vpcmpgtd        ymm0, ymm1, ymm0
        vmovmskps       edx, ymm2
        vmovmskps       esi, ymm0
        shl     edx, 8
        lea     edx, [rdx + rsi + 65536]
        tzcnt   edx, edx
        add     edx, ecx
        mov     eax, dword ptr [rax + 4*rdx]
        vzeroupper
        ret