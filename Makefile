all: run_simd run_nosimd

run_simd: build_simd
	./simd_vec

build_simd:
	gcc -mavx simd.c simd_vec.c -o simd_vec -I.

run_nosimd: build_nosimd
	./nosimd

build_nosimd:
	gcc nosimd.c -o nosimd