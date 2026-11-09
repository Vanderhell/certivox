#include <certivox/cv_crc24_compute.h>
#include <stdint.h>
#include <stdio.h>
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)
int main(void)
{
    uint32_t crc=0x12345678u;
    static const uint8_t check[]="123456789";
    CHECK(cv_crc24_compute(check,sizeof(check)-1u,&crc)&&crc==0x21CF02u);
    CHECK(cv_crc24_compute(NULL,0u,&crc)&&crc==0xB704CEu);
    crc=0x12345678u; CHECK(!cv_crc24_compute(NULL,1u,&crc)&&crc==0x12345678u);
    CHECK(!cv_crc24_compute(check,1u,NULL));
    return 0;
}
