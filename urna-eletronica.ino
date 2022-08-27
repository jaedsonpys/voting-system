#include <LiquidCrystal_I2C.h>

const int buzzerPin = 2;

const int changeButtonPin = 3;
const int cancelButtonPin = 4;
const int confirmButtonPin = 5;

String participants[4];

LiquidCrystal_I2C lcd(0x27, 16, 2);

void addParticipants(String participantsData);

void setup() {
  Serial.begin(9600);
  Serial.println("wp"); // waiting participants
  
  lcd.init();
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  pinMode(changeButtonPin, INPUT_PULLUP);
  pinMode(cancelButtonPin, INPUT_PULLUP);
  pinMode(confirmButtonPin, INPUT_PULLUP);

  while(Serial.available() == 0) {delay(100);}

  String participantsData = Serial.readString();
  addParticipants(participantsData);
}

void loop() {
  
}

void addParticipants(String participantsData) {
  String participantName = "";
  participantsData.replace(" ", "");
  
  int arrayPos = 0;
  
  for(int i = 0; i < participantsData.length(); i++) {
    char letter = participantsData[i];

    if(letter != ',') {
      participantName.concat(letter);
    } else {
      participants[arrayPos] = participantName;
      participantName = "";
      arrayPos += 1;
    }
  }
}
