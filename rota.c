#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

#define __DECLARE_ROTATE(bits, type)                   	\
    static inline type rotl##bits(const type v,int c)  	\
    {   	    				   	\
        const int mask = (bits) - (1);                 	\
        c &= mask;                                     	\
							\
	return (v << c) | (v >> (bits-c));     		\
    }                                                   \
       							\
							\
    static inline type rotr##bits(const type v, int c) 	\
    {                                                  	\
        const int mask = (bits) - (1);                 	\
        c &= mask;                                     	\
                                                       	\
        return (v >> c) | (v << (mask-c));             	\
    }

#define DECLARE_ROTATE(bits) __DECLARE_ROTATE(bits, uint##bits##_t)

DECLARE_ROTATE(64);
DECLARE_ROTATE(8);
DECLARE_ROTATE(32);



static inline uint32_t roll32(uint32_t word, unsigned int shift)
{
	return (word << shift) | (word >> (32-shift));
}


static inline uint32_t rotate_right (uint32_t u, size_t r)
{
    __asm__ volatile("rorl %%cl, %0" : "+r" (u) : "c" (r));
    return u;
}


static inline uint32_t rotate_left (uint32_t u, size_t r)
{
    __asm__  volatile("roll %%cl, %0" : "+r" (u) : "c" (r));
    return u;
}

struct timespec diff(struct timespec start, struct timespec end) {
  struct timespec temp;
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return temp;
}






int main()
{
//struct timespec *ts = malloc(sizeof(struct timespec));
	struct timespec start,end;
	double time_used;
	uint32_t x = 0x10000000;
	clock_gettime(CLOCK_MONOTONIC,&start);
	for(int i = 0;i<10000000;i++)
	{
		//rotate_left(x,2);
		rotl32(x,2);
	}

 	clock_gettime(CLOCK_MONOTONIC, &end);
	struct timespec temp = diff(start, end);
 	time_used = temp.tv_sec + (double) temp.tv_nsec / 1000000000.0;
	printf("x = 0x%x\n",rotl32(x,1));
    	printf("x = 0x%x\n",rotl32(x,2));
     	printf("x = 0x%x\n",rotl32(x,3));
     	printf("x = 0x%x\n",rotl32(x,4));
      	printf("x = 0x%x\n",rotl32(x,5));
    	printf("x = 0x%x\n",rotl32(x,6));
     	//printf("Time = %f\n", time_used);
}
