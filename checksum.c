#include "checksum.h"

int check_sum(unsigned short *iphd, int len)
{
	unsigned short checksum = count_check_sum(iphd, len);
	return (checksum == 0)? 0 : -1;

}


unsigned short count_check_sum(unsigned short *iphd, int len)
{
   	unsigned int cksum=0;

    while (len > 1) 
    {
        cksum += *iphd++;
        len -= sizeof(unsigned short);
    }
    if (len) 
    {
        cksum += *(unsigned char *)iphd;
    }
	/*对每个16bit进行二进制反码求和*/
    cksum = (cksum >> 16) + (cksum & 0xffff);	//高16位低16位相加
    cksum += (cksum >>16);						//如果上次相加产生近位则需要加上
    return (unsigned short)(~cksum);
}
