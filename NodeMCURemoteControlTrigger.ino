/*
 * Update: 04/12/2017
 * Author: Kaylani Bochie
 * 
 * Yeah, I know. That's a stupid way of doing it.
 * 
 */

/*
 * The NodeMCU will host a webpage that will be used to control 6 outputs.
 * These outputs will be used to control the code sent by the Nano's remote controller.
 *
 * Just attach D0, D1, D2, D3 and D5 to the input pins of the Arduino Nano (the same input pins used for the dip switch)
 * Attach D8 to the trigger pin of the Arduino Nano. 
 *
 */

/* Defines and includes for the NodeMCU board */
#define D0                                        16
#define D1                                        5
#define D2                                        4
#define D3                                        0
#define D4                                        2
#define D5                                        14
#define D6                                        12
#define D7                                        13
#define D8                                        15
#define D9                                        3
#define D10                                       1
#define BUILT_IN_LED                              D4
#define INDICATOR_LED                             D4
/* ------------------------------------------------------------------------------------*/

#include <ESP8266WiFi.h>

#define TRIGGER_PIN                              D8
#define NETWORK_SSID                             "NETWORK NAME"
#define NETWORK_PASSWORD                         "NETWORK PASSWORD"

int ledPin = BUILT_IN_LED;
int firstOutput = D1;
int secondOutput = D2;

WiFiServer server(80);

int FazerConexaoWifiInicializarServidor (const char *networkSSID, const char *networkPassword)
{  
  Serial.println ();
  Serial.print ("Connecting to ");
  Serial.println (networkSSID);
  Serial.println ();
  delay (500);

  WiFi.begin (networkSSID, networkPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay (500);
    Serial.print (".");
  }
  Serial.println ("");
  Serial.println ("WiFi connected");

  // Start the server
  server.begin ();
  Serial.println ("Server started");

  // Print the IP address
  Serial.print ("Use this URL to connect: ");
  Serial.print ("http://");
  Serial.print (WiFi.localIP ());
  Serial.println ("/");

  delay (500);
  return OK;
}

void setup() {
  Serial.begin (9600);  

  pinMode (D0, OUTPUT);
  pinMode (D1, OUTPUT);
  pinMode (D2, OUTPUT);
  pinMode (D3, OUTPUT);
  pinMode (D5, OUTPUT);
  pinMode (TRIGGER_PIN, OUTPUT);
  pinMode (INDICATOR_LED, OUTPUT);
  digitalWrite (D0, HIGH);
  digitalWrite (D1, HIGH);
  digitalWrite (D2, HIGH);
  digitalWrite (D3, HIGH);
  digitalWrite (D5, HIGH);
  digitalWrite (TRIGGER_PIN, HIGH);
  digitalWrite (INDICATOR_LED, LOW);

  // Connect to WiFi network
  FazerConexaoWifiInicializarServidor (NETWORK_SSID, NETWORK_PASSWORD);
}

