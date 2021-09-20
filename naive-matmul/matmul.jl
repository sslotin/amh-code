n = 128

using LinearAlgebra
BLAS.set_num_threads(1) 

A = rand(n, n)
B = rand(n, n)

function mm(A, B)
    C = zeros(n, n)
    for i = 1:n
        for j = 1:n
            c = 0.0
            @simd for k = 1:n
                c += A[i, k] * B[k, j]
            end
            C[i,j] = c
        end
    end
    C
end

@time mm(A, B)
