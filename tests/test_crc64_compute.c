#include <certivox/cv_crc64_compute.h>
#include <stdint.h>
#include <stdio.h>
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)
int main(void)
{
    uint64_t crc=UINT64_C(0x123456789ABCDEF0);
    static const uint8_t check[]="123456789";
    CHECK(cv_crc64_compute(check,sizeof(check)-1u,&crc)&&crc==UINT64_C(0x6C40DF5F0B497347));
    CHECK(cv_crc64_compute(NULL,0u,&crc)&&crc==0u);
    crc=UINT64_C(0x123456789ABCDEF0); CHECK(!cv_crc64_compute(NULL,1u,&crc)&&crc==UINT64_C(0x123456789ABCDEF0));
    CHECK(!cv_crc64_compute(check,1u,NULL));
    return 0;
}
