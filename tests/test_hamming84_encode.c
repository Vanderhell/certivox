#include <certivox/cv_hamming84_encode.h>
#include <stdint.h>
#include <stdio.h>
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)
static unsigned distance(uint8_t a,uint8_t b){unsigned d=0;uint8_t v=a^b;while(v){d+=v&1u;v>>=1u;}return d;}
int main(void)
{
    uint8_t codes[16],out=0xA5u;
    for(unsigned n=0;n<16u;++n) {
        CHECK(cv_hamming84_encode((uint8_t)n,&codes[n]));
        unsigned ones=0;for(unsigned b=0;b<8u;++b)ones+=(codes[n]>>b)&1u;
        CHECK((ones&1u)==0u);
    }
    for(unsigned a=0;a<16u;++a) for(unsigned b=0;b<16u;++b) if(a!=b) CHECK(distance(codes[a],codes[b])>=4u);
    out=0xA5u;CHECK(!cv_hamming84_encode(16u,&out)&&out==0xA5u);
    CHECK(!cv_hamming84_encode(0u,NULL));
    return 0;
}
