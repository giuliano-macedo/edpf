#include "bbmed.h"
#include "benchmark.h"
long unsigned int timer(struct timespec* s,struct timespec* e,int f){
	if(f){
		clock_gettime(CLOCK_MONOTONIC_RAW, s);
		return 0;  
	}
	clock_gettime(CLOCK_MONOTONIC_RAW, e);
	return (e->tv_sec - s->tv_sec) * 1000000 + (e->tv_nsec - s->tv_nsec) / 1000;
}
int main(int argc,char** argv){
	if(argc!=2){
		printf("[USO] %s [arquivo de usuarios]\n",argv[0]);
	}


}