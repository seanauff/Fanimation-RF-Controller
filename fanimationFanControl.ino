#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <secrets.h>

const char* wifi_ssid = SECRET_WIFI_SSID;
const char* wifi_password = SECRET_WIFI_PASSWORD;
const char* mqtt_server = SECRET_MQTT_SERVER;
//const char* mqtt_user = "user";
//const char* mqtt_password = "pass";
const char* inTopic1 = "esp/fanimation/fan1/cmnd";
const char* outTopic1 = "esp/fanimation/fan1/stat";
const char* inTopic2 = "esp/fanimation/fan2/cmnd";
const char* outTopic2 = "esp/fanimation/fan2/stat";
const char* inTopic3 = "esp/fanimation/fan3/cmnd";
const char* outTopic3 = "esp/fanimation/fan3/stat";

const int TXPin = 2;

const int bitLengthShort = 265; // us
const int bitLengthLong = 600; // us
const int numBits = 13;
const int numRepeats = 15;

// fan1 (loft) DIP switches = [UP, DOWN, UP, DOWN]
int times1Stop[numBits] =    {0,655,1643,2968,3627,4945,5611,6929,7915,8908,9904,10562,11876};
int times1Reverse[numBits] = {0,655,1643,2968,3627,4945,5611,6929,7915,8908,9564,10885,11876};
int times1Speed1[numBits] =  {0,655,1643,2968,3627,4945,5611,6929,7915,8576,9904,10885,11876};
int times1Speed2[numBits] =  {0,655,1643,2968,3627,4945,5611,6929,7915,8576,9904,10562,11876};
int times1Speed3[numBits] =  {0,655,1643,2968,3627,4945,5611,6929,7590,8908,9904,10885,11876};
int times1Speed4[numBits] =  {0,655,1643,2968,3627,4945,5611,6929,7590,8576,9904,10885,11876};
int times1Speed5[numBits] =  {0,655,1643,2968,3627,4945,5611,6598,7915,8908,9904,10562,11876};
int times1Speed6[numBits] =  {0,655,1643,2968,3627,4945,5611,6598,7915,8908,9904,10885,11876};

int durations1Stop[numBits] =    {1,2,2,1,2,1,2,1,1,1,1,2,1};
int durations1Reverse[numBits] = {1,2,2,1,2,1,2,1,1,1,2,1,1};
int durations1Speed1[numBits] =  {1,2,2,1,2,1,2,1,1,2,1,1,1};
int durations1Speed2[numBits] =  {1,2,2,1,2,1,2,1,1,2,1,2,1};
int durations1Speed3[numBits] =  {1,2,2,1,2,1,2,1,2,1,1,1,1};
int durations1Speed4[numBits] =  {1,2,2,1,2,1,2,1,2,2,1,1,1};
int durations1Speed5[numBits] =  {1,2,2,1,2,1,2,2,1,1,1,2,1};
int durations1Speed6[numBits] =  {1,2,2,1,2,1,2,2,1,1,1,1,1};

// fan2 (bedroom) DIP switches = [DOWN, DOWN, DOWN, DOWN]
int times2Stop[numBits] =    {0,655,1944,2968,3896,4883,5611,6929,7915,8908,9904,10562,11876};
int times2Reverse[numBits] = {0,655,1944,2968,3896,4883,5611,6929,7915,8908,9564,10885,11876};
int times2Speed1[numBits] =  {0,655,1944,2968,3896,4883,5611,6929,7915,8576,9904,10885,11876};
int times2Speed2[numBits] =  {0,655,1944,2968,3896,4883,5611,6929,7915,8576,9904,10562,11876};
int times2Speed3[numBits] =  {0,655,1944,2968,3896,4883,5611,6929,7590,8908,9904,10885,11876};
int times2Speed4[numBits] =  {0,655,1944,2968,3896,4883,5611,6929,7590,8576,9904,10885,11876};
int times2Speed5[numBits] =  {0,655,1944,2968,3896,4883,5611,6598,7915,8908,9904,10562,11876};
int times2Speed6[numBits] =  {0,655,1944,2968,3896,4883,5611,6598,7915,8908,9904,10885,11876};
int times2Light[numBits] =   {0,655,1944,2968,3896,4883,5611,6929,7915,8908,9904,10885,11399};

