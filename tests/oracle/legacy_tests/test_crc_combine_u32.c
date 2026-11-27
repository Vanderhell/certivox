#include <certivox/cv_crc_combine_u32.h>
#include <certivox/cv_crc32_compute.h>
#include <stdint.h>
#include <stdio.h>
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)
int main(void)
{
    static const uint8_t a[]="1234", b[]="56789", whole[]="123456789";
    uint32_t crc_a,crc_b,expected;
    CHECK(cv_crc32_compute(a,sizeof(a)-1u,&crc_a));
    CHECK(cv_crc32_compute(b,sizeof(b)-1u,&crc_b));
    CHECK(cv_crc32_compute(whole,sizeof(whole)-1u,&expected));
    CHECK(expected==UINT32_C(0xCBF43926));
    CHECK(cv_crc_combine_u32(crc_a,crc_b,(uint32_t)(sizeof(b)-1u))==expected);
    CHECK(cv_crc_combine_u32(crc_a,0u,0u)==crc_a);
    uint8_t left,right,data[2];
    for(unsigned x=0;x<256u;++x) for(unsigned y=0;y<256u;++y) {
        left=(uint8_t)x;right=(uint8_t)y;data[0]=left;data[1]=right;
        CHECK(cv_crc32_compute(&left,1u,&crc_a));
        CHECK(cv_crc32_compute(&right,1u,&crc_b));
        CHECK(cv_crc32_compute(data,2u,&expected));
        CHECK(cv_crc_combine_u32(crc_a,crc_b,1u)==expected);
    }
    return 0;
}
