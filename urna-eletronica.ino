#include <LiquidCrystal_I2C.h>

const int buzzerPin = 2;

const int changeButtonPin = 3;
const int cancelButtonPin = 4;
const int confirmButtonPin = 5;

String participants[4];
int participantsVotes[4] = {0, 0, 0, 0};

LiquidCrystal_I2C lcd(0x27, 16, 2);

void addParticipants(String participantsData);
void soundFinishVoting();
void soundButtonPress();

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

  delay(1000);
}

void loop() {
  lcd.clear();
  lcd.print("Bem vindo.");
  lcd.setCursor(0, 1);
  lcd.print("Inicie o voto.");

  while(true) {
    if(digitalRead(confirmButtonPin) == LOW) {
      while(digitalRead(confirmButtonPin) == LOW) {
        delay(50);
      }

      soundButtonPress();

      break;
    } else if(Serial.available()) {
      delay(250);

      String cmd = Serial.readString();

      cmd.replace("\n", "");
      Serial.println(cmd);

      if(cmd == "gv") {
        for(int i = 0; i < 4; i++) {
          Serial.print(participants[i]);
          Serial.print('=');
          Serial.println(participantsVotes[i]);
        }
      }
    }
  }

  while(true) {
    for(int arrayPos = 0; arrayPos < 4; arrayPos++) {
      lcd.clear();
      lcd.print(participants[arrayPos]);
      lcd.setCursor(0, 1);
      lcd.print("Mudar / Confirm.");
  
      while(true) {
        if(digitalRead(confirmButtonPin) == LOW) {
          while(digitalRead(confirmButtonPin) == LOW) {
            delay(50);
          }

          soundButtonPress();

          lcd.clear();
          lcd.print("Confirmar voto?");
          lcd.setCursor(0, 1);
          lcd.print(participants[arrayPos]);

          while(true) {
            if(digitalRead(confirmButtonPin) == LOW) {
              while(digitalRead(confirmButtonPin) == LOW) {
                delay(50);
              }

              soundFinishVoting();
              participantsVotes[arrayPos] += 1;
  
              lcd.clear();
              lcd.print("Votou em:");
              lcd.setCursor(0, 1);
              lcd.print(participants[arrayPos]);

              delay(2000);
              return;
            } else if(digitalRead(cancelButtonPin) == LOW) {
              while(digitalRead(cancelButtonPin) == LOW) {
                delay(50);
              }

              soundButtonPress();

              lcd.clear();
              lcd.print("Cancelado");

              delay(2000);
              break;
            }
          }

          break;
        } else if(digitalRead(changeButtonPin) == LOW) {
          while(digitalRead(changeButtonPin) == LOW) {
            delay(50);
          }

          soundButtonPress();
  
          // return to "for" loop and changing
          // participant
          break;
        }
      }
    }
  }
}

void addParticipants(String participantsData) {
  String participantName = "";
  participantsData.replace(" ", "");
  participantsData.replace("\n", "");
  participantsData.concat(',');

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

void soundFinishVoting() {
  for(int i = 0; i < 5; i++) {
    tone(buzzerPin, 2032);
    delay(90);
    tone(buzzerPin, 2132);
    delay(90);
  }

  tone(buzzerPin, 2032);
  delay(120);
  noTone(buzzerPin);
}

void soundButtonPress() {
  tone(buzzerPin, 1000);
  delay(70);
  noTone(buzzerPin);
}
