// RFM69HCW Example Sketch
// Send serial input characters from one RFM69 node to another
// Based on RFM69 library sample code by Felix Rusu
// http://LowPowerLab.com/contact
// Modified for RFM69HCW by Mike Grusin, 4/16

// This sketch will show you the basics of using an
// RFM69HCW radio module. SparkFun's part numbers are:
// 915MHz: https://www.sparkfun.com/products/12775
// 434MHz: https://www.sparkfun.com/products/12823

// See the hook-up guide for wiring instructions:
// https://learn.sparkfun.com/tutorials/rfm69hcw-hookup-guide

// Uses the RFM69 library by Felix Rusu, LowPowerLab.com
// Original library: https://www.github.com/lowpowerlab/rfm69
// SparkFun repository: https://github.com/sparkfun/RFM69HCW_Breakout

// Include the RFM69 and SPI libraries:

#include <RFM69.h>
#include <SPI.h>
#include <SPIFlash.h>

// Addresses for this node. CHANGE THESE FOR EACH NODE!

#define NETWORKID     0   // Must be the same for all nodes
#define MYNODEID      1   // My node ID
#define TONODEID      2   // Destination node ID

// RFM69 frequency, uncomment the frequency of your module:

//#define FREQUENCY   RF69_433MHZ
#define FREQUENCY     RF69_915MHZ

// AES encryption (or not):

#define ENCRYPT       false // Set to "true" to use encryption
#define ENCRYPTKEY    "TOPSECRETPASSWRD" // Use the same 16-byte key on all nodes

// Use ACKnowledge when sending messages (or not):

#define USEACK        true // Request ACKs or not

// Packet sent/received indicator LED (optional):

#define LED           9 // LED positive pin
#define GND           8 // LED ground pin

// Create a library object for our RFM69HCW module:

RFM69 radio;

int flexInVal = 0;
char buff[50];
int flexHPeak = 0;
int flexLPeak = 1023;

// HEARTRATE SENSOR SECTION:
#include "Arduino.h"

//Using built in LED pin for demo
#define ledPin 7
#define ledPin2 6

// Pulse meter connected to any Analog pin
#define sensorPin A0

// Values from provided (eBay) code
float alpha = 0.75;
int period = 50;
float max = 0.0;
int sendDelay = 0;

int flexAv = 0;

void setup()
{
  // Inbuilt LED
  pinMode(ledPin, OUTPUT);

  // Open a serial port so we can send keystrokes to the module:

  Serial.begin(9600);
  Serial.print("Node ");
  Serial.print(MYNODEID,DEC);
  Serial.println(" ready");  

  // Set up the indicator LED (optional):

  //pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  pinMode(GND,OUTPUT);
  digitalWrite(GND,LOW);

  // Initialize the RFM69HCW:

  radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
  radio.setHighPower(); // Always use this for RFM69HCW

  // Turn on encryption if desired:

  if (ENCRYPT)
    radio.encrypt(ENCRYPTKEY);
}

