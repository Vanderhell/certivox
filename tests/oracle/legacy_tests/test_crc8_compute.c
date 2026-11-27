#include <certivox/cv_crc8_compute.h>
#include <stdint.h>
#include <stdio.h>
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)
int main(void)
{
    uint8_t crc=0xA5u;
    static const uint8_t check[]="123456789";
    CHECK(cv_crc8_compute(check,sizeof(check)-1u,&crc)&&crc==0xF4u);
    CHECK(cv_crc8_compute(NULL,0u,&crc)&&crc==0u);
    crc=0x5Au; CHECK(!cv_crc8_compute(NULL,1u,&crc)&&crc==0x5Au);
    CHECK(!cv_crc8_compute(check,1u,NULL));
    return 0;
}
