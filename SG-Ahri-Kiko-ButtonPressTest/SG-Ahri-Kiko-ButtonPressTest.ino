#define BUTTONPIN 7
void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTONPIN, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(BUTTONPIN) == LOW)
    Serial.println("Play some shit!"); 
}
