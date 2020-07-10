
// Mikael Hessel 2020-07-10

// sine table generator for synth
// generates a table of floats of given length to paste into an array

#include <stdio.h>
#include <math.h>

#define LENGTH      2048 // length of table

void main()
{
    double delta = (2 * M_PI)/LENGTH;
    float sample = 0.f;
    int row = 0;

    printf("SINE TABLE, %d values\n",LENGTH);
        for(int i=0; i < LENGTH; i++){
            sample = sinf(i*delta); // calculate sample
            printf("%.9gf, ", sample); // gives the value with 9 digits of precision
            
            if (row < 6){ row++; }else{row=0; printf("\n"); } // six values per row
        }

}