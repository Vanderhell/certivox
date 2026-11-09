#include <certivox/cv_crc16_compute.h>
#include <stdint.h>
#include <stdio.h>
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)
int main(void)
{
    uint16_t crc=0x1234u;
    static const uint8_t check[]="123456789";
    CHECK(cv_crc16_compute(check,sizeof(check)-1u,&crc)&&crc==0x4B37u);
    CHECK(cv_crc16_compute(NULL,0u,&crc)&&crc==0xFFFFu);
    crc=0x5AA5u; CHECK(!cv_crc16_compute(NULL,1u,&crc)&&crc==0x5AA5u);
    CHECK(!cv_crc16_compute(check,1u,NULL));
    return 0;
}
