#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int lcg_x;

/*          Method 1: the random generator built into C.
    The srand(seed) must be called in main before calling the funciton */
unsigned char next_random(void) {
    return rand() % 256;    //a number from 0 to 255 a.k.a. one byte
}

/* Method 2: Linear Congruential Generator */
unsigned char next_lcg(void) {
    lcg_x = 1664525 * lcg_x + 1013904223;   //values extracted from the wiki web page
    return lcg_x % 256;
}

/* Method 3: Subtractive Generator */
unsigned char next_sub(int sub_key[], int *pos) {
    int older = sub_key[*pos];                /* 55 steps ago*/
    int newer = sub_key[(*pos + 31) % 55];    /* 24 steps ago (you add 31 and loop back 55 steps, so pos + 31 - 55 = pos - 24)*/
    unsigned char x = (newer - older) % 256;    //computing the next element in the series

    sub_key[*pos] = x;  //i no longer need the 56th value so i replace it with the one i just calculated for future 
    *pos = (*pos + 1) % 55; //going to the next element in the series
    return x;
}

int main(int argc, char *argv[]) {
    int seed;
    char method[1005];
    int sub_key[60];

    scanf("%s", method);
    if(strcmp(method, "random") == 0 || strcmp(method, "lcg") == 0) {
        scanf("%d", &seed);
        if(seed < 0 || seed > 255) {
            printf("Key is invalid, it must be a nr from 0 to 255\n");
            /* we assume the keys are always bytes, which is 8 bits, aka 256 different values */
            return 1;
        }
        if(strcmp(method, "random") == 0) {
            srand(seed);    //preparing the seed 'random' values
        } else {       //when method is "lcg"
            lcg_x = seed; //in this case, seed is the first element in the recursive series
        }
    } else if(strcmp(method, "sub") == 0) {
        for(int i = 0; i < 55; i++){   //the key must be exactly 55 characters long 
            scanf("%d", &sub_key[i]);
        }
    } else {
        printf("Unknown method. Use random, lcg, or sub.\n");
        return 1;
    }

    int c, pos = 0;
    while((c = getchar()) != EOF && c != '\n') {    //encrypting/decrypting every number in the input by using XOR with one of the methods
        if(strcmp(method, "random") == 0) {
            putchar(c ^ next_random());
        } else if(strcmp(method, "lcg") == 0) {
            putchar(c ^ next_lcg());
        } else {
            putchar(c ^ next_sub(sub_key, &pos));
        }
    }

    return 0;
}