#define CRC32MASK  0xEDB88320
#define CRC32MASK1 0x76DC4190
#define CRC32MASK2 0x9B64C2B0

unsigned long crc32(unsigned long crc, unsigned char *addr, unsigned int num)
{
	crc^=0xffffffff;
	for (; num>0; num--)              /* Step through bytes in memory */
	{
		unsigned char c=*addr++;
		unsigned int lowbits;
		int i;

		crc = crc ^ c;
		
		for (i=0;i<4;i++)
		{
			lowbits=crc & 0x3;
			crc >>= 2; 

			switch (lowbits)
			{
				case 1:
					crc ^= CRC32MASK1;   
					break;
				case 2:
					crc ^= CRC32MASK;   
					break;
				case 3:
					crc^= CRC32MASK2;
					break;
				default:
					break;
			}
		}
	}                             
	return(crc^0xffffffff);                 
}
