#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <math.h>
/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    gcc -o PosixPassword 1_a.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./PosixPassword > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {

	"$6$KB$0G24VuNaA9ApVG4z8LkI/OOr9a54nBfzgQjbebhqBZxMHNg0HiYYf1Lx/HcGg6q1nnOSArPtZYbGy7yc5V.wP/",

	"$6$KB$S8w.zpH4EcQ7.HXep3qUhZMfS6ODZAMrf.UC6BmnWzBWkYKniOzIRDRO4XjWqQY0xbQiAFzFwO2Xx7ZGjoKD90",

	"$6$KB$dL/UJksvXIob3FSHXkSPN.gJ.racNc7al3B5oIoCeH0jwbCGFxJQ4qkLhjGBvtDF4TVnOczq4pz8ZW/WmBpX50",

	"$6$KB$BU6OM3QHROtDFEX259Z7GM6zCH9USl3aVVwsbmGQX2qfprJ/sE8iwPc3/.lUA0nzAObmL7tKREfHDBlDIdrK5/"
};
void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void crack(char *salt_and_encrypted){
  int a1, b2, c3;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(a1='A'; a1<='Z'; a1++){  //iterates over first intiital 
    for(b2='A'; b2<='Z'; b2++){ //iterates over second initial 
      for(c3=0; c3<=99; c3++){   //iterates over last two initials 00 to 99
        sprintf(plain, "%c%c%02d", a1, b2, c3); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          //printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}
//time difference code 
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
//main function
int main(int argc, char *argv[]){
  int i;
  struct  timespec start, finish;
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

  
  for(i=0;i<n_passwords;i<i++) {
    crack(encrypted_passwords[i]);
  }

        clock_gettime(CLOCK_MONOTONIC, &finish);
	time_difference(&start, &finish, &time_elapsed);
	printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
		(time_elapsed/1.0e9)); 
  return 0;
}
