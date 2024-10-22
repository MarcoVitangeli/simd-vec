all: run_simd_sum run_nosimd_sum run_nosimd_dp run_simd_dp

sum: run_simd_sum run_nosimd_sum

dp: run_simd_dp run_nosimd_dp

run_simd_sum: build_simd_sum
	./sumsimd

build_simd_sum:
	gcc -mavx2 sumsimd.c simd_vec.c -o sumsimd -I.

run_nosimd_sum: build_nosimd_sum
	./sumnosimd

build_nosimd_sum:
	gcc sumnosimd.c -o sumnosimd

run_simd_dp: build_simd_dp
	./dpsimd

build_simd_dp:
	gcc -mavx2 dpsimd.c simd_vec.c -o dpsimd -I.

run_nosimd_dp: build_nosimd_dp
	./dpnosimd


build_nosimd_dp:
	gcc dpnosimd.c -o dpnosimd