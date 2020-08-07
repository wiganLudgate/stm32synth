// MOOG Table
// For precalculating cutoff parameters of filter 
// (since apparently it's to slow to do in realtime on STM32?)


#define SRATE 44100 // samplerate used

#include <math.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char* argv[])
{   
    float c;            // filter frequency
    float cutoff;       // cutoff value
    float t1,t2,p,k;    // parameters to calculate

    int steps;
    float low, high, fdt;

    if (argc == 3){
        low = atof(argv[0]);
        high = atof(argv[1]);
        steps = atoi(argv[2]);
    }else{
        printf("moogtable <lower frq> <upper frq> <number steps>\n");
        printf("using default: 100 Hz, 1000 Hz and 10 steps.\n\n");

        low = 100.0;
        high = 1000.0;
        steps = 10;
    }


    // set up
    fdt = (high - low)/(steps - 1);
    c = low;

    for(int i = 0; i < steps; i++){

        cutoff = 2.0 * c / SRATE;
    	p = cutoff * (1.8 - 0.8 * cutoff);
	    k = 2.0 * sin(cutoff * M_PI * 0.5) - 1.0;
	    t1 = (1.0 - p) * 1.386249;
	    t2 = 12.0 + t1 * t1;


        printf("// %d of %d, %f Hz\n", (i+1), steps, c);
        printf("%.9gf, %.9gf, %.9gf, %.9gf", p, k, t1, t2);
        if(i < (steps - 1)){ printf(","); }
        printf("\n");
        c += fdt;
    }
}

