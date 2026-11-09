#include <certivox/cv_secded_parity_u32.h>
#include <stdint.h>
#include <stdio.h>
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"failed line %d\n",__LINE__); return 1; } } while (0)
static uint64_t codeword(uint32_t data,uint8_t checks){uint64_t word=0;unsigned d=0;unsigned c=0;for(unsigned p=1;p<=38u;++p){uint8_t bit;if((p&(p-1u))==0u){bit=(checks>>c)&1u;++c;}else{bit=(data>>d)&1u;++d;}word|=(uint64_t)bit<<(p-1u);}word|=(uint64_t)((checks>>6u)&1u)<<38u;return word;}
static unsigned distance(uint64_t a,uint64_t b){unsigned d=0;uint64_t v=a^b;while(v){d+=(unsigned)(v&1u);v>>=1u;}return d;}
int main(void)
{
    uint64_t words[256];uint8_t checks=0xFFu;
    CHECK(cv_secded_parity_u32(0u,&checks)&&checks==0u);
    for(uint32_t data=0u;data<256u;++data){
        CHECK(cv_secded_parity_u32(data,&checks));
        words[data]=codeword(data,checks);
        for(unsigned p=1u;p<=38u;++p)if((p&(p-1u))==0u){unsigned parity=0;for(unsigned pos=1u;pos<=38u;++pos)if((pos&p)!=0u)parity^=(unsigned)((words[data]>>(pos-1u))&1u);CHECK(parity==0u);}
        CHECK((distance(words[data],0u)&1u)==0u);
    }
    for(unsigned a=0;a<256u;++a)for(unsigned b=0;b<256u;++b)if(a!=b)CHECK(distance(words[a],words[b])>=4u);
    for(unsigned bit=0;bit<32u;++bit){CHECK(cv_secded_parity_u32(UINT32_C(1)<<bit,&checks));CHECK(distance(codeword(UINT32_C(1)<<bit,checks),0u)>=4u);}
    CHECK(!cv_secded_parity_u32(0u,NULL));
    return 0;
}
