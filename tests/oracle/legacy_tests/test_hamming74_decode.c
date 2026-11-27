#include <certivox/cv_hamming74_decode.h>
#include <certivox/cv_hamming74_encode.h>
#include <stdint.h>
#include <stdio.h>
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)
int main(void)
{
    for(uint8_t n=0;n<16u;++n) {
        uint8_t code=0u,decoded=0xA5u;
        CHECK(cv_hamming74_encode(n,&code));
        CHECK(cv_hamming74_decode(code,&decoded)==CV_HAMMING74_SYNDROME_ZERO&&decoded==n);
        for(unsigned bit=0;bit<7u;++bit) {
            decoded=0xA5u;
            CHECK(cv_hamming74_decode((uint8_t)(code^(1u<<bit)),&decoded)==CV_HAMMING74_SYNDROME_NONZERO&&decoded==n);
        }
    }
    for(unsigned word=0;word<128u;++word) {
        uint8_t decoded=0u;
        const cv_hamming74_status status=cv_hamming74_decode((uint8_t)word,&decoded);
        CHECK(status==CV_HAMMING74_SYNDROME_ZERO||status==CV_HAMMING74_SYNDROME_NONZERO);
        CHECK(decoded<16u);
    }
    uint8_t out=0xA5u;
    CHECK(cv_hamming74_decode(0x80u,&out)==CV_HAMMING74_INVALID&&out==0xA5u);
    CHECK(cv_hamming74_decode(0u,NULL)==CV_HAMMING74_INVALID);
    return 0;
}
