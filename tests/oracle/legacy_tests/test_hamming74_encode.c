#include <certivox/cv_hamming74_encode.h>
#include <stdint.h>
#include <stdio.h>
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)
static unsigned distance(uint8_t a,uint8_t b){unsigned d=0;uint8_t v=a^b;while(v){d+=v&1u;v>>=1u;}return d;}
int main(void)
{
    uint8_t codes[16],out=0xA5u;
    for(unsigned n=0;n<16u;++n){
        CHECK(cv_hamming74_encode((uint8_t)n,&codes[n]));
        const uint8_t c=codes[n];
        CHECK(((c>>0u)^(c>>2u)^(c>>4u)^(c>>6u))%2u==0u);
        CHECK(((c>>1u)^(c>>2u)^(c>>5u)^(c>>6u))%2u==0u);
        CHECK(((c>>3u)^(c>>4u)^(c>>5u)^(c>>6u))%2u==0u);
        CHECK(((c>>2u)&1u)==(n&1u));
        CHECK(((c>>4u)&1u)==((n>>1u)&1u));
        CHECK(((c>>5u)&1u)==((n>>2u)&1u));
        CHECK(((c>>6u)&1u)==((n>>3u)&1u));
    }
    for(unsigned a=0;a<16u;++a) for(unsigned b=0;b<16u;++b) if(a!=b) CHECK(distance(codes[a],codes[b])>=3u);
    out=0xA5u;CHECK(!cv_hamming74_encode(16u,&out)&&out==0xA5u);
    CHECK(!cv_hamming74_encode(0u,NULL));
    return 0;
}
