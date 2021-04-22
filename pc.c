#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <x86intrin.h>

#include "util.h"

#define REGION_SIZE (1<<30)
int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		fprintf(stderr,"Usage: %s stride node_count experiment_count\n",argv[0]);
		fprintf(stderr,"8 byte loads used. Effective working set is stride*node_count\n");
		fprintf(stderr,"Only node_count * sizeof(uint64_t) is actually accessed\n");
		exit(-1);
	}

	int stride = atoi(argv[1]);
	int node_count = atoi(argv[2]);
	int experiment_count = atoi(argv[3]);

//TODO: test for incorrectness
/*
	Allocating memory to execute the pointer chasing technique
		mmap syscall used for creating a mapping in the virtual address space of the calling process
		MAP_HUGETLB flag is set which enables Huge Pages 
		i.e.makes it possible for the operating system to support memory pages greater than the default

	malloc is used in case of normal page size allocation.
*/
#ifdef HUGE_PAGE 
	char *test_array = mmap (NULL, REGION_SIZE, PROT_READ|PROT_WRITE, 
	                  MAP_PRIVATE|MAP_ANONYMOUS|MAP_HUGETLB, -1, (off_t)0);
#else
	char* test_array = malloc(REGION_SIZE);
#endif
	memset(test_array,'x',REGION_SIZE);  //Forcing OS to give us data.

	uint64_t* perm_array = malloc(node_count * sizeof(uint64_t));
	gen_cyc_perm(perm_array,node_count);

	//Storing a pc_linked_list in memory
	for(int i=0;i<node_count;i++){
		uint64_t* ptr=(void*)&test_array[i*stride];
		*ptr=perm_array[i]*stride;
	}	
	free(perm_array);

	//Offset is defines as a volatile register which tells the compiler to avoid optimizations
	volatile register uint64_t offset=0;
	warmup_cpu();
	int junk;
	uint64_t time_start,time_end;
	time_start = __rdtscp(&junk);
	for(int i=0;i<experiment_count;i++)
		for(int j=0;j<node_count;j++)
			offset = *(uint64_t*)(test_array+offset);

	time_end = __rdtscp(&junk);
	uint64_t latency = time_end - time_start;
	printf("%ld\t%ld\n",latency, latency/(node_count*experiment_count));
	
	
/*Free the allocated memory*/
#ifdef HUGE_PAGE
	munmap(test_array,REGION_SIZE);
#else
	free(test_array);
#endif
}
