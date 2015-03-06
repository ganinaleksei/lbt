// clang -O3 -target mips-unknown-linux-gnu -c 1.cpp -emit-llvm -o 1.bc

// llvm-link -o=a.bc 1.bc 2.bc
// opt -O3 -o=a1.bc a.bc
// llvm-dis a.bc -o -
// llvm-dis a1.bc -o -

// /usr/local/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -relocation-model=pic -filetype=asm a.bc -o -
// /usr/local/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -relocation-model=pic -filetype=asm a1.bc -o -

// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=cpu0 -relocation-model=pic -filetype=asm a.bc -o -
// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=cpu0 -relocation-model=pic -filetype=asm a1.bc -o -

/// start
int callee(const int *a) {
  return *a+1;
}

