#include <FS.h>
#include <WiFiManager.h>
#include <WiFi.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

// Replace with your network credentials
//const char* ssid = "OnePlus 9R";
//const char* password = "rick1102";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String plotter = "off";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// Define the stepper motor and the pins that is connected to
AccelStepper stepper1(AccelStepper::FULL4WIRE, 13, 12, 27, 33); // Left motor
AccelStepper stepper2(AccelStepper::FULL4WIRE, 21, 17, 16, 19); // Right motor

MultiStepper steppersControl;  // Create instance of MultiStepper

long gotoposition[2]; // An array to store the target positions for each stepper motor



void setup() {
  Serial.begin(115200);
  Serial.println();

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //reset settings - for testing
  wifiManager.resetSettings();

  //set static ip
  //block1 should be used for ESP8266 core 2.1.0 or newer, otherwise use block2

  //start-block1
  //IPAddress _ip,_gw,_sn;
  //_ip.fromString(static_ip);
  //_gw.fromString(static_gw);
  //_sn.fromString(static_sn);
  //end-block1

  //start-block2
  IPAddress _ip = IPAddress(10, 50, 1, 15);
  IPAddress _gw = IPAddress(10, 50, 0, 1);
  IPAddress _sn = IPAddress(255, 255, 255, 0);
  //end-block2
  
  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);


  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" with password "password"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("PenplotterAP", "password")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS 1: ");
  Serial.println(WiFi.dnsIP(0));
  Serial.print("DNS 2: ");
  Serial.println(WiFi.dnsIP(1));
  server.begin();
  
  // Setting maximum speed value for the stepper
  stepper1.setMaxSpeed(250); 
  stepper2.setMaxSpeed(250);

  // Adding the 2 steppers to the steppersControl instance for multi stepper control
  steppersControl.addStepper(stepper1);
  steppersControl.addStepper(stepper2);
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /13/on") >= 0){
              Serial.println("Motors are on");
              plotter = "on";
              // Side 1
                gotoposition[0] = 1120;
                gotoposition[1] = -1120;
              
                steppersControl.moveTo(gotoposition); // Calculates the required speed for all motors
                steppersControl.runSpeedToPosition(); // Blocks until all steppers are in position
              
                delay(1000);
              
                //Side 2
                gotoposition[0] = gotoposition[0] - 892.416; 
                gotoposition[1] = gotoposition[1] - 1040.896;

                steppersControl.moveTo(gotoposition);
                steppersControl.runSpeedToPosition();
              
                delay(1000);
              
                // Side 3
                gotoposition[0] = gotoposition[0] - 1267.712; 
                gotoposition[1] = gotoposition[1] + 1267.712;
              
                steppersControl.moveTo(gotoposition);
                steppersControl.runSpeedToPosition();
              
                delay(1000);
              
                // Side 4
                gotoposition[0] = gotoposition[0] + 218.112; 
                gotoposition[1] = gotoposition[1] - 1049.6;
              
                steppersControl.moveTo(gotoposition);
                steppersControl.runSpeedToPosition();
              
                delay(1000);
              
                // Side 5
                gotoposition[0] = gotoposition[0] + 1049.6; 
                gotoposition[1] = gotoposition[1] - 218.112;
              
                steppersControl.moveTo(gotoposition);
                steppersControl.runSpeedToPosition();

                delay(1000);
              
                // Side 6
                gotoposition[0] = gotoposition[0] - 226.816; 
                gotoposition[1] = gotoposition[1] + 2160.128;
              
                steppersControl.moveTo(gotoposition);
                steppersControl.runSpeedToPosition();
              
                delay(1000);
              
                // Side 7
                gotoposition[0] = gotoposition[0] - 1040.896; 
                gotoposition[1] = gotoposition[1] - 892.416;
              
                steppersControl.moveTo(gotoposition);
                steppersControl.runSpeedToPosition();
              
                delay(1000);
              
                // Side 8
                gotoposition[0] = gotoposition[0] + 2160.128; 
                gotoposition[1] = gotoposition[1] - 226.816;
              
                steppersControl.moveTo(gotoposition);
                steppersControl.runSpeedToPosition();
              
                delay(1000);
            } else if (header.indexOf("GET /13/off") >= 0){
              Serial.println("Motors are off");
              plotter = "off";
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Team 12 Penplotter</h1>");
            
            // Display current state, and ON/OFF button 
            client.println("<p>Plotter is " + plotter + "</p>");
            // If the output13State is off, it displays the ON button       
            if (plotter=="off") {
              client.println("<p><a href=\"/13/on\"><button class=\"button\">ON</button></a></p>");
            } else
              client.println("<p><a href=\"/13/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
 
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
      }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    //Serial.println("Client disconnected.");
    //Serial.println("");
  }
