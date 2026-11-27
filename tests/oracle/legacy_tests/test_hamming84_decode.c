#include <certivox/cv_hamming84_decode.h>
#include <certivox/cv_hamming84_encode.h>
#include <stdint.h>
#include <stdio.h>
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)
int main(void)
{
    for(uint8_t nibble=0;nibble<16u;++nibble) {
        uint8_t code=0u,out=0xA5u;
        CHECK(cv_hamming84_encode(nibble,&code));
        CHECK(cv_hamming84_decode(code,&out)==CV_HAMMING84_CLEAN&&out==nibble);
        for(unsigned bit=0;bit<8u;++bit) {
            out=0xA5u;
            CHECK(cv_hamming84_decode((uint8_t)(code^(1u<<bit)),&out)==CV_HAMMING84_CORRECTED&&out==nibble);
        }
        for(unsigned a=0;a<8u;++a) for(unsigned b=a+1u;b<8u;++b) {
            out=0xA5u;
            CHECK(cv_hamming84_decode((uint8_t)(code^(1u<<a)^(1u<<b)),&out)==CV_HAMMING84_UNCORRECTABLE&&out==0xA5u);
        }
    }
    uint8_t out=0xA5u;
    CHECK(cv_hamming84_decode(0u,NULL)==CV_HAMMING84_INVALID);
    CHECK(out==0xA5u);
    return 0;
}
