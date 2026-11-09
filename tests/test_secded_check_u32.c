#include <certivox/cv_secded_check_u32.h>
#include <certivox/cv_secded_parity_u32.h>
#include <stdint.h>
#include <stdio.h>
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)
static void flip(uint32_t *data,uint8_t *checks,unsigned position){unsigned c=0,d=0;for(unsigned p=1;p<=38u;++p){if((p&(p-1u))==0u){if(p==position)*checks^=(uint8_t)(1u<<c);++c;}else{if(p==position)*data^=UINT32_C(1)<<d;++d;}}if(position==39u)*checks^=0x40u;}
int main(void)
{
    const uint32_t original=UINT32_C(0x12345678);uint8_t checks=0u;uint32_t out=0u;
    CHECK(cv_secded_parity_u32(original,&checks));
    CHECK(cv_secded_check_u32(original,checks,&out)==CV_SECDED_CLEAN&&out==original);
    for(unsigned position=1u;position<=39u;++position){uint32_t data=original;uint8_t bits=checks;flip(&data,&bits,position);out=0u;CHECK(cv_secded_check_u32(data,bits,&out)==CV_SECDED_CORRECTED&&out==original);}
    for(unsigned a=1u;a<=39u;++a)for(unsigned b=a+1u;b<=39u;++b){uint32_t data=original;uint8_t bits=checks;flip(&data,&bits,a);flip(&data,&bits,b);out=0xA5A5A5A5u;CHECK(cv_secded_check_u32(data,bits,&out)==CV_SECDED_UNCORRECTABLE&&out==0xA5A5A5A5u);}
    out=0xA5A5A5A5u;CHECK(cv_secded_check_u32(original,0x80u,&out)==CV_SECDED_INVALID&&out==0xA5A5A5A5u);
    CHECK(cv_secded_check_u32(original,checks,NULL)==CV_SECDED_INVALID);
    return 0;
}
