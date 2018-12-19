#include "LedControl.h"
#include "EEPROMAnything1.h"
#include <LiquidCrystal.h>
#define SW_PIN 2 // digital pin connected to switch output
#define X_PIN A0 // analog pin connected to X output
#define Y_PIN A1 // analog pin connected to Y output
#define BUTTON_PIN 6
#define V0_PIN 9
#define BUZZER 13
int switch_value;
int x_value, y_value;
int interval = 25,intervalTragere = 1000;
int i = 0,j=0;
int conditioner =0;
int contor = 0;
int ok =1;
int vieti = 3;
int semafor = 1;
int score = 0;
int HIGHSCORE;

LiquidCrystal lcd(3, 4, 5, 6, 7, 8);
LedControl lc = LedControl(12, 11, 10,1);
short unsigned int matriceJoc [8][8] ={
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,1,0,0,0,0},
   {0,0,1,1,1,0,0,0}
};
void restartJoc(){
  for(int i =0 ; i<8;i++)
    for(int j =0; j< 8;j++)
      matriceJoc[i][j]=0;
  matriceJoc[7][2]=1;
  matriceJoc[7][3]=1;
  matriceJoc[7][4]=1;
  matriceJoc[6][3]=1;
}
void inamicGenerator(){
  int monster1 = random(1,7);
  matriceJoc[0][monster1] = 2;
  delay(100);
}

void joc(){
  for(int row=0 ; row < 8 ; row ++)
      for(int col=0; col < 8 ; col ++){
        if(matriceJoc[row][col] == 2)
          lc.setLed(0,row,col,matriceJoc[row][col]-1);
        else
          lc.setLed(0,row,col,matriceJoc[row][col]);
      }
}

int fire (){ 
  for(int col=0 ; col < 8 ; col ++)
    if(matriceJoc[6][col] == 1){
      tone(BUZZER,800,200);
      delay(100);
      noTone(BUZZER);
      matriceJoc[5][col] = 1;
    }
}

void moveRight (){
  short unsigned int col = 0;
  while(1){
    if( col == 5)
      break;
    if ( matriceJoc[7][col] == 1 && col < 5){
      matriceJoc[7][col] = 0;
      matriceJoc[7][col+1] = 1;
      matriceJoc[7][col+2] = 1;
      matriceJoc[7][col+3] = 1;
      matriceJoc[6][col+1] = 0;
      matriceJoc[6][col+2] = 1;
      tone(BUZZER,950,200);
      delay(50);
      noTone(BUZZER);
      break;
    }
    
    ++col;
  }
}
void moveLeft (){
  short unsigned int col = 7;
  while(1){
    if(col == 2)
      break;
    if ( matriceJoc[7][col] == 1 && col > 2){
      matriceJoc[7][col] = 0;
      matriceJoc[7][col-1] = 1;
      matriceJoc[7][col-2] = 1;
      matriceJoc[7][col-3] = 1;
      matriceJoc[6][col-1] = 0;
      matriceJoc[6][col-2] = 1;
      tone(BUZZER,950,200);
      delay(50);
      noTone(BUZZER);
      break;
    }
    if (matriceJoc[7][col] == 1 && col == 2){
      matriceJoc[7][0] = 1;
      matriceJoc[7][1] = 1;
      matriceJoc[7][2] = 1;
      matriceJoc[6][1] = 1;
      matriceJoc[7][3] = 0;
      matriceJoc[7][2] = 0;
      tone(BUZZER,950,200);
      delay(50);
      noTone(BUZZER);
      break;
    }
    --col;
  }
}
void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 3);
  lc.clearDisplay(0);
  Serial.begin(9600);
  pinMode(SW_PIN, INPUT);
  digitalWrite(SW_PIN, HIGH);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(1, 0);
  pinMode(V0_PIN, OUTPUT); 
  analogWrite(V0_PIN, 140);
  pinMode(BUZZER, OUTPUT);
  EEPROM_readAnything(0,HIGHSCORE);
}

