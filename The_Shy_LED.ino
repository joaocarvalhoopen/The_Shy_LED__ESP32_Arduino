
/*

The shy LED   -    O Led envergonhado :-D

Author:  Joao Nuno Carvalho
Date:    2019-07-30
Email:   joaonunocarv@gmail.com
License: MIT OpenSource license.

Schematic components:

Pin 12 -> 220 Ohms -> NODE_1
NODE 1 -> LED -> NODE_2 -> Pin 25 Virtual Ground
Pin 14 ADC -> NODE_1
Pin 27 Half Power Offset -> 220 Ohms -> 220 Ohms -> NODE_3 -> 100 Ohms -> GND
NODE_2 -> NODE_03

*/


const int LEDOffsetVoltWhileShiningFlashLight = 130; // Relative value for any LED color. // 1700 Absolute value for green LED.

const int ledPinPower           = 12;
const int ledPinVirtualGround   = 25;
const int ledPinHalfPowerOffset = 27;
const int ledPinADCAnalogIn     = 14;

int initial_value = 0;
int curr_value    = 0;

int counter = 0;

const int bufferSize = 20;
int last[bufferSize]; // = {0, 0, 0, 0, 0};

int measureLEDVoltage(){
	int valueAccumulator = 0;

  // Write led power pin as LOW.
  digitalWrite(ledPinPower, LOW);
  delay(1);
  
	// Config led power pin as input Hi Z (High impedance).
	pinMode(ledPinPower, INPUT);
	
	// Config led virtual ground pin as input Hi Z (High impedance).
	pinMode(ledPinVirtualGround, INPUT);

	// Config led virtual half power offset pin as OUTPUT.
	pinMode(ledPinHalfPowerOffset, OUTPUT);

	// Write led virtual half power offset pin HIGH 3.3V.
	digitalWrite(ledPinHalfPowerOffset, HIGH);

	// Measure LED ADC voltage drop with half high voltage offset....
  delay(10);
	valueAccumulator = 0;
  	for(int i = 0; i < 40; i++){
    	valueAccumulator += analogRead(ledPinADCAnalogIn); 
  	}
  	valueAccumulator /= 40;

	// Config led virtual half power offset pin as INPUT.
	pinMode(ledPinHalfPowerOffset, INPUT);

	// Config led power pin as OUTPUT.
	pinMode(ledPinPower, OUTPUT);

	// Config led virtual ground pin as OUTPUT.
	pinMode(ledPinVirtualGround, OUTPUT);

	// Write led virtual ground pin LOW 0V.	
	digitalWrite(ledPinVirtualGround, LOW);	
	
	return valueAccumulator;
}

void turnLEDOn(){
	digitalWrite(ledPinPower, HIGH);
}

void turnLEDOff(){
	digitalWrite(ledPinPower, LOW);
}

float updateMeanBufferInitialization(float curr_value_par_init){
  for(int i= (bufferSize - 1); i > 0; i--){
    last[i] = curr_value_par_init;
  }
  last[0] = curr_value_par_init;

  float meanValue = 0;
  for(int i=0; i < bufferSize; i++){
    meanValue += last[i];
  }
  meanValue = meanValue / bufferSize;
  
  return meanValue;
}

float updateMeanBuffer(float curr_value_par){
  if (curr_value_par < 1000)
    curr_value_par = 1000;

  /*
  last[2] = last[1];
  last[1] = last[0];
  last[0] = curr_value;
  */

  for(int i= (bufferSize - 1); i > 0; i--){
    last[i] = last[i-1];
  }
  last[0] = curr_value_par;

  float meanValue = 0;
  for(int i=0; i < bufferSize; i++){
    meanValue += last[i];
  }
  meanValue = meanValue / bufferSize;
  
  return meanValue;
}

void setup(){
	Serial.begin(115200);
	delay(5000);

	pinMode(ledPinADCAnalogIn, INPUT);

  // Initialize the mean.
  initial_value = measureLEDVoltage();
  updateMeanBufferInitialization(initial_value);
  // Converge the running mean.
  for(int i=0; i<bufferSize; i++){
    initial_value = measureLEDVoltage();
    initial_value = updateMeanBuffer(initial_value);
  }
  delay(1000);
  
  Serial.println("begin initial_value");
  Serial.println(initial_value);
  Serial.println("end initial_value");
}

void loop(){
  delay(1);
	curr_value = measureLEDVoltage();
	// Serial.println(curr_value);

  float meanValue = updateMeanBuffer(curr_value);  
  Serial.println(meanValue);
 
	// if (curr_value > initial_value + LEDOffsetVoltWhileShiningFlashLight){
  if (meanValue > initial_value + LEDOffsetVoltWhileShiningFlashLight){
	  turnLEDOff();
		// Serial.println("OFF");
	}
	else{
		turnLEDOn();
	}
	// Faster then "eye sample rate", 25Hz.
	delay(20);
}
