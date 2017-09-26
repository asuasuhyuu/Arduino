/* David Wang
 * Code that takes audio input from a 3.5mm cable
 * and flashes an LED strip based on the frequency 
 * of the music.
 *
 * HUGE thanks to the arduino community
 * If you see your code here, I owe you my gratitude
 *
 */

int analogPin = 0; // MSGEQ7 OUT
int strobePin = 2; // MSGEQ7 STROBE
int resetPin = 4; // MSGEQ7 RESET
int analogPin2 = 1; //MSGEQ7 (2) OUT
int strobePin2 = 1; //MSGEQ7 (2) STROBE
int resetPin2 = 7; //MSGEQ7 (2) RESET 
int spectrumValue[7*2];
 
// MSGEQ7 OUT pin produces values around 50-80
// when there is no input, so use this value to
// filter out a lot of the chaff.
int filterValue = 100;
 
// LED pins connected to the PWM pins on the Arduino

int ledPinR = 10;
int ledPinG = 9;
int ledPinB = 11;
int ledPinR2 = 5;
int ledPinG2 = 6; 
int ledPinB2 = 3; 
 
void setup()
{
  Serial.begin(9600);
  // Read from MSGEQ7 OUT
  pinMode(analogPin, INPUT);
  pinMode(analogPin2, INPUT);
  // Write to MSGEQ7 STROBE and RESET
  pinMode(strobePin, OUTPUT);
  pinMode(strobePin2, OUTPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(resetPin2, OUTPUT);
 
  // Set analogPin's reference voltage
  analogReference(DEFAULT); // 5V
 
  // Set startup values for pins
  digitalWrite(resetPin, LOW);
  digitalWrite(resetPin2, LOW);
  digitalWrite(strobePin, HIGH);
  digitalWrite(strobePin2, HIGH);
 analogWrite(ledPinR,0); 
  analogWrite(ledPinG, 0); 
  analogWrite(ledPinB, 0);
  analogWrite(ledPinR2,0); 
  analogWrite(ledPinG2, 0); 
  analogWrite(ledPinB2, 0); 
  }
 
void loop()
{
  // Set reset pin low to enable strobe
  //digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  digitalWrite(resetPin2, LOW);
 
  // Get all 7 spectrum values from the MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    digitalWrite(strobePin2, LOW);
    delayMicroseconds(30); // Allow output to settle
 
    spectrumValue[i] = analogRead(analogPin);
    spectrumValue[i+7] = analogRead(analogPin2);
 
    // Constrain any value above 1023 or below filterValue
    spectrumValue[i] = constrain(spectrumValue[i], filterValue, 1023);
    spectrumValue[i+7] = constrain(spectrumValue[i+7], filterValue, 1023);

 
    // Remap the value to a number between 0 and 255
    spectrumValue[i] = map(spectrumValue[i], filterValue, 1023, 0, 255);
    spectrumValue[i+7] = map(spectrumValue[i+7], filterValue, 1023, 0, 255);

    // Remove serial stuff after debugging
    //Serial.print(spectrumValue[i]);
    //Serial.print(" ");
    //Serial.print(spectrumValue[i+7]);
    //Serial.print(" ");
    digitalWrite(strobePin2, HIGH);
    digitalWrite(strobePin, HIGH);
   }
   for (int i = 0; i < 14; i++)
  {
    Serial.print(spectrumValue[i]);
    Serial.print(" ");
    if (i == 6)    Serial.print(", ");
  }
   Serial.println();
 
   // Write the PWM values to the LEDs
   // I find that with three LEDs, these three spectrum values work the best
//                     max / intensity / luminosity
/*  
  analogWrite(ledPinR, 1023/    16     /    2); // intensity=63 
  analogWrite(ledPinG, 1023/    16     /    4); 
  analogWrite(ledPinB, 1023/    16           ); 
  analogWrite(ledPinR2, spectrumValue [7+1]/12); 
  analogWrite(ledPinG2, spectrumValue [7+4]/12);
  analogWrite(ledPinB2, spectrumValue [7+6]/12); 
*/

  analogWrite(ledPinR, spectrumValue[1]/12); 
  analogWrite(ledPinG, spectrumValue[4]/12); 
  analogWrite(ledPinB, spectrumValue[5]/12); 
  analogWrite(ledPinR2, spectrumValue [7+1]/12); 
  analogWrite(ledPinG2, spectrumValue [7+4]/12);
  analogWrite(ledPinB2, spectrumValue [7+5]/12); 

}


