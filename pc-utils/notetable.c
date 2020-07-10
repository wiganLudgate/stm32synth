// Mikael Hessel 2020-07-10
// make a table of floats for frequency conversion in synth

#include <stdio.h>
#include <math.h>

// using standard A4 as reference
#define REFFREQ     440
#define REFNOTE     69

// piano keys from 21 to 108 (A0 to C8)
// organ from 36 to 96 (C2 to C7)

#define LOWNOTE     16    // about 20 Hz
#define HIGHNOTE    127   // about 12.6 kHz

//

// output a table of floats for the selected interval of notes
void main()
{
    double twelfth = pow(2,(1.f/12));
    float frq = 0.f;
    int row = 0;

    printf("twelfth root of 2 = %g\n",twelfth);
        for(int i=LOWNOTE; i <= HIGHNOTE; i++){
            frq = pow(twelfth,(i - REFNOTE))*REFFREQ; // calculate frequency
            printf("%.9gf, ", frq); // gives the value with 9 digits of precision
            if (row++ >= 5){ row=0; printf("\n"); }
        }

}