int durations2Stop[numBits] =    {1,2,1,1,1,1,2,1,1,1,1,2,1};
int durations2Reverse[numBits] = {1,2,1,1,1,1,2,1,1,1,2,1,1};
int durations2Speed1[numBits] =  {1,2,1,1,1,1,2,1,1,2,1,1,1};
int durations2Speed2[numBits] =  {1,2,1,1,1,1,2,1,1,2,1,2,1};
int durations2Speed3[numBits] =  {1,2,1,1,1,1,2,1,2,1,1,1,1};
int durations2Speed4[numBits] =  {1,2,1,1,1,1,2,1,2,2,1,1,1};
int durations2Speed5[numBits] =  {1,2,1,1,1,1,2,2,1,1,1,2,1};
int durations2Speed6[numBits] =  {1,2,1,1,1,1,2,2,1,1,1,1,1};
int durations2Light[numBits] =   {1,2,1,1,1,1,2,2,1,1,1,1,2};

// fan3 (living room) DIP switches = [UP, DOWN, DOWN, UP]
int times3Stop[numBits] =    {0,655,1643,2968,3896,4537,5611,6929,7915,8908,9904,10562,11876};
int times3Reverse[numBits] = {0,655,1643,2968,3896,4537,5611,6929,7915,8908,9564,10885,11876};
int times3Speed1[numBits] =  {0,655,1643,2968,3896,4537,5611,6929,7915,8576,9904,10885,11876};
int times3Speed2[numBits] =  {0,655,1643,2968,3896,4537,5611,6929,7915,8576,9904,10562,11876};
int times3Speed3[numBits] =  {0,655,1643,2968,3896,4537,5611,6929,7590,8908,9904,10885,11876};
int times3Speed4[numBits] =  {0,655,1643,2968,3896,4537,5611,6929,7590,8576,9904,10885,11876};
int times3Speed5[numBits] =  {0,655,1643,2968,3896,4537,5611,6598,7915,8908,9904,10562,11876};
int times3Speed6[numBits] =  {0,655,1643,2968,3896,4537,5611,6598,7915,8908,9904,10885,11876};
int times3Light[numBits] =   {0,655,1643,2968,3896,4537,5611,6929,7915,8908,9904,10885,11399};

int durations3Stop[numBits] =    {1,2,2,1,1,2,2,1,1,1,1,2,1};
int durations3Reverse[numBits] = {1,2,2,1,1,2,2,1,1,1,2,1,1};
int durations3Speed1[numBits] =  {1,2,2,1,1,2,2,1,1,2,1,1,1};
int durations3Speed2[numBits] =  {1,2,2,1,1,2,2,1,1,2,1,2,1};
int durations3Speed3[numBits] =  {1,2,2,1,1,2,2,1,2,1,1,1,1};
int durations3Speed4[numBits] =  {1,2,2,1,1,2,2,1,2,2,1,1,1};
int durations3Speed5[numBits] =  {1,2,2,1,1,2,2,2,1,1,1,2,1};
int durations3Speed6[numBits] =  {1,2,2,1,1,2,2,2,1,1,1,1,1};
int durations3Light[numBits] =   {1,2,2,1,1,2,2,2,1,1,1,1,2};

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  pinMode(TXPin, OUTPUT);
  delayMicroseconds(10);
  digitalWrite(TXPin, LOW);
  
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}

void sendSignal(int times[], int durations[], int repeats)
{
  for (int r = 0; r < repeats; r++)
  {
    int last = 0;
    int bitLength = 0;
    // go through each "1" bit
    for (int i = 0; i < numBits; i++)
    {
      // calculate microseconds (us)
      int us = times[i];
      if (i != 0)
      {
        delayMicroseconds(us - last - bitLength);
      }
      if (durations[i] == 1)
      {
        bitLength = bitLengthShort;
      }
      else
      {
        bitLength = bitLengthLong;
      }
      // send a "1" for our bitLength
      digitalWrite(TXPin, HIGH);
      delayMicroseconds(bitLength);
      digitalWrite(TXPin, LOW);
      last = us;
    }
    digitalWrite(LED_BUILTIN, LOW);
    delayMicroseconds(11600); // short delay before sending another signal
    digitalWrite(LED_BUILTIN, HIGH);
  } 
}

