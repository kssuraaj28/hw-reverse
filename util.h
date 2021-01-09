#pragma once
#include <string.h>
#include <stdlib.h>
#include <time.h>
void warmup_cpu()
{
  for(int i=0;i<100000000;i++);
}
static inline void swap (uint64_t* t1, uint64_t* t2)
{
  uint64_t temp = *t1;
  *t1 = *t2;
  *t2 = temp;
}

int gen_perm(uint64_t* perm_array,uint64_t count)
{
	srand(time(NULL));
	for(uint64_t i=0;i<count;i++)
		perm_array[i] = i;

	for(int i=count-2;i>=0;i--)
	{
		uint64_t temp = rand() % (i+1);
		swap(&perm_array[i],&perm_array[temp]);
	}
  return 0;
}

void gen_cyc_perm(uint64_t* cindex,uint64_t count) //Generates a cyclic perm,  the order of perm is n
{
  uint64_t* perm_array = malloc ((count)*sizeof(uint64_t));
  //Now we have an array like this 4 2 1 3 5. The last number will be the index

  gen_perm(perm_array,count);

  for(int i=0;i<count-1;i++)
    cindex[perm_array[i]] = perm_array[i+1];
  cindex[count - 1] = perm_array[0]; //count-1 == perm_array[count-1];

  free(perm_array);
}

