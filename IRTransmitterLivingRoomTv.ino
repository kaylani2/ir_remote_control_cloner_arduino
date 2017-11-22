/*
 * Update: 16/11/2017
 * Author: Kaylani Bochie
 * 
 * Yeah. Don't use strings with hex values instead of actual hex values, unless you wanna have a bad time.
 * The output on Nano is absolutely reliable with the protocols NEC (1), SONY (2) and NECX (7) !!!
 * Just ignore the 15/11/2017 update.
 */
 

/*
 * Update: 15/11/2017
 * Author: Kaylani Bochie
 * 
 * The output produced on Nano is reliable with the protocols NEC (1) and SONY (2).
 * The output produced on Nano IS NOT reliable with the protocol NECX (7) -> It doesn't seem to send the entire code
 * 
 */

/*
 * Coder: Kaylani Bochie
 * Board: Arduino Nano
 * 
 * TV: Samsung UN40H5100AG
 * 
 * Circuit:
 * 
 * Transmitter:
 *   Digital Pin 3 -> One terminal of the 330 ohm resistor
 *   Other terminal of the 330 ohm resistor -> Base of the BC546 npn transistor
 *   Arduino 5V -> One terminal of the 56 ohm resistor
 *   Other terminal of the 56 ohm resistor -> Anode of the IR Led
 *   Cathode of the IR Led -> Colector of the BC546 npn transistor
 *   Arduino Gnd -> Emitter of the BC546 npn transistor
 * 
 * 
 * Receiver: (Optional for the IR controller)
 *  Digital Pin 2 -> Signal Out of the TSOP1838 IR Receiver
 *  Arduino 5V -> Vcc of the TSOP1838 IR Receiver
 *  Arduino Gnd -> Gnd of the TSOP1838 IR Receiver
 * 
 * Code selector: (We'll use a five channel dip switch for this)
 *  Digital Pin 10 -> One terminal of the switch's first channel
 *  Other terminal of the switch's first channel -> Arduino Gnd
 *  Digital Pin 9 -> One terminal of the switch's second channel
 *  Other terminal of the switch's second channel -> Arduino Gnd
 *  Digital Pin 8 -> One terminal of the switch's third channel
 *  Other terminal of the switch's third channel -> Arduino Gnd
 *  Digital Pin 7 -> One terminal of the switch's fourth channel
 *  Other terminal of the switch's fourth channel -> Arduino Gnd
 *  Digital Pin 6 -> One terminal of the switch's fifth channel
 *  Other terminal of the switch's fifth channel -> Arduino Gnd
 *  
 *  Digital Pin 12 -> One terminal of the tactile switch
 *  Other terminal of the tactile switch -> Arduino Gnd
 *  
 *  
 */

/*
 * Instructions: https://learn.adafruit.com/using-an-infrared-library/overview
 */


#include "IRLibAll.h"
#define RECEIVER_PIN                                      2

#define OK                                                0
#define UNKNOWN_DIP_STRING_ERROR                          1
#define DIP_STRING_LENGTH                                 6
/* Five channel dip switch + EOS */

#define TRIGGER_PIN                                       12

/* We'll use a five channel dip switch to select which code will be sent to the TV */
/* FIRST_PIN is the most significant bit (MSB) */
#define FIRST_PIN                                         10
#define SECOND_PIN                                        9
#define THIRD_PIN                                         8
#define FOURTH_PIN                                        7
#define FIFTH_PIN                                         6
/* FIFTH_PIN is the least significant bit (LSB) */

#define ON_BUTTON_STRING                                  "00000"
#define SOURCE_BUTTON_STRING                              "00001"
#define PLUS_VOLUME_STRING                                "00010"
#define MINUS_VOLUME_STRING                               "00011"
#define MUTE_BUTTON_STRING                                "00100"
#define PLUS_CHANNEL_STRING                               "00101"
#define MINUS_CHANNEL_STRING                              "00110"
#define CHANNEL_1_STRING                                  "00111"
#define CHANNEL_2_STRING                                  "01000"
#define CHANNEL_3_STRING                                  "01001"
#define CHANNEL_4_STRING                                  "01010"
#define CHANNEL_5_STRING                                  "01011"
#define CHANNEL_6_STRING                                  "01100"
#define CHANNEL_7_STRING                                  "01101"
#define CHANNEL_8_STRING                                  "01110"
#define CHANNEL_9_STRING                                  "01111"
#define CHANNEL_0_STRING                                  "10000"
#define RETURN_BUTTON_STRING                              "10001"
#define EXIT_BUTTON_STRING                                "10010"


/* These are the codes used by my living room tv -> Samsung Model UN40H5100AG */
/* Update: DO NOT USE STRINGS!!! The .send () method accepts the strings but doesn't work properly */
#define ON_BUTTON                                         0xE0E040BF
#define SOURCE_BUTTON                                     0xE0E0807F
#define PLUS_VOLUME                                       0xE0E0E01F
#define MINUS_VOLUME                                      0xE0E0D02F
#define MUTE_BUTTON                                       0xE0E0F00F
#define PLUS_CHANNEL                                      0xE0E048B7
#define MINUS_CHANNEL                                     0xE0E008F7
#define CHANNEL_1                                         0xE0E020DF
#define CHANNEL_2                                         0xE0E0A05F
#define CHANNEL_3                                         0xE0E0609F
#define CHANNEL_4                                         0xE0E010EF
#define CHANNEL_5                                         0xE0E0906F
#define CHANNEL_6                                         0xE0E050AF
#define CHANNEL_7                                         0xE0E030CF
#define CHANNEL_8                                         0xE0E0B04F
#define CHANNEL_9                                         0xE0E0708F
#define CHANNEL_0                                         0xE0E08877
#define RETURN_BUTTON                                     0xE0E01AE5
#define EXIT_BUTTON                                       0xE0E0B44B

