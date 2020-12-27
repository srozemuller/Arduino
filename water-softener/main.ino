// This sketch can be used on a Wemos D1 mini. 
// For the sensor I used the HC-SR04

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = [SSID];
const char* password = [PASSOWRD];
const char* mqtt_server = [MQTT-ServerIP];
const char* mqtt_user = [MQTT-User];
const char* mqtt_password = [MQTT-Password];
const char* clientID = [MQTT-ClienName];

#define echoPin 4 // Echo Pin D2
#define trigPin 5 // Trigger Pin D1

long duration, distance; // Duration used to calculate distance

// Define which MQTT topics the data is published to
#define salt_topic "watersoftner/sensor/salt"
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  // Blink the LED until we're connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(0, LOW);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  // Connect to MQTT
  client.setServer(mqtt_server, 1883);
  // Initiate the sensor

  delay(50);
  digitalWrite(0, LOW);
}
// Attempts to reconnect to MQTT if we're ever disconnected
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    digitalWrite(0, HIGH);
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("WaterSoftner", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      digitalWrite(0, LOW);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
// Helps check whether any value change is significant enough to warrant a data push
bool checkSaltlevel(float newValue, float prevValue, float maxDiff) {
  return !isnan(newValue) &&
         (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}
long lastMsg = 0;
float newDistance = 0.0;
float diff = 1.0;
int counter = 0;
void loop() {
  // Ensure we stay connected
  if (!client.connected()) {
    reconnect();
  }
  // Let MQTT do it's thing
  client.loop();
  digitalWrite(0, LOW);
  bool sent = false;
  // Check the temp and humidity once per second
  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    // Read the values from the sensor

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    //Calculate the distance (in cm) based on the speed of sound.
    distance = duration / 58.2;
    Serial.print("Current distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    client.publish(salt_topic, String(distance).c_str(), true);
    // Check whether the humidity has changed
    counter ++;
    Serial.print("Last message was send ");
    Serial.print(counter);
    Serial.println(" checks ago");
    if (checkSaltlevel(distance, newDistance, diff)) {
      newDistance = distance;
      Serial.print("New distance:");
      Serial.println(String(newDistance).c_str());
      client.publish(salt_topic, String(newDistance).c_str(), true);
      sent = true;
      counter = 0;
    }
    if (counter == 10){
      Serial.print("Last message was ");
      Serial.print(counter);
      Serial.println("Last message was 10 check ago, sending value anyway. Value is: ");
      Serial.println(String(distance).c_str());
      client.publish(salt_topic, String(distance).c_str(), true);
      sent = true;
      counter = 0;
    }
    // If any data was sent (due to a change) then blink the blue LED
    if (sent) {
      sent = false;
      digitalWrite(2, LOW);
      delay(300);
      digitalWrite(2, HIGH);
    }
  }
  //delay(1000);
  Serial.println("Going into deepsleep for 20 minutes ");
  delay(1200000);
}
