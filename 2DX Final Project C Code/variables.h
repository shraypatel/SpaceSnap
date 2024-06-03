#define CW 1 //Clockwise
#define CCW 0 //Counter Clockwise

// 32 measurements = 11.25 deg
// 64 measurements = 5.625 deg
#define SAMPLE_SIZE 64 

#define TOTAL_MEASURES 3

extern int measurements[SAMPLE_SIZE]; // array to hold the measurments from ToF
static char signal; 
extern char motorSteps[]; //array for the motorSteps
extern int delay; // delay variable for between motor steps
extern int spin_status; // vairble to keep track of the motors status