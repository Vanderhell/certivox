#include <certivox/cv_crc_update_byte_u32.h>
#include <stdint.h>
#include <stdio.h>
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)
int main(void)
{
    static const uint8_t text[]="123456789";
    uint32_t crc=UINT32_MAX;
    for(unsigned i=0;i<sizeof(text)-1u;++i) crc=cv_crc_update_byte_u32(crc,text[i],UINT32_C(0xEDB88320));
    CHECK((crc^UINT32_MAX)==UINT32_C(0xCBF43926));
    CHECK(cv_crc_update_byte_u32(0u,0u,0u)==0u);
    CHECK(cv_crc_update_byte_u32(UINT32_MAX,UINT8_MAX,0u)==UINT32_C(0x00FFFFFF));
    for(uint32_t seed=0u;seed<256u;++seed) for(unsigned byte=0u;byte<256u;++byte) {
        const uint32_t raw=(seed*UINT32_C(0x01010101))^UINT32_C(0xA5A55A5A);
        const uint32_t got=cv_crc_update_byte_u32(raw,(uint8_t)byte,UINT32_C(0xEDB88320));
        uint32_t expected=raw^(uint8_t)byte;
        for(unsigned bit=0;bit<8u;++bit) expected=(expected&1u)?(expected>>1u)^UINT32_C(0xEDB88320):expected>>1u;
        CHECK(got==expected);
    }
    return 0;
}
