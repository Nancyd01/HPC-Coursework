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
    cc -o CrackCCC99-With-Data 1_C_ThreeInitial.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./CrackCCC99-With-Data > results.txt
The password used is CCC99 


  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 1;
// encrypted password hash function in array
char *encrypted_passwords[] = {
    "$6$KB$HHtmO1Kn/YG/835DhBsg8i8.R5z6QiToKTUA5UOZ4Q3BeW8EGYALj1reFInGCdDwEQyx5v8mIrEuvM1S0xTHy."
};

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

void crack(char *salt_and_encrypted){
    int a1, b2, c3, digit;     // Loop counters
    char salt[7];    // String used in hashing the password. Need space
    char plain[7];   // The combination of letters currently being checked
    char *enc;       // Pointer to the encrypted password
    int count = 0;   // The number of combinations explored so far

    substr(salt, salt_and_encrypted, 0, 6);

    for(a1='A'; a1<='Z'; a1++){ // loop for first initial 
        for(b2='A'; b2<='Z'; b2++){// loop for second initial
            for(c3='A'; c3<='Z'; c3++){ // loop for third initial 
                for(digit=0; digit<=99; digit++){ //loop for last 2 digits 00 to 99
                    sprintf(plain, "%c%c%c%02d", a1, b2, c3, digit);
                    enc = (char *) crypt(plain, salt);
                    count++;
                    if(strcmp(salt_and_encrypted, enc) == 0){
                        printf("#%-8d%s %s\n", count, plain, enc);
                    } else {
                        printf(" %-8d%s %s\n", count, plain, enc);
                    }
                }
            }
        }

    }

    printf("%d solutions explored\n", count);
}
//time calculation fucnction
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

    for(i=0;i<n_passwords;i<i++) {
        crack(encrypted_passwords[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
    time_difference(&start, &finish, &time_elapsed);
    printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
     (time_elapsed/1.0e9));

    return 0;
}
