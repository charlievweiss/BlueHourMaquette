#include"LPD8806.h" // for our special old leds
#include "SPI.h"

//led variables
int dataPin = 4;
int clockPin = 3;
unsigned long timer;
int colors[15] = {0,0,127,0,0,127,0,0,127,0,0,127,0,0,127}; // array for light values (only 10 leds max, 2 per chip)
LPD8806 strip = LPD8806(10, dataPin, clockPin);

//motion variables
int motionPin = 1; //input pin for photoresistors
int val = 0; //variable for motionPin input status
bool motion = false;
int maxVal = 0;//

//variables to detect number of totes
int analogPin = 0; //input for tote resistors
int raw=0; //raw input value
int Vin = 5; //power with 5 volts
float Vout=0; //voltage value measured
int numberTotes=1; //default value


void setup() {
  pinMode(motionPin,INPUT); //declare sensor as input
  //check resistance to find number of totes
  raw=analogRead(analogPin);
  if(raw){
    Vout=(raw*Vin)/1023.0; //the voltage is mapped from 0-1023 for some reason, so divide by 1023 here.
    numberTotes = (Vin/(Vin-Vout))+.15; //.15 is to round up with int
  }
  int i;
  Serial.begin(9600);
  strip.begin();
  // Update strip, to start they are all 'blue'
  for (i=0; i<numberTotes*2; i++) {
    strip.setPixelColor(i,0,0,127);
  }
  strip.show();
}

void loop() {
  timer = millis(); // track time
  //continuously check number of totes
  raw=analogRead(analogPin);
  if(raw){
    Vout=(raw*Vin)/1023.0;
    numberTotes = (Vin/(Vin-Vout))+.15; //.15 is to round up with int
  }
  else{
    numberTotes=1;
  }
  //setup for check for motion
  val = analogRead(motionPin); //read input val from photoresistors
  //check for the new max val, since this correlates to the max light in the room
  if (val>maxVal){
    maxVal=val;
  }
  //check for motion
  if (val<maxVal-(maxVal/8)){ //if the pin reads a light value lower than the max (meaning something is in front of a photoresistor)
    //motion started
    motion = true;
  }
  else{
    //motion ended
    motion = false;
  }
  //set leds to correct color
  int i;
  for (i=0;i<numberTotes;i++){
    checkAndFade(timer,i);
    strip.setPixelColor(i*2,colors[i*3],colors[(i*3)+1],colors[(i*3)+2]);
    strip.setPixelColor((i*2)+1,colors[i*3],colors[(i*3)+1],colors[(i*3)+2]);
  }
  //turn off if tote exceeds numberTotes (which is the number of active totes)
  for (i=numberTotes;i<5;i++){
    strip.setPixelColor(i*2,0,0,0);
    strip.setPixelColor((i*2)+1,0,0,0);
  }
  strip.show();
}

void checkAndFade(int timer,int tote){
  //checks for motion and updates the leds accordingly
  if (timer%10==0){
    if ((tote==0 or colors[(tote-1)*3]>=127) and motion==true){//if the previous tote is yellow or red
     fadeUp(colors,tote*3);
    }
    //fadeDown
    if ((tote==numberTotes or colors[(tote+1)*3]==0) and motion==false){//if the next tote is blue
      fadeDown(colors,tote*3);
    }
  }
}

void fadeUp(int *colors, int shift){
  //check current color
  int red = colors[shift];
  int green = colors[1+shift];
  int blue = colors[2+shift];
  if (blue>0 and red<127){//it's still in the blue phase
    //fade to yellow
    red++;
    green++;
    blue--;
  }
  if (red==127 and green>0){//in the yellow phase (if green==0, stay red)
    //fade to red
    red=127;
    green--;
    blue=0;
  }
  int rgb[3]={red,green,blue};
  int i;
  for (i=0;i<3;i++){
    colors[i+shift]=rgb[i];
  }
  //return new colors array
  return *colors;
}

void fadeDown(int *colors, int shift){
  //check current color
  int red = colors[shift];
  int green = colors[1+shift];
  int blue = colors[2+shift];
  if (red==127 && green<127){//it's still in the red->yellow phase
    //fade to yellow
    green++;
    blue=0;
  }
  if (red==green && blue<127){//in the yellow->blue phase
    //fade to blue
    red--;
    green--;
    blue++;
  }
  int rgb[3]={red,green,blue};
  int i;
  for (i=0;i<3;i++){
    colors[i+shift]=rgb[i];
  }
  //return new colors array
  return *colors;
}
