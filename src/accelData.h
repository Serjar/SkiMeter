#define NUMSAMPLES 50
#define MOVINGTHRESHOLD 100
#define STATICTHRESHOLD 70 

void accel_init();

void accel_deinit();

void accel_data_handler(AccelData *, uint32_t );

int get_data_values ( int *  );
