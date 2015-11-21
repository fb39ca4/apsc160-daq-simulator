#define DAQ_NUM_DIGITAL_OUTPUTS 6
#define DAQ_NUM_DISPLAY_OUTPUTS 8
#define DAQ_NUM_DIGITAL_INPUTS 4
#define DAQ_NUM_ANALOG_INPUTS 2

int setupDAQ(int setup_num);
int continueSuperLoop(void);
void digitalWrite(int channel_number, int val);
int digitalRead(int channel_number);
void displayWrite(int data, int position);
double analogRead(int channel_number);