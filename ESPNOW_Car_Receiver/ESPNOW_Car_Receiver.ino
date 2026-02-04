// Define analog input pins
const int joyXPin = 34;  // X-axis
const int joyYPin = 35;  // Y-axis
const int buttonPin = 32; // Joystick button

// Variables to store joystick readings
int joyX = 0;
int joyY = 0;
bool buttonPressed = false;

void setup() {
  Serial.begin(115200);

  // Initialize the button pin as input with internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // You can also print a welcome message
  Serial.println("PS2 Joystick Test - ESP32");
}

void loop() {
  // Read analog values (range 0–4095 on ESP32)
  joyX = analogRead(joyXPin);
  joyY = analogRead(joyYPin);

  // Read button state (LOW when pressed due to pull-up)
  buttonPressed = digitalRead(buttonPin) == LOW;

  // Print values to Serial Monitor
  Serial.print("X: ");
  Serial.print(joyX);
  Serial.print(" | Y: ");
  Serial.print(joyY);
  Serial.print(" | Button: ");
  Serial.println(buttonPressed ? "Pressed" : "Released");

  delay(100); // Small delay for readability
}