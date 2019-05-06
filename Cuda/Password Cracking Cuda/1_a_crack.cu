#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>
/****************************************************************************
  This program gives an example of a poor way to implement a password cracker
  in CUDA C. It is poor because it acheives this with just one thread, which
  is obviously not good given the scale of parallelism available to CUDA
  programs.
  
  The intentions of this program are:
    1) Demonstrate the use of __device__ and __global__ functions
    2) Enable a simulation of password cracking in the absence of library 
       with equivalent functionality to libcrypt. The password to be found
       is hardcoded into a function called is_a_match.   

  Compile and run with:
    nvcc -o 1_a_crack 1_a_crack.cu
    ./1_a_crack
   
  Dr Kevan Buckley, University of Wolverhampton, 2018
*****************************************************************************/

/****************************************************************************
  This function returns 1 if the attempt at cracking the password is 
  identical to the plain text password string stored in the program. 
  Otherwise,it returns 0.
*****************************************************************************/

__device__ int is_a_match(char *attempt) {
	char plain_password1[] = "AA1111";
	char plain_password2[] = "BB2222";
	char plain_password3[] = "SS3333";
	char plain_password4[] = "ZZ4444";


	char *a = attempt;
	char *b = attempt;
	char *c = attempt;
	char *d = attempt;
	char *p1 = plain_password1;
	char *p2 = plain_password2;
	char *p3 = plain_password3;
	char *p4 = plain_password4;

	while(*a == *p1) { 
		if(*a == '\0') 
		{
			printf("Password: %s\n",plain_password1);
			break;
		}

		a++;
		p1++;
	}
	
	while(*b == *p2) { 
		if(*b == '\0') 
		{
			printf("Password: %s\n",plain_password2);
			break;
		}

		b++;
		p2++;
	}

	while(*c == *p3) { 
		if(*c == '\0') 
		{
			printf("Password: %s\n",plain_password3);
			break;
		}

		c++;
		p3++;
	}

	while(*d == *p4) { 
		if(*d == '\0') 
		{
			printf("Password: %s\n",plain_password4);
			return 1;
		}

		d++;
		p4++;
	}
	return 0;

}

__global__ void  kernel() {
	char a1,b2,c3,d4;//variables 

	char password[7];
	password[6] = '\0';
	//block id threrad id initilized
	int i = blockIdx.x+65;
	int j = threadIdx.x+65;
	char firstMatch = i; 
	char secondMatch = j; 

	password[0] = firstMatch;
	password[1] = secondMatch;
	for(a1='0'; a1<='9'; a1++){
		for(b2='0'; b2<='9'; b2++){
			for(c3='0'; c3<='9'; c3++){
				for(d4='0'; d4<='9'; d4++){
					password[2] = a1;
					password[3] = b2;
					password[4] = c3;
					password[5] = d4; 
					if(is_a_match(password)) {
					} 
					else {
	     			//printf("tried: %s\n", password);		  
					}
				}
			}
		}
	}
}
//time difference
int time_difference(struct timespec *start, 
	struct timespec *finish, 
	long long int *difference) {
	long long int ds =  finish->tv_sec - start->tv_sec; 
	long long int dn =  finish->tv_nsec - start->tv_nsec; 
	if(dn < 0 ) {
		ds--;
		dn += 1000000000; 
	} 
	*difference = ds * 1000000000 + dn;
	return !(*difference > 0);
}


int main() {

	struct  timespec start, finish;
	long long int time_elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);
	//kernal function that determine block and threads to use
	kernel <<<26,26>>>();
	cudaThreadSynchronize();

	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_difference(&start, &finish, &time_elapsed);
	printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed/1.0e9)); 

	return 0;
}
