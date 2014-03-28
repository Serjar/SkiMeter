#define NUMSAMPLES 25
#define DATAHZ 25
#define MOVINGTHRESHOLD 140
#define STATICTHRESHOLD MOVINGTHRESHOLD/2 

#define THUPKEY 3
#define THDOWNKEY 4

#define MAXCALIBTRY 30

// FNS Exported

void accel_init();

void accel_deinit();

void accel_data_handler(AccelData *, uint32_t );

void calibrate_data_handler(AccelData *, uint32_t );

void calibrate_accel_start();

void calibrate_accel_end(bool);

int get_data_values ( int *  );
