/*
 * Update: 24/11/2017
 * Author: Kaylani Bochie
 * 
 * Usual power consumption when using Arduino Nano:
 *  10mA/20mA <-> 5V
 * 
 */

/*
 * Update: 17/11/2017
 * Author: Kaylani Bochie
 * 
 * Since the code is working properly with different protocols, I'll use this sketch to control multiple appliances.
 * Including:
 * Samsung TV - Model UN40H5100AG - NECX (7) Protocol
 * Samsung TV - Model BLABLABLABLA - NEC (1) Protocol
 * A generic Led Strip - Model ????????? - NEC (1) Protocol
 * 
 */

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
 *  Digital Pin 11 -> One terminal of the switch's first channel
 *  Other terminal of the switch's first channel -> Arduino Gnd
 *  Digital Pin 10 -> One terminal of the switch's second channel
 *  Other terminal of the switch's second channel -> Arduino Gnd
 *  Digital Pin 9 -> One terminal of the switch's third channel
 *  Other terminal of the switch's third channel -> Arduino Gnd
 *  Digital Pin 8 -> One terminal of the switch's fourth channel
 *  Other terminal of the switch's fourth channel -> Arduino Gnd
 *  Digital Pin 7 -> One terminal of the switch's fifth channel
 *  Other terminal of the switch's fifth channel -> Arduino Gnd
 *  
 *  Digital Pin 5 -> One terminal of the tactile switch
 *  Other terminal of the tactile switch -> Arduino Gnd
 *  
 *  
 */

/*
 * Instructions: https://learn.adafruit.com/using-an-infrared-library/overview
 */


#include "IRLibAll.h"
#define RECEIVER_PIN                                                  2

#define OK                                                            0
#define UNKNOWN_DIP_STRING_ERROR                                      1
#define INVALID_DIP_STRING_ERROR                                      2
#define DIP_STRING_LENGTH                                             6
/* Five channel dip switch + EOS */

#define TRIGGER_PIN                                                   5

/* We'll use a five channel dip switch to select which code will be sent to the TV */
/* FIRST_PIN is the most significant bit (MSB) */
#define FIRST_PIN                                                     11
#define SECOND_PIN                                                    10
#define THIRD_PIN                                                     9
#define FOURTH_PIN                                                    8
#define FIFTH_PIN                                                     7
/* FIFTH_PIN is the least significant bit (LSB) */

#define ON_BUTTON_UN40H5100AG_STRING_UN40H5100AG                      "00000"
#define SOURCE_BUTTON_UN40H5100AG_STRING_UN40H5100AG                  "00001"
#define PLUS_VOLUME_STRING_UN40H5100AG                                "00010"
#define MINUS_VOLUME_STRING_UN40H5100AG                               "00011"
#define MUTE_BUTTON_UN40H5100AG_STRING_UN40H5100AG                    "00100"
#define PLUS_CHANNEL_STRING_UN40H5100AG                               "00101"
#define MINUS_CHANNEL_STRING_UN40H5100AG                              "00110"
#define CHANNEL_1_STRING_UN40H5100AG                                  "00111"
#define CHANNEL_2_STRING_UN40H5100AG                                  "01000"
#define CHANNEL_3_STRING_UN40H5100AG                                  "01001"
#define CHANNEL_4_STRING_UN40H5100AG                                  "01010"
#define CHANNEL_5_STRING_UN40H5100AG                                  "01011"
#define CHANNEL_6_STRING_UN40H5100AG                                  "01100"
#define CHANNEL_7_STRING_UN40H5100AG                                  "01101"
#define CHANNEL_8_STRING_UN40H5100AG                                  "01110"
#define CHANNEL_9_STRING_UN40H5100AG                                  "01111"
#define CHANNEL_0_STRING_UN40H5100AG                                  "10000"
#define RETURN_BUTTON_UN40H5100AG_STRING_UN40H5100AG                  "10001"
#define EXIT_BUTTON_UN40H5100AG_STRING_UN40H5100AG                    "10010"

#define TEST_BUTTON_STRING                                            "11111"

/* These are the codes used by my living room tv -> Samsung Model UN40H5100AG */
/* Update: DO NOT USE STRINGS!!! The .send () method accepts the strings but doesn't work properly */
#define ON_BUTTON_UN40H5100AG                                         0xE0E040BF
#define SOURCE_BUTTON_UN40H5100AG                                     0xE0E0807F
#define PLUS_VOLUME_UN40H5100AG                                       0xE0E0E01F
#define MINUS_VOLUME_UN40H5100AG                                      0xE0E0D02F
#define MUTE_BUTTON_UN40H5100AG                                       0xE0E0F00F
#define PLUS_CHANNEL_UN40H5100AG                                      0xE0E048B7
#define MINUS_CHANNEL_UN40H5100AG                                     0xE0E008F7
#define CHANNEL_1_UN40H5100AG                                         0xE0E020DF
#define CHANNEL_2_UN40H5100AG                                         0xE0E0A05F
#define CHANNEL_3_UN40H5100AG                                         0xE0E0609F
#define CHANNEL_4_UN40H5100AG                                         0xE0E010EF
#define CHANNEL_5_UN40H5100AG                                         0xE0E0906F
#define CHANNEL_6_UN40H5100AG                                         0xE0E050AF
#define CHANNEL_7_UN40H5100AG                                         0xE0E030CF
#define CHANNEL_8_UN40H5100AG                                         0xE0E0B04F
#define CHANNEL_9_UN40H5100AG                                         0xE0E0708F
#define CHANNEL_0_UN40H5100AG                                         0xE0E08877
#define RETURN_BUTTON_UN40H5100AG                                     0xE0E01AE5
#define EXIT_BUTTON_UN40H5100AG                                       0xE0E0B44B
#define TEST_BUTTON                                                   0x89ABCDEF

