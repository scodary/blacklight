// Blacklight v1
//
// An arduino prototype of the blacklight, which
// responds to human body heat by adjusting the
// color temperature of a flashlight, from a dim
// red ember, through a bright white light, up
// to a very bright blue sky light.

// The structure of the lookup data
const int MIN_TEMP_K = 1000;
const int MAX_TEMP_K = 10000;
const int TEMP_STEP_K = 250;
const int BLACKBODY_LOOKUP_SIZE = (MAX_TEMP_K - MIN_TEMP_K)/TEMP_STEP_K + 1;

// Converts a temperature to its (full-brightness)
// RGBW values. These were calculated using a
// seperate utility I wrote that estimates both the
// blackbody and LED-spectral CIE values for a given
// set of PWM brightness values.
static int blackbody[BLACKBODY_LOOKUP_SIZE][4] = {
{255, 8, 0, 0},
{255, 17, 0, 0},
{255, 28, 0, 0},
{255, 38, 2, 0},
{255, 47, 17, 0},
{255, 55, 37, 0},
{255, 62, 63, 0},
{255, 67, 79, 3},
{255, 69, 76, 17},
{255, 71, 77, 34},
{255, 73, 83, 52},
{255, 76, 93, 74},
{255, 80, 110, 101},
{255, 85, 134, 134},
{254, 88, 163, 169},
{255, 90, 197, 206},
{255, 91, 237, 245},
{226, 80, 250, 255},
{194, 68, 255, 252},
{164, 56, 254, 245},
{138, 46, 255, 238},
{117, 37, 255, 231},
{99, 30, 254, 224},
{83, 24, 255, 218},
{69, 19, 255, 212},
{56, 14, 255, 206},
{45, 10, 255, 200},
{36, 7, 255, 195},
{27, 4, 255, 191},
{20, 2, 255, 186},
{13, 1, 255, 181},
{7, 0, 255, 175},
{2, 0, 255, 169},
{0, 0, 255, 157},
{0, 0, 255, 150},
{0, 0, 255, 144}
};

// The LED PWM pins
// Actually drives the AMC7135 drivers
const int RED = 10;
const int GREEN = 11;
const int BLUE = 9;
const int WHITE = 6;

// The termistor pins
const int THERM_REF = A5;
const int THERM_HOT = A4;

// Moving average weight to smooth
// the termistor values.
const float alpha = 0.03;

// State variables for the loop
int sensorValue = 0;
float sensor_iit = 0.0;
int outputValue = 0;        // value output to the PWM (analog out)
int wait_time = 100;
double rgbw[4];

// Converts temperature into a brightness
// Note: This isn't purely physical, as
// blackbody brightness goes as T^4
// under the Stefan–Boltzmann law
float temp_to_brightness(float temp) {
  float lin = min((temp/4000.0), 1.0);
  return lin*lin*lin;
}

// Converts the temperature to an RGBW
// value. For intermediate temperatures,
// it interpolates within the lookup table
void temp_to_rgbw(double * rgbw, float temp) {
  int ind = 0;
  int lower[4];
  int upper[4];
  temp = max(min(temp, MAX_TEMP_K), MIN_TEMP_K);
  ind = (int) ((int) temp - MIN_TEMP_K)/TEMP_STEP_K;
  for (int i = 0; i < 4; i++) {
    lower[i] = blackbody[ind][i];
    upper[i] = blackbody[min(ind + 1, BLACKBODY_LOOKUP_SIZE - 1)][i];
  }
  float err = (temp - (float) MIN_TEMP_K)/((float) TEMP_STEP_K) - (float) ind;
  // Interpolate between temperatures
  for (int i = 0; i < 4; i++) {
    rgbw[i] = ((1.0 - err)*lower[i] + err*upper[i])*temp_to_brightness(temp);
  }
}

// Turn off the LED's
void setup() {
  // put your setup code here, to run once:
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  analogWrite(WHITE, 0);
  Serial.begin(9600);
}

void loop() {
  // Measure the difference between the hot and reference
  // thermistor. By making it differential, we allow the
  // device to work in various ambient temperatures
  sensorValue = min(max(analogRead(THERM_HOT) - analogRead(THERM_REF), 0), 35);
  // Smooth the value
  sensor_iit = alpha*sensorValue + (1.0 - alpha)*sensor_iit;
  Serial.println(sensor_iit*250.0);
  // Convert the smoothed "temperature" to
  // an rgbw value
  temp_to_rgbw(rgbw, sensor_iit*250.0);
  analogWrite(RED, rgbw[0]);
  analogWrite(GREEN, rgbw[1]);
  analogWrite(BLUE, rgbw[2]);
  analogWrite(WHITE, rgbw[3]);
  delay(200);
}
