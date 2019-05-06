/**
run with
gcc -o Posixtwothread 1_e_TwoThread.c -lcrypt -pthread
./Posixtwothread > PosixTwoThreadresult.txt
**/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <crypt.h>
#include <pthread.h>

int n_passwords = 4;
pthread_t thread_1, thread_2;  //thread variables 
//arrayy of hashed passowrd with salt
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
//kernal function 1 that iterates overs password that starts from A to M
void *kernel_function_1(void *salt_and_encrypted){
  int a1, b2, c3;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(a1='A'; a1<='M'; a1++){   //iiterates overs password that starts from A to M
    for(b2='A'; b2<='Z'; b2++){
      for(c3=0; c3<=99; c3++){
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

//kernal function 2 that iterates overs password that starts from N to Z
void *kernel_function_2(void *salt_and_encrypted){
int a1, b2, c3;       // Loop counters
  char salt[7];    // String used in hashing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
  for(a1='N'; a1<='Z'; a1++){  //iterates overs password that starts from N to Z
   for(b2='A'; b2<='Z'; b2++){
     for(c3=0; c3<=99; c3++){
      sprintf(plain, "%c%c%02d", a1, b2, c3);
      enc = (char *) crypt(plain, salt);
      count++;
      if(strcmp(salt_and_encrypted, enc) == 0){
        printf("#%-8d%s %s\n", count, plain, enc);
      } else {
       // printf(" %-8d%s %s\n", count, plain, enc);
      }
    }
  }
}
printf("%d solutions explored\n", count);

}
///time difference code
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
int main(int argc, char *argv[]){
  int i;
  struct  timespec start, finish;
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

   //pthread create and pass kernal function to each thread 
  for(i=0;i<n_passwords;i<i++) {
    pthread_create(&thread_1, NULL, kernel_function_1, encrypted_passwords[i]);
    pthread_create(&thread_2, NULL, kernel_function_2, encrypted_passwords[i]);
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);   
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
   (time_elapsed/1.0e9)); 

  return 0;
}
