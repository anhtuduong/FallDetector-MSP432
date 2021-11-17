#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information

WiFiClient client;

// Set the number of Max Calls to one since the user only needs to be notified once that the object has fallen
int maxCalls = 1;

// The number of times this Choreo has been run so far in this sketch
int calls = 0;

const int xpin = 23;                  // x-axis of the accelerometer
const int ypin = 24;                  // y-axis
const int zpin = 25;                  // z-axis (only on 3-axis models)
const int maxChange = 100;             // increment the accelerometer value needs to change in order to trigger the choreo
//initialize accelerometer values
int x_val = 0;
int y_val = 0;
int z_val = 0;
int x_init = 0;
int y_init = 0;
int z_init = 0;
boolean firstRun = 1;

void setup() {
  Serial.begin(9600);

  int wifiStatus = WL_IDLE_STATUS;

  // Determine if the WiFi Shield is present
  Serial.print("\n\nShield:");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("FAIL");

    // If there's no WiFi shield, stop here
    while(true);
  }

  Serial.println("OK");

  // Try to connect to the local WiFi network
  while(wifiStatus != WL_CONNECTED) {
    Serial.print("WiFi:");
    wifiStatus = WiFi.begin(WIFI_SSID, WPA_PASSWORD);

    if (wifiStatus == WL_CONNECTED) {
      Serial.println("OK");
    } 
    else {
      Serial.println("FAIL");
    }
    delay(5000);
  }

  // Initialize pins
  pinMode(xpin,INPUT);
  pinMode(ypin,INPUT);
  pinMode(zpin,INPUT);

  Serial.println("Setup complete.\n");
}

void loop() {
  //read in current values of accelerometer and print through serial
  x_val = analogRead(xpin);
  y_val = analogRead(ypin);
  z_val = analogRead(zpin);
  Serial.print(x_val);
  Serial.print("\t");
  Serial.print(y_val);
  Serial.print("\t");
  Serial.print(z_val);
  Serial.print("\n\r");
  //check if first time running through and if base values need to be set
  if(!firstRun){
    //trigger the choreo if the accelerometer values have changed more than the max allowed amount
    if ((abs(x_val-x_init)>=maxChange)||(abs(y_val-y_init)>=maxChange)||(abs(z_val-z_init)>=maxChange)) {
      if (calls < maxCalls) {
        Serial.println("\nTriggered! Calling SendSMS Choreo...");
        runSendSMS();
        calls++;
      } 
      else {
        Serial.println("\nTriggered! Already notified user of the fall so no text sent!");
      }
    }
    delay(250);
  }
  else{
    //if this is the first time through, set the initial values and change the firstRun boolean value
    x_init = x_val;
    y_init = y_val;
    z_init = z_val;
    firstRun = 0;
  }
}
void runSendSMS() {
  TembooChoreo SendSMSChoreo(client);

  // Set Temboo account credentials
  SendSMSChoreo.setAccountName(TEMBOO_ACCOUNT);
  SendSMSChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  SendSMSChoreo.setAppKey(TEMBOO_APP_KEY);

  // Set profile to use for execution
  SendSMSChoreo.setProfile("Twiliownum");
  // Identify the Choreo to run
  SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");

  // Run the Choreo
  unsigned int returnCode = SendSMSChoreo.run();

  // Read and print the error message
  while (SendSMSChoreo.available()) {
    char c = SendSMSChoreo.read();
    Serial.print(c);
  }
  Serial.println();
  SendSMSChoreo.close();
}
