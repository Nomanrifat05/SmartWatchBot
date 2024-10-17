#define BLYNK_TEMPLATE_ID "TMPL6PXP3MlLw"
#define BLYNK_TEMPLATE_NAME "smart detection"
#define BLYNK_AUTH_TOKEN "Ik2psx9qdr5U0Za9nbMJnLGhY82JkLfx"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

int flame_sensor = D5;
int gas_sensor = A0;
int buzzer_pin = D6;  // Pin connected to the buzzer
char ssid[] = "Noman.";  
char pass[] = "0987654321";  

BlynkTimer timer;

void sendSensorData() {
  int gas_data = analogRead(gas_sensor);
  int flame_data = !digitalRead(flame_sensor);

  Serial.println("Gas sensor data: " + String(gas_data));
  Serial.println("Flame sensor data: " + String(flame_data));

  // Send data to Blynk app
  Blynk.virtualWrite(V0, flame_data);
  Blynk.virtualWrite(V1, gas_data);

  // Buzzer logic
  if (flame_data == 1) {
    Serial.println("Flame detected! Turning on buzzer for 10 seconds.");
    digitalWrite(buzzer_pin, HIGH);  // Turn on the buzzer
    delay(10000);  // Keep the buzzer on for 10 seconds
    digitalWrite(buzzer_pin, LOW);   // Turn off the buzzer
  } else if (gas_data > 500) {
    Serial.println("High gas level detected! Turning on buzzer.");
    digitalWrite(buzzer_pin, HIGH);  // Turn on the buzzer
  } else {
    digitalWrite(buzzer_pin, LOW);   // Turn off the buzzer if no flame or gas
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(buzzer_pin, OUTPUT);  // Set buzzer pin as OUTPUT
  pinMode(flame_sensor, INPUT); // Set flame sensor pin as INPUT
  pinMode(gas_sensor, INPUT);   // Set gas sensor pin as INPUT

  timer.setInterval(1000L, sendSensorData);  // Send sensor data every 1 second
}

void loop() {
  Blynk.run();
  timer.run();  // Runs the sensor data sending function
}
