#include <pebble.h>

#include "accelData.h"

#define INTEGFACTOR 5
//define counters for periods 

static int periodsStatic;
static int periodsLift;
static int periodsSki;

static AccelData *derivData;
static AccelData lastAccel; // used from group to group to hold last values measured

//static Acceldata lastDerivs[INTEGFACTOR];
// FN Declares
uint32_t derivate_data ( AccelData *, AccelData *, uint32_t);
uint32_t max_g_calc (AccelData *, uint32_t);
unsigned short isqrt(unsigned long );

///// Process accel events 

void accel_data_handler(AccelData *data, uint32_t num_samples) {

    app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Event accel with %i samples",(int) num_samples);
    
    AccelData *derivData;
    uint32_t validSamples = 0;

    if (num_samples) {
        derivData = (AccelData *) malloc(sizeof(AccelData)*NUMSAMPLES);
        validSamples = derivate_data(data , derivData, num_samples);

        max_g_calc(derivData, validSamples);
// testing   
        periodsStatic = validSamples;
        periodsSki = data[0].x;
        periodsLift = data[0].y;
        periodsStatic = data [0].z;
//end testing
        free(derivData);
// keep last accel readings
        lastAccel = data[num_samples-1];
    }
}


void accel_init() {
   periodsStatic = 0;
   periodsLift = 0;
   periodsSki = 0;

   derivData = NULL;
   lastAccel.x = lastAccel.y = lastAccel.z = 0;

   accel_data_service_subscribe(NUMSAMPLES, (AccelDataHandler) &accel_data_handler);
   accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
}   

void accel_deinit() {
   accel_data_service_unsubscribe();
// destroy
}  



// Get Values

int get_data_values ( int * p ) {
    p[0] = periodsSki;
    p[1] = periodsLift;
    p[2] = periodsStatic;
    return 0;
}

   

// get derivative of accel on 3 axis

uint32_t derivate_data ( AccelData *data, AccelData *derData, uint32_t samples ) {
    uint32_t j=0;
    if (lastAccel.x || lastAccel.y || lastAccel.z) {
           derData[j].x = data[0].x - lastAccel.x;
           derData[j].y = data[0].y - lastAccel.y;
           derData[j].z = data[0].z - lastAccel.z;
           derData[j].timestamp = lastAccel.timestamp;
           j++;
    }
    
    data[0].x >>= 4;
    data[0].y >>= 4;
    data[0].z >>= 4;
        
    for ( int i=0; i < (int)(samples-1) ; i++)
       if (!data[i].did_vibrate && !data[i+1].did_vibrate) {
           // reduce noise
	   data[i+1].x >>= 4;
           data[i+1].y >>= 4;
           data[i+1].z >>= 4;

           // compute differences

           derData[j].x = data[i+1].x - data[i].x;
           derData[j].y = data[i+1].y - data[i].y;
           derData[j].z = data[i+1].z - data[i].z;
           derData[j].timestamp = data[i+1].timestamp;


    app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Orig: ,%i,%i,%i, %llu ",data[i+1].x,data[i+1].y,data[i+1].z, (long long unsigned int) data[i+1].timestamp );
//    app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Deriv: ,%i,%i,%i",derData[j].x,derData[j].y,derData[j].z );

/*
    if (j>4) 
        app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Integ: ,%i,%i,%i",derData[j].x \
									+ derData[j-1].x \
                                                                        + derData[j-2].x \
                                                                        + derData[j-3].x \
                                                                        + derData[j-4].x, \
									derData[j].y  \
                                                                        + derData[j-1].y \
                                                                        + derData[j-2].y \
                                                                        + derData[j-3].y \
                                                                        + derData[j-4].y,\
									derData[j].z \
                                                                        + derData[j-1].z \
                                                                        + derData[j-2].z \
                                                                        + derData[j-3].z \
                                                                        + derData[j-4].z );
*/
       j++;

    }

    return j-1;
}



//
//
//

uint32_t max_g_calc (AccelData *data, uint32_t numsamples) {
    uint32_t i = 0;

    for (i=0; i<numsamples ; i++) {
      int absolG = isqrt( data[i].x * data[i].x + data[i].y * data[i].y + data[i].z * data[i].z); 
      if ( absolG > MOVINGTHRESHOLD)
	  periodsSki++;
      else
         if(absolG < STATICTHRESHOLD )
            periodsStatic++;
         else
	    periodsLift++;
    }
    return i;
}


//
// SQRT INTEGER
//

unsigned short isqrt(unsigned long a) {
    unsigned long rem = 0;
    unsigned int root = 0;
    unsigned int i;

    for (i = 0; i < 16; i++) {
        root <<= 1;
        rem <<= 2;
        rem += a >> 30;
        a <<= 2;

        if (root < rem) {
            root++;
            rem -= root;
            root++;
        }
    }

    return (unsigned short) (root >> 1);
}

