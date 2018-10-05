#include <SD.h>

#include <SPI.h>


#ifndef _VARIANT_MINITRONICS_20_
  #error Please select board minitronics v2.0
#endif

Sd2Card card;
SdVolume volume;
SdFile root;

template <typename T, size_t N>
inline
size_t SizeOfArray( const T(&)[ N ] )
{
  return N;
}



#define CS_SD   4
#define SD_DETECT 20

int leds[] = {6,10,11, 24 /* fan */};
int tempsensors[] = {4,3,2};
int endstops[] = {8,9,4};

struct axis {
 int step;
 int dir;
 int enable;
};

/* step,dir,enable */
axis axes[] = {

{1,3,0}
, {29,28,0}
, {16,17,0}
, {14,15,0}

};

/*{1,3,0}
, {29,28,0}
, {16,17,0}
, {14,15,0}
 */

#define AXISCOUNT 4
#define LEDCOUNT 4
#define LEDDBG 13

// the setup function runs once when you press reset or power the board
void setup() {

  SerialUSB.begin(115200);

  // initialize digital pin 13 as an output.
  pinMode(LEDDBG, OUTPUT);
  digitalWrite(LEDDBG, HIGH);
 // pinMode(SD_DETECT, INPUT_PULLUP);
  
  

  
  for (int i=0; i<AXISCOUNT; i++) {
    pinMode(axes[i].step, OUTPUT);
    pinMode(axes[i].dir, OUTPUT);

  }

  pinMode(0, OUTPUT);
  digitalWrite(0,LOW);

  for (int i=0; i<LEDCOUNT; i++) 
    pinMode(leds[i], OUTPUT);
  
  for (int i = 0; i< SizeOfArray(tempsensors); i++) {
    pinMode(A0 + tempsensors[i],INPUT);
  }

  for (int i = 0; i< SizeOfArray(endstops); i++) {
    pinMode(endstops[i],INPUT_PULLUP);
  }

  pinMode(SS, OUTPUT);     // change this to 53 on a mega
  pinMode(CS_SD, OUTPUT); 
  

  digitalWrite(CS_SD, HIGH); 

  
  
SPI.begin(); 
}

unsigned long prevMilLed = millis();
unsigned long prevMil12V = millis();
unsigned long prevStep = micros();
unsigned long prevDumpTemp = millis();
int ledstate =0;

// the loop function runs over and over again forever
void loop() {
 
  if (millis() - prevMilLed > 2000) {
    
    digitalWrite(LEDDBG, HIGH);
    for (int i=0; i<AXISCOUNT; i++) {

      digitalWrite(axes[i].dir, HIGH);

    }   
    prevMilLed = millis();
  }
  else  if (millis() - prevMilLed > 1000) {
    
    digitalWrite(LEDDBG, LOW);
    
    for (int i=0; i<AXISCOUNT; i++) {

      digitalWrite(axes[i].dir, LOW);
  
    }   
    

    
  }
  
  if (millis() - prevDumpTemp >500) {
    
    String temps = "AD values - ";
    for (int i = 0; i< SizeOfArray(tempsensors); i++) {
    int t = analogRead(tempsensors[i]);
    temps.concat(" T");
    temps.concat(i);
    temps.concat(": ");
    temps.concat(t);
    }
    SerialUSB.println(temps);

    temps = "Endstops - ";
    for (int i = 0; i< SizeOfArray(endstops); i++) {
    int t = digitalRead(endstops[i]);
    temps.concat(" S");
    temps.concat(i);
    temps.concat(": ");
    temps.concat(t);
    }
    SerialUSB.println(temps);

   prevDumpTemp =  millis();
   
  }
  
  if (millis() - prevMil12V >300) {
    
      for (int i=0; i<LEDCOUNT; i++)   
        digitalWrite(leds[i], LOW);
        
      digitalWrite(leds[ledstate], HIGH);
   
   
      ledstate++;
      if (ledstate==LEDCOUNT)
        ledstate=0;
      prevMil12V = millis();
  }
  
  if (micros()-prevStep > 1600){

     for (int i=0; i<AXISCOUNT; i++) {
      digitalWrite(axes[i].step, HIGH);
    }   
    delayMicroseconds(10);
    for (int i=0; i<AXISCOUNT; i++) {
      digitalWrite(axes[i].step, LOW);
    }  
    prevStep = micros();
  }
  
  
  
}