void setup_wifi() 
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client"))
    if (client.connect("fanimation"))//, mqtt_user, mqtt_password))
    {
      Serial.println("connected");
      client.subscribe(inTopic1);
      Serial.print("Subscribed to: ");
      Serial.println(inTopic1);
      client.subscribe(inTopic2);
      Serial.print("Subscribed to: ");
      Serial.println(inTopic2);
      client.subscribe(inTopic3);
      Serial.print("Subscribed to: ");
      Serial.println(inTopic3);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  char cmnd = (char)payload[0];
  Serial.print("Command received: ");
  Serial.println(cmnd);

  if (topic[18] == '1')
  {
    switch (cmnd)
    {
      case 'S':
        sendSignal(times1Stop, durations1Stop, numRepeats);
        Serial.println("Sent Stop Signal to Fan 1.");
        break;
      case '1':
        sendSignal(times1Speed1, durations1Speed1, numRepeats);
        Serial.println("Sent Speed1 Signal to Fan 1.");
        break;
      case '2':
        sendSignal(times1Speed2, durations1Speed2, numRepeats);
        Serial.println("Sent Speed2 Signal to Fan 1.");
        break;
      case '3':
        sendSignal(times1Speed3, durations1Speed3, numRepeats);
        Serial.println("Sent Speed3 Signal to Fan 1.");
        break;
      case '4':
        sendSignal(times1Speed4, durations1Speed4, numRepeats);
        Serial.println("Sent Speed4 Signal to Fan 1.");
        break;
      case '5':
        sendSignal(times1Speed5, durations1Speed5, numRepeats);
        Serial.println("Sent Speed5 Signal to Fan 1.");
        break;
      case '6':
        sendSignal(times1Speed6, durations1Speed6, numRepeats);
        Serial.println("Sent Speed6 Signal to Fan 1.");
        break;
      case 'R':
        sendSignal(times1Reverse, durations1Reverse, numRepeats);
        Serial.println("Sent Reverse Signal to Fan 1.");
        break;
    }
  }
  else if (topic[18] == '2')
  {
    switch (cmnd)
    {
      case 'S':
        sendSignal(times2Stop, durations2Stop, numRepeats);
        Serial.println("Sent Stop Signal to Fan 2.");
        break;
      case '1':
        sendSignal(times2Speed1, durations2Speed1, numRepeats);
        Serial.println("Sent Speed1 Signal to Fan 2.");
        break;
      case '2':
        sendSignal(times2Speed2, durations2Speed2, numRepeats);
        Serial.println("Sent Speed2 Signal to Fan 2.");
        break;
      case '3':
        sendSignal(times2Speed3, durations2Speed3, numRepeats);
        Serial.println("Sent Speed3 Signal to Fan 2.");
        break;
      case '4':
        sendSignal(times2Speed4, durations2Speed4, numRepeats);
        Serial.println("Sent Speed4 Signal to Fan 2.");
        break;
      case '5':
        sendSignal(times2Speed5, durations2Speed5, numRepeats);
        Serial.println("Sent Speed5 Signal to Fan 2.");
        break;
      case '6':
        sendSignal(times2Speed6, durations2Speed6, numRepeats);
        Serial.println("Sent Speed6 Signal to Fan 2.");
        break;
      case 'R':
        sendSignal(times2Reverse, durations2Reverse, numRepeats);
        Serial.println("Sent Reverse Signal to Fan 2.");
        break;
      case 'L':
        sendSignal(times2Light, durations2Light, numRepeats);
        Serial.println("Sent Light Signal to Fan 2.");
        break;
    }
  }
  else if (topic[18] == '3')
  {
    switch (cmnd)
    {
      case 'S':
        sendSignal(times3Stop, durations3Stop, numRepeats);
        Serial.println("Sent Stop Signal to Fan 3.");
        break;
      case '1':
        sendSignal(times3Speed1, durations3Speed1, numRepeats);
        Serial.println("Sent Speed1 Signal to Fan 3.");
        break;
      case '2':
        sendSignal(times3Speed2, durations3Speed2, numRepeats);
        Serial.println("Sent Speed2 Signal to Fan 3.");
        break;
      case '3':
        sendSignal(times3Speed3, durations3Speed3, numRepeats);
        Serial.println("Sent Speed3 Signal to Fan 3.");
        break;
      case '4':
        sendSignal(times3Speed4, durations3Speed4, numRepeats);
        Serial.println("Sent Speed4 Signal to Fan 3.");
        break;
      case '5':
        sendSignal(times3Speed5, durations3Speed5, numRepeats);
        Serial.println("Sent Speed5 Signal to Fan 3.");
        break;
      case '6':
        sendSignal(times3Speed6, durations3Speed6, numRepeats);
        Serial.println("Sent Speed6 Signal to Fan 3.");
        break;
      case 'R':
        sendSignal(times3Reverse, durations3Reverse, numRepeats);
        Serial.println("Sent Reverse Signal to Fan 3.");
        break;
      case 'L':
        sendSignal(times3Light, durations3Light, numRepeats);
        Serial.println("Sent Light Signal to Fan 3.");
        break;
    }
  }
}