void fireMove()
{
  for(int row = 1 ; row < 7 ; row++)
    if(matriceJoc[5][row] == 1)
      for(int linie = 5 ; linie > 0 ; linie -- ){
        if ( matriceJoc[linie-1][row] == 2){ 
           matriceJoc[linie-1][row] = 0;
           matriceJoc[linie][row] = 0;
           lc.setLed(0,linie-1,row,matriceJoc[linie-1][row]);
           lc.setLed(0,linie-1,row,matriceJoc[linie][row]);
           score += 10 ;
           tone(BUZZER,1000,200);
           delay(25);
           noTone(BUZZER);
           delay(5);
           break;
         }
        else{
        matriceJoc[linie-1][row] = matriceJoc[linie][row];
        lc.setLed(0,linie-1,row,matriceJoc[linie-1][row]);
        matriceJoc[linie][row] = 0;
        lc.setLed(0,linie,row,matriceJoc[linie][row]);
        delay(5);
        if(linie-1 == 0)
          matriceJoc[linie-1][row] = 0;
          lc.setLed(0,linie-1,row,matriceJoc[linie-1][row]);
        }
        
      }
      
        
    
}
void mutareInamici(){
  for ( int row = 7 ; row > 0 ; row--)
    for( int col = 1 ; col < 7 ; col++)
      if(matriceJoc[row-1][col] == 2){
      if(matriceJoc[row-1][col] == 2 && matriceJoc[row][col] == 1){
        ok = 0;
        conditioner = 0;
        matriceJoc[row-1][col] = 0;
        if(vieti !=0){
        vieti -- ;
        }
        semafor = 0;
        break;
        }
      matriceJoc[row][col]=matriceJoc[row-1][col];
      lc.setLed(0,row,col,matriceJoc[row-1][col]);
      matriceJoc[row-1][col]=0;
      lc.setLed(0,row,col,matriceJoc[row][col]);
      delay(55);
      
    }
  for( int col = 1 ; col < 7 ; col++){
    if( matriceJoc[7][col] == 2 ){
        matriceJoc[7][col] = 0;
        lc.setLed(0,7,col,matriceJoc[7][col]);
      }
  }
}
int mode = 0;
void bossBattle(){
 for(int col = 0; col < 2; col ++)
  for(int row = 1; row <7 ; row++){
    matriceJoc[col][row]=2;
    lc.setLed(0,col,row,matriceJoc[col][row]-1);
  }

}
void loop() {
  switch_value = digitalRead(SW_PIN);
  x_value = map(analogRead(X_PIN),0,1023,0,10);
  y_value = map(analogRead(Y_PIN),0,1023,0,10);
  unsigned long previousTime;
  if(switch_value == 0)
    previousTime = millis();
  if ( vieti == 0){
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("TRY AGAIN?");
      lcd.setCursor(0, 1);
      lcd.print("PRESS TO RESTART");
      tone(BUZZER, 800,100);
      delay(100);
      tone(BUZZER, 820,100);
      delay(120);
      tone(BUZZER, 840,100);
      delay(140);
      tone(BUZZER, 1300,100);
      delay(160);
      tone(BUZZER, 1800,1000);
      delay(200);
      noTone(BUZZER);
      delay(1000);
      lcd.clear();
      if(HIGHSCORE < score)
        HIGHSCORE = score;
      EEPROM_writeAnything(0,HIGHSCORE);
      lcd.setCursor(0, 0);
      lcd.print("HIGHSCORE:");
      lcd.print(HIGHSCORE);
      lcd.setCursor(0, 1);
      lcd.print("SCORE :");
      lcd.print(score);
      delay(2000);
      switch_value = digitalRead(SW_PIN);
      if(switch_value == 0){
        vieti = 3;
        semafor = 1;
        ok = 1;
        }
      if(HIGHSCORE < score)
        HIGHSCORE = score;
      EEPROM_writeAnything(0,HIGHSCORE);
      score = 0;
  }
  if ( ok == 1 && conditioner == 0 && semafor == 1){
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("PRESS BUTTON");
        lcd.setCursor(4, 1);
        lcd.print("TO START");
        tone(BUZZER, 800,100);
        delay(100);
        tone(BUZZER, 820,100);
        delay(120);
        tone(BUZZER, 840,100);
        delay(140);
        tone(BUZZER, 1300,100);
        delay(160);
        tone(BUZZER, 1800,1000);
        delay(200);
        noTone(BUZZER);
        if( switch_value == 0){
        conditioner = 1;
        }
        restartJoc();
     }
  
  if ( ok == 0 && vieti != 0 && semafor == 0){
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("GAME OVER");
    lcd.setCursor(2, 1);
    lcd.print("KEEP PRESSED");
    tone(BUZZER, 100,100);
    delay(100);
    tone(BUZZER, 500,100);
    delay(120);
    tone(BUZZER, 200,100);
    delay(140);
    tone(BUZZER, 800,100);
    delay(160);
    tone(BUZZER, 1800,1000);
    delay(200);
    noTone(BUZZER);
    delay(300);
    restartJoc();
    }
  if ( ok == 0 && switch_value == 0){
      conditioner = 1;
      ok = 1;
    }
  if( conditioner == 1 ){
    if ( ok == 1){
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("SCORE :");
      lcd.print(score);
      lcd.setCursor(1,1);
      lcd.print("LIFES :");
      lcd.print(vieti);
    joc();
    if( y_value < 4){
          delay(35);
          moveRight();
      }
    if( y_value > 4 ){
          delay(35);
          moveLeft();
      }
    if( contor%6 == 0)
      inamicGenerator();
    joc();
    if( switch_value == 0)
        fire();
    delay(25);
    fireMove();
    mutareInamici();
    contor++;
    if(score%100 == 0 && score !=0){
      bossBattle();
      mutareInamici();
    }
  }
  }
  }
