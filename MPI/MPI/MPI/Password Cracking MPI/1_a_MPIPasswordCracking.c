#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <crypt.h>
#include <mpi.h>
/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    mpicc -o 1_a_MPIPasswordCracking 1_a_MPIPasswordCracking.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    mpirun -n 3 ./1_a_MPIPasswordCracking

  Dr Kevan Buckley, University of Wolverhampton, 2019
******************************************************************************/
int n_passwords = 4;
// encrypted passwords are AA1111, BB2222, SS3333, ZZ4444
char *encrypted_passwords[] = {

"$6$KB$YJ2cyUdgpCT/xIjdOjTkhYBZ9ZMGhECA6s0Oa2qcqet4QG99J1KOp1kU8KM96NAv.MUefq7znOGoBPT/R/Qof1",
"$6$KB$joc9OPGjxifQU65CL8CBjrzktkRaTaCh6V4EGA3GDVQNU1o6SXjZkPEzFv7sY6AKVKygbtHTPQ8sW.gvlkd0x.",
"$6$KB$EU/6W9xieuBxRicwU8jww278fq5AB4pO3PoA1jAZ4.a.zuLiV/q.5N2FvdJunOcAjjMKi/ygDMe8kRkCbdr01.",
"$6$KB$ShfuEhmrWupfgZMOqNv3Zj4Lqu8H064/byysYxm/hDthUkQEHNimhxLx1BDwc8c2yGHg7kF9RncwvObSWCfd2."
};

/**
 Required by lack of standard function in C.   
*/

void crack(char *salt_and_encrypted, char start, char finish);

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All
combinations
 that are tried are displayed and when the password is found, #, is put
at the
 start of the line. Note that one of the most time consuming operations
that
 it performs is the output of intermediate results, so performance
experiments
 for this kind of program should not include this. i.e. comment out the
printfs.
*/

void crack(char *salt_and_encrypted, char start, char finish){
  int x, y, z, w;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
  for(x= start; x<= finish; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
         for(w=0; w<=99; w++){
        sprintf(plain, "%c%c%02d%02d",x, y, z, w); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
         //printf(" %-8d%s %s\n", count, plain, enc);
        }}
      }
    }
}
  printf("%d solutions explored\n", count);
}
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

// Calculate the difference between two times. Returns zero on
// success and the time difference through an argument. It will 
// be unsuccessful if the start time is after the end time.


int main(int argc, char **argv)
{
	int size, rank;
	struct timespec start, finish;
	long long int time_elapsed;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(size != 3)
	{
		if(rank == 0)
			{
			printf("This program needs exactly 3 process to run\n");
			exit(-1); 			
			}
	}
	else
		{
			for(int i=0; i<n_passwords; i++)
				{
				if(rank==0){
					clock_gettime(CLOCK_MONOTONIC, &start);
					MPI_Send(&encrypted_passwords[i][0],512,MPI_BYTE,1,0,MPI_COMM_WORLD);
					MPI_Send(&encrypted_passwords[i][0],512,MPI_BYTE,2,0,MPI_COMM_WORLD);
					}
				else
					{
					if(rank==1){
						char password[512];
						MPI_Status status;
						MPI_Recv(&password[0], 512,MPI_BYTE,0,0,MPI_COMM_WORLD,&status);
						crack(password, 'A', 'M');			
						}
					if(rank==2){
						char password[512];
						MPI_Status status;
						MPI_Recv(&password[0], 512,MPI_BYTE,0,0,MPI_COMM_WORLD,&status);
						crack(password, 'N', 'Z');			
						}	
					}
				}
		}
	MPI_Barrier(MPI_COMM_WORLD);
		if(rank==0)
			{
				clock_gettime(CLOCK_MONOTONIC, &finish);
  				time_difference(&start, &finish, &time_elapsed);
  				printf("Time elapsed was %lldns or %0.9lfs or %0.9lfmin\n",time_elapsed,(time_elapsed/1.0e9),((time_elapsed/1.0e9)/60));
			}
		MPI_Finalize();
return 0;

}
