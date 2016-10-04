static double blackbody[37][4] = {
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

const int RED = 10;
const int GREEN = 11;
const int BLUE = 9;
const int WHITE = 6;

const int THERM_REF = A5;
const int THERM_HOT = A4;

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int wait_time = 100;

/*
def temp_to_blackbody(temp) {
    if (nm <= 1000):
        return CIE_COLOR_MATCH[0]
    ind = (int(nm) - 380)/5
    if ind >= len(CIE_COLOR_MATCH) - 1:
        return CIE_COLOR_MATCH[-1]
    err = (float(nm) - 380.0)/5.0 - ind
}
*/

void setup() {
  // put your setup code here, to run once:
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  analogWrite(WHITE, 0);
  Serial.begin(9600);
}

void loop() {
  sensorValue = min(max(analogRead(THERM_HOT) - analogRead(THERM_REF), 0), 35);
  analogWrite(RED, blackbody[sensorValue][0]);
  analogWrite(GREEN, blackbody[sensorValue][1]);
  analogWrite(BLUE, blackbody[sensorValue][2]);
  analogWrite(WHITE, blackbody[sensorValue][3]);
  delay(200);
  /*
  sensorValue = max((analogRead(THERM_HOT) - analogRead(THERM_REF) - 3)*40, 3);
  Serial.println(sensorValue);
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  analogWrite(WHITE, 0);
  analogWrite(RED, outputValue);
  delay(wait_time);
  analogWrite(RED, 0);
  analogWrite(GREEN, outputValue);
  delay(wait_time);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, outputValue);
  delay(wait_time);
  analogWrite(BLUE, 0);
  analogWrite(WHITE, outputValue);
  delay(wait_time);
  analogWrite(RED, outputValue);
  analogWrite(GREEN, outputValue);
  analogWrite(BLUE, outputValue);
  analogWrite(WHITE, outputValue);
  delay(wait_time);
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  analogWrite(WHITE, 0);
  */
}