/* NECX Protocol: */
#define NUMBER_OF_BITS_NECX                                           32
 
//Create a receiver object to listen on pin RECEIVER_PIN
IRrecvPCI myReceiver (RECEIVER_PIN);

//Create a sender object
IRsend mySender;
 
//Create a decoder object 
IRdecode myDecoder;   

/*
 * int ConstructDipString (char *, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
 * 
 * Arguments:
 * char * - Reference of where this functions will save the corresponding read value (O)
 * unsigned char - Leftmost pin of the dip switch. It's the most significant bit (I)
 * unsigned char - Second pin from the left (I)
 * unsigned char - Third pin from the left (I)
 * unsigned char - Fourth pin form the left (I)
 * unsigned char - Rightmost pin of the dip switch. It's the least significant bit (I)
 * 
 * Return values:
 * OK - The function was executed correctly.
 * 
 * Description:
 * This functions reads the value of five given pins and outputs the corresponding value with its first argument.
 * A high value (5V) corresponds to a 1 on the output string.
 * A low value (0V) corresponds to a 0 on the output string.
 * The function was written to be used with a 5 channel dip switch.
 * 
 */
int ConstructDipString (char *dipString, unsigned char firstPin, unsigned char secondPin, unsigned char thirdPin, unsigned char fourthPin, unsigned char fifthPin)
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

/*
 * int SendCode (const char *);
 * 
 * Arguments:
 * const char * - The string that we'll be used to select the code that will be sent by this function (I)
 * 
 * Return values:
 * OK - The function was executed correctly.
 * INVALID_DIP_STRING_ERROR - The function received an invalid string code;=,
 * UNKNOWN_DIP_STRING_ERROR - The function received an unknown string code.
 * 
 * Description:
 * This function receives a string that should contain a 5 bit code.
 * The function selects a IR code to be sent by comparing the input string with the macros defined previously.
 * If the argument is valid the function sends the corresponding IR code with the corresponding protocol.
 * 
 */
int SendCode (const char *dipString)
{
  for (unsigned char index = 0; index < strlen (dipString); index++)
    if ( (dipString [index] != 0) && (dipString [index] != 1) )
      return INVALID_DIP_STRING_ERROR;
  
  if (strcmp (dipString, ON_BUTTON_UN40H5100AG_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, ON_BUTTON_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, SOURCE_BUTTON_UN40H5100AG_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, SOURCE_BUTTON_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }
  if (strcmp (dipString, PLUS_VOLUME_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, PLUS_VOLUME_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, MINUS_VOLUME_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, MINUS_VOLUME_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, MUTE_BUTTON_UN40H5100AG_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, MUTE_BUTTON_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, PLUS_CHANNEL_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, PLUS_CHANNEL_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, MINUS_CHANNEL_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, MINUS_CHANNEL_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_1_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, CHANNEL_1_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_2_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, CHANNEL_2_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_3_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, CHANNEL_3_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_4_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, CHANNEL_4_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_5_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, CHANNEL_5_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_6_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, CHANNEL_6_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_7_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, CHANNEL_7_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_8_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, CHANNEL_8_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_9_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, CHANNEL_9_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, CHANNEL_0_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, CHANNEL_0_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));   
    return OK;
  }

  if (strcmp (dipString, RETURN_BUTTON_UN40H5100AG_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, RETURN_BUTTON_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, EXIT_BUTTON_UN40H5100AG_STRING_UN40H5100AG) == 0)
  {
    mySender.send (NECX, EXIT_BUTTON_UN40H5100AG, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

  if (strcmp (dipString, TEST_BUTTON_STRING) == 0)
  {
    mySender.send (NECX, TEST_BUTTON, NUMBER_OF_BITS_NECX);
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    return OK;
  }

/*
 * It takes about 55 seconds to send 2^10 codes.
 * For a 32 bit code, we have 2^32 possibilities, so it would take 22 times as long (22+ minutes) to send every possible code.
 * Conclusion: NOT HAPPENING

  //We'll send every code possible in the NECX protocol. Just for a goof
  // The ALL_BUTTON_UN40H5100AGS_STRING_UN40H5100AG use to correspond to "11110"
  if (strcmp (dipString, ALL_BUTTON_UN40H5100AGS_STRING_UN40H5100AG) == 0)
  {
    for (unsigned long index = 0; index < pow (2, 32); index++)
    {
      mySender.send (NECX, index, NUMBER_OF_BITS_NECX);
      digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
    }
    return OK;
  }
*/

  return UNKNOWN_DIP_STRING_ERROR;
}

void setup()
{
  Serial.begin (9600);
  delay (1001);
  myReceiver.enableIRIn (); // Start the receiver
  Serial.println ("Ready to send and receive IR signals");
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
  char dipString [DIP_STRING_LENGTH];
  /* We'll use this string to select which code will be sent to the TV */

  if (digitalRead (TRIGGER_PIN) == LOW)
  {
    ConstructDipString (dipString, FIRST_PIN, SECOND_PIN, THIRD_PIN, FOURTH_PIN, FIFTH_PIN);
    Serial.print ("Sending: ");
    Serial.println (dipString);
    SendCode (dipString);  
    delay (501);
  }


  //Continue looping until you get a complete signal received
  if (myReceiver.getResults ())
  {
    myDecoder.decode ();           //Decode it
    myDecoder.dumpResults (true);  //Now print results. Use false for less detail
    myReceiver.enableIRIn ();      //Restart receiver
  }
}
