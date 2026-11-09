#include <certivox/cv_crc_reflect_poly_u32.h>
#include <stdint.h>
#include <stdio.h>
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)
static uint32_t reference(uint32_t poly,unsigned width){uint32_t out=0u;for(unsigned i=0;i<width;++i)out|=((poly>>i)&1u)<<(width-1u-i);return out;}
int main(void)
{
    uint32_t out=0xA5A5A5A5u;
    CHECK(cv_crc_reflect_poly_u32(0x07u,8u,&out)&&out==0xE0u);
    CHECK(cv_crc_reflect_poly_u32(0x8005u,16u,&out)&&out==0xA001u);
    CHECK(cv_crc_reflect_poly_u32(0x04C11DB7u,32u,&out)&&out==0xEDB88320u);
    for(unsigned width=1u;width<=8u;++width)for(uint32_t poly=0u;poly<(UINT32_C(1)<<width);++poly){CHECK(cv_crc_reflect_poly_u32(poly,(uint8_t)width,&out)&&out==reference(poly,width));uint32_t back=0;CHECK(cv_crc_reflect_poly_u32(out,(uint8_t)width,&back)&&back==poly);}
    out=0xA5A5A5A5u;CHECK(!cv_crc_reflect_poly_u32(0u,0u,&out)&&out==0xA5A5A5A5u);
    CHECK(!cv_crc_reflect_poly_u32(0u,33u,&out)&&out==0xA5A5A5A5u);
    CHECK(!cv_crc_reflect_poly_u32(0x100u,8u,&out)&&out==0xA5A5A5A5u);
    CHECK(!cv_crc_reflect_poly_u32(0u,8u,NULL));
    return 0;
}
