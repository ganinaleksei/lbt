// clang -c ch14_inherit.cpp -emit-llvm -DTEST_RUN -o ch14_inherit.bc
// clang -c ch14_inherit.cpp -emit-llvm -DTEST_RUN -DCOUT_TEST -o ch14_inherit.bc
// /Users/Jonathan/llvm/test/cmake_debug_build/Debug/bin/llvm-dis ch14_inherit.bc -o -
// /Users/Jonathan/llvm/test/cmake_debug_build/Debug/bin/llc -march=cpu0 -relocation-model=static -filetype=asm ch14_inherit.bc -o -

/// start
#ifdef COUT_TEST
#include <iostream>
using namespace std;
#endif

extern "C" int printf(const char *format, ...);
extern "C" int sprintf(char *out, const char *format, ...);

class CPolygon { // _ZTVN10__cxxabiv117__class_type_infoE for parent class
  protected:
    int width, height;
  public:
    void set_values (int a, int b)
    { width=a; height=b; }
//    virtual int area (void) =0; // __cxa_pure_virtual
    virtual int area (void) { return 0;};
    void printarea (void)
#ifdef COUT_TEST
 // generate IR nvoke, landing, resume and unreachable on iMac
    { cout << this->area() << endl; }
#else
    { printf("%d\n", this->area()); }
#endif
  };

// _ZTVN10__cxxabiv120__si_class_type_infoE for derived class
class CRectangle: public CPolygon {
  public:
    int area (void)
    { return (width * height); }
};

class CTriangle: public CPolygon {
  public:
    int area (void)
    { return (width * height / 2); }
};

class CAngle: public CPolygon {
  public:
    int area (void)
    { return (width * height / 4); }
};
#if 0
int test_cpp_polymorphism() {
  CPolygon * ppoly1 = new CRectangle;	// _Znwm
  CPolygon * ppoly2 = new CTriangle;
  ppoly1->set_values (4,5);
  ppoly2->set_values (4,5);
  ppoly1->printarea();
  ppoly2->printarea();
  delete ppoly1;	// _ZdlPv
  delete ppoly2;
  return 0;
}
#else
int test_cpp_polymorphism() {
  CRectangle poly1;
  CTriangle poly2;
  CAngle poly3;
  
  CPolygon * ppoly1 = &poly1;
  CPolygon * ppoly2 = &poly2;
  CPolygon * ppoly3 = &poly3;
  
  ppoly1->set_values (4,5);
  ppoly2->set_values (4,5);
  ppoly3->set_values (4,5);
  ppoly1->printarea();
  ppoly2->printarea();
  ppoly3->printarea();
  return 0;
}
#endif

#ifdef TEST_RUN
int main() {
  return test_cpp_polymorphism();
}
#endif
