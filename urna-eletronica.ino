#include <LiquidCrystal_I2C.h>

const int buzzerPin = 2;

const int changeButtonPin = 3;
const int cancelButtonPin = 4;
const int confirmButtonPin = 5;

String participants[4];

LiquidCrystal_I2C lcd(0x27, 16, 2);

void addParticipants(String participantsData);

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  pinMode(changeButtonPin, INPUT_PULLUP);
  pinMode(cancelButtonPin, INPUT_PULLUP);
  pinMode(confirmButtonPin, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("wp"); // waiting participants

  lcd.print("Aguardando");
  lcd.setCursor(0, 1);
  lcd.print("participantes...");

  while(Serial.available() == 0) {delay(100);}

  String participantsData = Serial.readString();
  addParticipants(participantsData);

  Serial.println("pa"); // participants added
  lcd.clear();
  lcd.print("Participantes");
  lcd.setCursor(0, 1);
  lcd.print("adicionados.");

  tone(buzzerPin, 800);
  delay(150);
  tone(buzzerPin, 1000);
  delay(150);
  tone(buzzerPin, 1200);
  delay(150);
  noTone(buzzerPin);
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