/* NECX Protocol: */
#define NUMBER_OF_BITS                                    32
 
//Create a receiver object to listen on pin RECEIVER_PIN
IRrecvPCI myReceiver (RECEIVER_PIN);

//Create a sender object
IRsend mySender;
 
//Create a decoder object 
IRdecode myDecoder;   

int ConstructDipString (char *dipString, int firstPin, int secondPin, int thirdPin, int fourthPin, int fifthPin)
{
  /*  firstPin is the most significant bit    (MSB)
   *  fifthPin is the least significant bit   (LSB)
   */
  
  strcpy (dipString, "");
  
  if (digitalRead (firstPin) == LOW)
    strcat (dipString, "0");
  else
    strcat (dipString, "1");
  
  if (digitalRead (secondPin) == LOW)
    strcat (dipString, "0");
  else
    strcat (dipString, "1");
  
  if (digitalRead (thirdPin) == LOW)
    strcat (dipString, "0");
  else
    strcat (dipString, "1");

  if (digitalRead (fourthPin) == LOW)
    strcat (dipString, "0");
  else
    strcat (dipString, "1");
  
  if (digitalRead (fifthPin) == LOW)
    strcat (dipString, "0");
  else
    strcat (dipString, "1");

  return OK;
}

int SendCode (const char *dipString)
{
  if (strcmp (dipString, ON_BUTTON_STRING) == 0)
  {
    mySender.send (NECX, ON_BUTTON, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, SOURCE_BUTTON_STRING) == 0)
  {
    mySender.send (NECX, SOURCE_BUTTON, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }
  if (strcmp (dipString, PLUS_VOLUME_STRING) == 0)
  {
    mySender.send (NECX, PLUS_VOLUME, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, MINUS_VOLUME_STRING) == 0)
  {
    mySender.send (NECX, MINUS_VOLUME, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, MUTE_BUTTON_STRING) == 0)
  {
    mySender.send (NECX, MUTE_BUTTON, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, PLUS_CHANNEL_STRING) == 0)
  {
    mySender.send (NECX, PLUS_CHANNEL, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, MINUS_CHANNEL_STRING) == 0)
  {
    mySender.send (NECX, MINUS_CHANNEL, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_1_STRING) == 0)
  {
    mySender.send (NECX, CHANNEL_1, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_2_STRING) == 0)
  {
    mySender.send (NECX, CHANNEL_2, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_3_STRING) == 0)
  {
    mySender.send (NECX, CHANNEL_3, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_4_STRING) == 0)
  {
    mySender.send (NECX, CHANNEL_4, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_5_STRING) == 0)
  {
    mySender.send (NECX, CHANNEL_5, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_6_STRING) == 0)
  {
    mySender.send (NECX, CHANNEL_6, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_7_STRING) == 0)
  {
    mySender.send (NECX, CHANNEL_7, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_8_STRING) == 0)
  {
    mySender.send (NECX, CHANNEL_8, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_9_STRING) == 0)
  {
    mySender.send (NECX, CHANNEL_9, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_0_STRING) == 0)
  {
    mySender.send (NECX, CHANNEL_0, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));   
    return OK;
  }

  if (strcmp (dipString, RETURN_BUTTON_STRING) == 0)
  {
    mySender.send (NECX, RETURN_BUTTON, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, EXIT_BUTTON_STRING) == 0)
  {
    mySender.send (NECX, EXIT_BUTTON, NUMBER_OF_BITS);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  return UNKNOWN_DIP_STRING_ERROR;
}

void setup()
{
  Serial.begin (9600);
  delay (1001);
  myReceiver.enableIRIn (); // Start the receiver
  Serial.println ("Ready to send IR signals");
  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite (LED_BUILTIN, LOW);

  pinMode (FIRST_PIN, INPUT_PULLUP);
  pinMode (SECOND_PIN, INPUT_PULLUP);
  pinMode (THIRD_PIN, INPUT_PULLUP);
  pinMode (FOURTH_PIN, INPUT_PULLUP);
  pinMode (FIFTH_PIN, INPUT_PULLUP);
  pinMode (TRIGGER_PIN, INPUT_PULLUP);
}
 
void loop()
{
  char dipString [DIP_STRING_LENGTH] = "";
  /* We'll use this string to select which code will be sent to the TV */

  if (digitalRead (TRIGGER_PIN) == LOW)
  {
    ConstructDipString (dipString, FIRST_PIN, SECOND_PIN, THIRD_PIN, FOURTH_PIN, FIFTH_PIN);
    SendCode (dipString);  
    delay (1002);
  }

//Uncoment the block below in order to use the receiver circuit and get the values from the controller you`re trying to clone
/*
  //Continue looping until you get a complete signal received
  if (myReceiver.getResults ())
  {
    myDecoder.decode ();           //Decode it
    myDecoder.dumpResults (true);  //Now print results. Use false for less detail
    myReceiver.enableIRIn ();      //Restart receiver
  }
*/
}