void loop()
{
  // Set up a "buffer" for characters that we'll send:

  static char sendbuffer[62];
  static int sendlength = 0;

  // SENDING

  // In this section, we'll gather serial characters and
  // send them to the other node if we (1) get a carriage return,
  // or (2) the buffer is full (61 characters).

  // If there is any serial input, add it to the buffer:

  if (Serial.available() > 0)
  {
    char input = Serial.read();

    if (input != '\r') // not a carriage return
    {
      sendbuffer[sendlength] = input;
      sendlength++;
    }

    // If the input is a carriage return, or the buffer is full:

    if ((input == '\r') || (sendlength == 61)) // CR or buffer full
    {
      // Send the packet!


      Serial.print("sending to node ");
      Serial.print(TONODEID, DEC);
      Serial.print(", message [");
      for (byte i = 0; i < sendlength; i++)
        Serial.print(sendbuffer[i]);
      Serial.println("]");

      // There are two ways to send packets. If you want
      // acknowledgements, use sendWithRetry():

      if (USEACK)
      {
        if (radio.sendWithRetry(TONODEID, sendbuffer, sendlength))
          Serial.println("ACK received!");
        else
          Serial.println("no ACK received");
      }

      // If you don't need acknowledgements, just use send():

      else // don't use ACK
      {
        radio.send(TONODEID, sendbuffer, sendlength);

      }

      sendlength = 0; // reset the packet
      Blink(LED,10);
    }
  }

  // RECEIVING

  // In this section, we'll check with the RFM69HCW to see
  // if it has received any packets:

  if (radio.receiveDone()) // Got one!
  {
    // Print out the information:

    Serial.print("received from node ");
    Serial.print(radio.SENDERID, DEC);
    Serial.print(", message [");

    // The actual message is contained in the DATA array,
    // and is DATALEN bytes in size:

    for (byte i = 0; i < radio.DATALEN; i++){
        Serial.print((char)radio.DATA[i]);
        buff[i]=(char)radio.DATA[i];
    }


    // RSSI is the "Receive Signal Strength Indicator",
    // smaller numbers mean higher power.

    Serial.print("], RSSI ");
    Serial.println(radio.RSSI);

    // Send an ACK if requested.
    // (You don't need this code if you're not using ACKs.)

    if (radio.ACKRequested())
    {
      radio.sendACK();
      Serial.println("ACK sent");
    }
    Blink(LED,10);
  }

  flexInVal = atoi(buff);
  //Serial.println(flexInVal);
  if(flexInVal > flexHPeak && flexInVal < 1000){
    flexHPeak = flexInVal;
  }
  if(flexInVal < flexLPeak && flexInVal >50){
    flexLPeak = flexInVal;
  }
  //Serial.println(strtch);
  int flexM = map(flexInVal,flexLPeak,flexHPeak,255,0);
  int flexC = constrain(flexM,0,255);
    
  //Serial.println(flexC);
  //Serial.println(flexLPeak);
  analogWrite(ledPin2, flexC);
  analogWrite(5,170);

  // HB SENSOR SECTION:

  // Arbitrary initial value for the sensor value (0 - 1023)
  // too large and it takes a few seconds to 'lock on' to pulse
  static float oldValue = 500;

  // Time recording for BPM (beats per minute)
  static unsigned long bpmMills = millis();
  static int bpm = 0;

  // Keep track of when we had the the last pulse - ignore
  // further pulses if too soon (probably false reading)
  static unsigned long timeBetweenBeats = millis();
  int minDelayBetweenBeats = 400;

  // This is generic code provided with the board:
  // Read the sensor value (0 - 1023)
  int rawValue = analogRead((unsigned char) sensorPin);

  // Some maths (USA: math) to determine whether we are detected a peak (pulse)
  float value = alpha * oldValue + (1 - alpha) * rawValue;
  float change = value - oldValue;
  oldValue = value;

  // Forum suggested improvement (works very well)
  // Display data on the LED via a blip:
  // Empirically, if we detect a peak as being X% from
  // absolute max, we find the pulse even when amplitude
  // varies on the low side.

  // if we find a new maximum value AND we haven't had a pulse lately
  if ((change >= max) && (millis() > timeBetweenBeats + minDelayBetweenBeats)) {

    // Reset max every time we find a new peak
    max = change;

    // Flash LED and beep the buzzer
    //digitalWrite(ledPin, 1);
    //tone(3, 2000, 50);
    char MSG []= "L";
    radio.send(TONODEID, MSG, 1);

    // Reset the heart beat time values
    timeBetweenBeats = millis();
    bpm++;
  }
  else {
    // No pulse detected, ensure LED is off (may be off already)
    digitalWrite(ledPin, 0);
  }
  // Slowly decay max for when sensor is moved around
  // but decay must be slower than time needed to hit
  // next pulse peak. Originally: 0.98
  max = max * 0.97;

  // Every 15 seconds extrapolate the pulse rate. Improvement would
  // be to average out BPM over last 60 seconds
  if (millis() >= bpmMills + 15000) {
    //Serial.print("BPM (approx): ");
    //Serial.println(bpm * 4);
    bpm = 0;
    bpmMills = millis();
  }

  // Must delay here to give the value a chance to decay
  delay(5);

  int strtch = analogRead(A5);
  if(sendDelay < 32){
    flexAv += strtch;
    sendDelay++;
  }
  if(sendDelay ==32){
    Serial.println(flexAv/32);
    char buff[50];
    byte sendSize = strlen(buff);
    sprintf(buff,"%d",flexAv/32);
    radio.send(TONODEID, buff, sendSize);
    sendDelay = 0;
    flexAv=0;
  }
  
}

void Blink(byte PIN, int DELAY_MS)
// Blink an LED for a given number of ms
{
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}
