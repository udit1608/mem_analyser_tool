#include<common.h>
#include<stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>                /* strcmp() */
#include <stdlib.h>                /* strtoul() */
#include <unistd.h>                /* close() */

int g_size = 4;
unsigned long int g_paddr;
uint32_t g_value = 0;
uint32_t g_count = 1;

using namespace std;
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;

extern class general_data gd;
extern vector<info_reg_dump> vector_reg_info;

void read_mem(void * addr, uint32_t count, uint32_t size, int *val)
{
	int i;
	uint8_t * addr8 = (uint8_t *)addr;
	uint16_t * addr16 = (uint16_t *)addr;
	uint32_t * addr32 = (uint32_t *)addr;

	switch (size)
	{
		case 1:
			*val = addr8[0];
			break;
		case 2:
			*val = addr16[0];
			break;
		case 4:
			*val = addr32[0];
			break;
	}
}

int get_regs_value()
{
	int i ,j = 0; int size;
	int fd;
	unsigned long int paddress;
	unsigned long aligned_paddr;
	void * aligned_vaddr, * mem;
	uint32_t aligned_size;

	for(i = 0; i < gd.num_of_reg_dumps; i++) {
		string addr = vector_reg_info[i].addr;
		// Convert the string object to char *
		char data[20] = {'\0'};
		for ( j = 0; j < addr.size(); j++)
			data[j] = addr[j]; // don't forget the terminating 0
		data[j] = '\0';

		//		paddress = strtoul(data, NULL, 16);
		size = vector_reg_info[i].size;

		// Get the address from the data field
		g_paddr = strtoul(data, NULL, 16);

		// set the g_size for number of bytes field
		if (size  == 32)
			g_size = 4;
		else if (size == 16)
			g_size = 2;
		else if (size == 8)
			g_size = 1;
		else
			g_size = 1;


		//allign the g_paddr
		g_paddr &= ~(g_size - 1);

		// allign the g_paddr to 4K page size
		aligned_paddr = g_paddr & ~(4096 - 1);

		//
		aligned_size = g_paddr - aligned_paddr + (g_count * g_size);
		aligned_size = (aligned_size + 4096 - 1) & ~(4096 - 1);

		fd = open("/dev/mem", O_RDWR);

		aligned_vaddr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, aligned_paddr);

		if (aligned_vaddr == NULL) {
			vector_reg_info[i].valid_bit = 0;
			cout<<" \t Error mapping address"<<endl;
		} else {
			vector_reg_info[i].valid_bit = 1;
			mem = (void *)((intptr_t)aligned_vaddr + (g_paddr - aligned_paddr));
			read_mem(mem, g_count, g_size, &vector_reg_info[i].val);
		}
	}

	munmap(aligned_vaddr, aligned_size);
	close(fd);

	return 1;
}