void loop() 
{
  digitalWrite (D0, HIGH);
  digitalWrite (D1, HIGH);
  digitalWrite (D2, HIGH);
  digitalWrite (D3, HIGH);
  digitalWrite (D5, HIGH);
  
  // Check if a client has connected
  WiFiClient client = server.available ();
  if (!client) 
  {
    return;
  }

  // Wait until the client sends some data
  Serial.println ("new client");
  while (!client.available ()) 
  {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil ('\r');
  Serial.println (request);
  client.flush ();

  // Match the request

  int value = LOW;
  if (request.indexOf ("/LED=ON") != -1) 
  {
    digitalWrite (ledPin, HIGH);
    value = HIGH;
  }
  else if (request.indexOf ("/LED=OFF") != -1) 
  {
    digitalWrite (ledPin, LOW);
    value = LOW;
  }
  else if (request.indexOf ("/00000") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, LOW);
    digitalWrite (D3, LOW);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/00001") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, LOW);
    digitalWrite (D3, LOW);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/00010") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, LOW);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/00011") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, LOW);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/00100") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, LOW);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/00101") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, LOW);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/00110") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/00111") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/01000") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, LOW);
    digitalWrite (D3, LOW);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/01001") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, LOW);
    digitalWrite (D3, LOW);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/01010") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, LOW);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/01011") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, LOW);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/01100") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, LOW);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/01101") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, LOW);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/01110") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/01111") != -1) 
  {
    digitalWrite (D0, LOW);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/10000") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, LOW);
    digitalWrite (D3, LOW);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/10001") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, LOW);
    digitalWrite (D3, LOW);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/10010") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, LOW);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/10011") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, LOW);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/10100") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, LOW);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/10101") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, LOW);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/10110") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/10111") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, LOW);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/11000") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, LOW);
    digitalWrite (D3, LOW);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/11001") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, LOW);
    digitalWrite (D3, LOW);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/11010") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, LOW);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/11011") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, LOW);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/11100") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, LOW);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/11101") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, LOW);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/11110") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, LOW);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else if (request.indexOf ("/11111") != -1) 
  {
    digitalWrite (D0, HIGH);   //MSB
    digitalWrite (D1, HIGH);
    digitalWrite (D2, HIGH);
    digitalWrite (D3, HIGH);
    digitalWrite (D5, HIGH);   //LSB
    delay (250);
    digitalWrite (TRIGGER_PIN, LOW);
    digitalWrite (INDICATOR_LED, !digitalRead (INDICATOR_LED));
  }
  else
  {
    Serial.print ("Input: ");
    Serial.println (request);
  }
  
  
  // Return the response
  client.println ("HTTP/1.1 200 OK");
  client.println ("Content-Type: text/html");
  client.println (""); //  do not forget this one
  client.println ("<!DOCTYPE HTML>");
  client.println ("<html>");

  client.print ("Led pin is now: ");
  if (value == HIGH)
    client.print ("On");
  else
    client.print ("Off");
  
  client.println ("<br><br>");
  client.println ("<a href=\"/LED=ON\"\"><button>Turn Off </button></a>");
  client.println ("<a href=\"/LED=OFF\"\"><button>Turn On </button></a>");


  client.println ("<br><br>");
  client.println ("<a href=\"/00000\"\"><button>00000</button></a><br>");
  client.println ("<a href=\"/00001\"\"><button>00001</button></a><br>");
  client.println ("<a href=\"/00010\"\"><button>00010</button></a><br>");
  client.println ("<a href=\"/00011\"\"><button>00011</button></a><br>");
  client.println ("<a href=\"/00100\"\"><button>00100</button></a><br>");
  client.println ("<a href=\"/00101\"\"><button>00101</button></a><br>");
  client.println ("<a href=\"/00110\"\"><button>00110</button></a><br>");
  client.println ("<a href=\"/00111\"\"><button>00111</button></a><br>");
  client.println ("<a href=\"/01000\"\"><button>01000</button></a><br>");
  client.println ("<a href=\"/01001\"\"><button>01001</button></a><br>");
  client.println ("<a href=\"/01010\"\"><button>01010</button></a><br>");
  client.println ("<a href=\"/01011\"\"><button>01011</button></a><br>");
  client.println ("<a href=\"/01100\"\"><button>01100</button></a><br>");
  client.println ("<a href=\"/01101\"\"><button>01101</button></a><br>");
  client.println ("<a href=\"/01110\"\"><button>01110</button></a><br>");
  client.println ("<a href=\"/01111\"\"><button>01111</button></a><br>");
  client.println ("<a href=\"/10000\"\"><button>10000</button></a><br>");
  client.println ("<a href=\"/10001\"\"><button>10001</button></a><br>");
  client.println ("<a href=\"/10010\"\"><button>10010</button></a><br>");
  client.println ("<a href=\"/10011\"\"><button>10011</button></a><br>");
  client.println ("<a href=\"/10100\"\"><button>10100</button></a><br>");
  client.println ("<a href=\"/10101\"\"><button>10101</button></a><br>");
  client.println ("<a href=\"/10110\"\"><button>10110</button></a><br>");
  client.println ("<a href=\"/10111\"\"><button>10111</button></a><br>");
  client.println ("<a href=\"/11000\"\"><button>11000</button></a><br>");
  client.println ("<a href=\"/11001\"\"><button>11001</button></a><br>");
  client.println ("<a href=\"/11010\"\"><button>11010</button></a><br>");
  client.println ("<a href=\"/11011\"\"><button>11011</button></a><br>");
  client.println ("<a href=\"/11100\"\"><button>11100</button></a><br>");
  client.println ("<a href=\"/11101\"\"><button>11101</button></a><br>");
  client.println ("<a href=\"/11110\"\"><button>11110</button></a><br>");
  client.println ("<a href=\"/11111\"\"><button>11111</button></a><br>");
  

  client.println ("</html>");
  delay (1);
  Serial.println ("Client disconnected");
  Serial.println ("");
}

