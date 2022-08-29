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
void logoImage();

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  pinMode(changeButtonPin, INPUT_PULLUP);
  pinMode(cancelButtonPin, INPUT_PULLUP);
  pinMode(confirmButtonPin, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("wp"); // waiting participants

  while(!Serial.available()) {
    logoImage();
    delay(500);
  }

  String participantsData = Serial.readString();
  addParticipants(participantsData);

  Serial.println("pa"); // participants added
  lcd.clear();
  lcd.blink();

  for(int i = 0; i < 4; i++) {
    Serial.print(participants[i]);
    Serial.print('=');
    Serial.println(participantsVotes[i]);
  }

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
  lcd.noBlink();
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

      if(cmd == "end") {
        // send votation result
        for(int i = 0; i < 4; i++) {
          Serial.print(participants[i]);
          Serial.print('=');
          Serial.println(participantsVotes[i]);
        }
        
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Sistema");
        lcd.setCursor(4, 1);
        lcd.print("fechado");

        tone(buzzerPin, 1200);
        delay(150);
        tone(buzzerPin, 1000);
        delay(150);
        tone(buzzerPin, 800);
        delay(150);
        noTone(buzzerPin);
      
        delay(1000);
        resetFunc();
      }
    }
  }

  while(true) {
    for(int arrayPos = 0; arrayPos < 4; arrayPos++) {
      String participantName = participants[arrayPos];

      int nameLength = participantName.length();
      int nameLCDCursor = (15 - nameLength) / 2;
      
      lcd.clear();

      byte image01[8] = {B00000, B10000, B11000, B11100, B11110, B11100, B11000, B10000};
      lcd.createChar(0, image01);

      lcd.setCursor(0, 0);
      lcd.write(byte(0));
      
      lcd.setCursor(nameLCDCursor, 0);
      lcd.print(participantName);
      lcd.setCursor(0, 1);
      lcd.print("Mudar / Confirm.");

      while(true) {
        if(digitalRead(confirmButtonPin) == LOW) {
          while(digitalRead(confirmButtonPin) == LOW) {
            delay(50);
          }

          soundButtonPress();

          lcd.clear();
          lcd.setCursor(nameLCDCursor, 0);
          lcd.print(participantName);
          lcd.setCursor(1, 1);
          lcd.print("Canc. / Conf.");

          while(true) {
            if(digitalRead(confirmButtonPin) == LOW) {
              while(digitalRead(confirmButtonPin) == LOW) {
                delay(50);
              }

              lcd.clear();
              lcd.setCursor(6, 0);
              lcd.print("FIM");
              lcd.setCursor(4, 1);
              lcd.print("Obrigado");

              soundFinishVoting();
              participantsVotes[arrayPos] += 1;

              delay(2000);
              return;
            } else if(digitalRead(cancelButtonPin) == LOW) {
              while(digitalRead(cancelButtonPin) == LOW) {
                delay(50);
              }

              soundButtonPress();

              lcd.clear();
              lcd.setCursor(3, 0);
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

void logoImage() {
  lcd.clear();

  byte image08[8] = {B00000, B00000, B00000, B11111, B11111, B11111, B00100, B00100};
  byte image07[8] = {B00000, B00000, B00000, B00111, B11111, B11100, B11100, B11100};
  byte image09[8] = {B00000, B00000, B00000, B11100, B11111, B00111, B00111, B00111};
  byte image23[8] = {B11100, B11100, B11100, B11111, B00111, B00000, B00000, B00000};
  byte image25[8] = {B00111, B00111, B00111, B11111, B11100, B00000, B00000, B00000};
  byte image24[8] = {B00100, B00100, B11111, B11111, B11111, B00000, B00000, B00000};
  
  lcd.createChar(0, image08);
  lcd.createChar(1, image07);
  lcd.createChar(2, image09);
  lcd.createChar(3, image23);
  lcd.createChar(4, image25);
  lcd.createChar(5, image24);
  
  lcd.setCursor(7, 0);
  lcd.write(byte(0));
  lcd.setCursor(6, 0);
  lcd.write(byte(1));
  lcd.setCursor(8, 0);
  lcd.write(byte(2));
  lcd.setCursor(6, 1);
  lcd.write(byte(3));
  lcd.setCursor(8, 1);
  lcd.write(byte(4));
  lcd.setCursor(7, 1);
  lcd.write(byte(5));

  delay(500);

  // 50%

  lcd.clear();

  byte image14[8] = {B00000, B00000, B00000, B00111, B11111, B11100, B11100, B11100};
  byte image15[8] = {B00000, B00000, B00000, B11100, B11111, B00111, B00111, B00111};
  byte image16[8] = {B11100, B11100, B11100, B11111, B00111, B00000, B00000, B00000};
  byte image17[8] = {B00111, B00111, B00111, B11111, B11100, B00000, B00000, B00000};
  byte image18[8] = {B00100, B00100, B11111, B11111, B11111, B00000, B00000, B00000};
  
  lcd.createChar(0, image14);
  lcd.createChar(1, image15);
  lcd.createChar(2, image16);
  lcd.createChar(3, image17);
  lcd.createChar(4, image18);
  
  lcd.setCursor(6, 0);
  lcd.write(byte(0));
  lcd.setCursor(8, 0);
  lcd.write(byte(1));
  lcd.setCursor(6, 1);
  lcd.write(byte(2));
  lcd.setCursor(8, 1);
  lcd.write(byte(3));
  lcd.setCursor(7, 1);
  lcd.write(byte(4));

  delay(500);

  // 100%

  lcd.clear();

  byte image10[8] = {B00000, B00000, B00000, B00111, B11111, B11100, B11100, B11100};
  byte image11[8] = {B00000, B00000, B00000, B11100, B11111, B00111, B00111, B00111};
  byte image12[8] = {B11100, B11100, B11100, B11111, B00111, B00000, B00000, B00000};
  byte image13[8] = {B00111, B00111, B00111, B11111, B11100, B00000, B00000, B00000};
  
  lcd.createChar(0, image10);
  lcd.createChar(1, image11);
  lcd.createChar(2, image12);
  lcd.createChar(3, image13);
  
  lcd.setCursor(6, 0);
  lcd.write(byte(0));
  lcd.setCursor(8, 0);
  lcd.write(byte(1));
  lcd.setCursor(6, 1);
  lcd.write(byte(2));
  lcd.setCursor(8, 1);
  lcd.write(byte(3));

  delay(1000);
}
