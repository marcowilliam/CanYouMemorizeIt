#include <Servo.h>
#define NOTE_C4  261
#define NOTE_D4  294
#define NOTE_E4  329
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_C6  1047
#define NOTE_D6  1175
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_G6  1568
#define NOTE_C7  2093
#define NOTE_D7  2349
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_G7  3136

int blue = 9; //this sets the blue led pin
int green = 8; //this sets the green led pin
int red = 7; //this sets the red led pin

//setting up the notes
int notes[][5] ={
  {NOTE_C4,NOTE_D4,NOTE_E4, NOTE_F4, NOTE_G4},
  {NOTE_C5,NOTE_D5,NOTE_E5, NOTE_F5, NOTE_G5},
  {NOTE_C6,NOTE_D6,NOTE_E6, NOTE_F6, NOTE_G6},
  {NOTE_C7,NOTE_D7,NOTE_E7, NOTE_F7, NOTE_G7},
  };
//control when change the notes
int notesSwitch = 0;

//variable for the velocity of the blinks
int vel = 300;

//variable to control the level
int level = 1;

//variable for the sequence
int sequence[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//variable to store the user input
int userInput = 0;

//setting up the motor
Servo myservo;
//making the sequence that the motor does, to show the score
int posServo[] = {178,159, 139, 120, 100, 80, 65, 46, 30, 8, 20, 37, 55, 74, 91, 112, 130, 150, 169};

//setting up the LEDs inputs
int LED[5] = {5,4,3,2,6};

void setup() {
  Serial.begin(9600);
  pinMode(LED[0], OUTPUT);
  pinMode(LED[1], OUTPUT);
  pinMode(LED[2], OUTPUT);
  pinMode(LED[3], OUTPUT);
  pinMode(LED[4], OUTPUT);
  randomSeed(analogRead(A5));
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  myservo.attach(10);
}

void loop() {
  
  //it chekcs if the level is 5,10,15 so then it changes the notes
  if(level%5==0){
    notesSwitch++;
  }
  
  //increasing the level 
  level++;
  //changin the positition of the mortor
  myservo.write(posServo[level-1]);
  delay(200);
  //calling the function to make and show the sequence
  blinkLEDs();
  //wait until the user enter with his/her sequence
  userEnters();
  noTone(11);
  delay(2000);
  //if the level is below 15, it keeps increasing the velocity
  if(level<15)
    vel=vel-10;
  //check if the player made 20 points and won the game
  checkGameWon();
  
  
}

//this function make the sequence of the LEDs blinking 
void blinkLEDs(){
  
  int control = 0;
  int num = random(5)+1;
 
  
  sequence[level-1] = num;
  
  while(control<level){
    
    tone(11, notes[notesSwitch][sequence[control]-1]);
    digitalWrite(LED[sequence[control]-1], HIGH);  
    delay(vel);             
    digitalWrite(LED[sequence[control]-1], LOW);
    delay(vel); 
    control++;
  
  }
}

void userEnters(){
  int control = 0;
  
  //keeps reading the user inputs until the number of sequence or until he/she loses
  while (control<level){
      analogWrite(green, 0);
      int keyVal = analogRead(A0);
      Serial.println(keyVal);
      if (keyVal >= 86 && keyVal <= 100) {
        tone(11, notes[notesSwitch][0]);
        digitalWrite(LED[0], HIGH);  
        delay(300);             
        digitalWrite(LED[0], LOW);
        delay(300); 
        control++;
        userInput = 1;
      }
      else if(keyVal >= 758 && keyVal <= 770) {
        tone(11, notes[notesSwitch][1]);
        digitalWrite(LED[1], HIGH);  
        delay(300);             
        digitalWrite(LED[1], LOW);
        delay(300);
        control++;
        userInput = 2;
      }
      else if(keyVal >= 790 && keyVal <= 815) {
        tone(11, notes[notesSwitch][2]);
        digitalWrite(LED[2], HIGH);  
        delay(300);             
        digitalWrite(LED[2], LOW);
        delay(300); 
        control++;
        userInput = 3;
      }
      else if(keyVal >= 490 && keyVal <= 530) {
        tone(11, notes[notesSwitch][3]);
        digitalWrite(LED[3], HIGH);  
        delay(300);             
        digitalWrite(LED[3], LOW);
        delay(300); 
        control++;
        userInput = 4;
      }
      else if(keyVal >= 970 && keyVal <= 1030) {
        tone(11, notes[notesSwitch][4]);
        digitalWrite(LED[4], HIGH);  
        delay(300);             
        digitalWrite(LED[4], LOW);
        delay(300); 
        control++;
        userInput = 5;
      }
      else {
        noTone(11);
      }
     checkGameOver(control);
  }
  
}

void checkGameOver(int control){
  
  //if the user gets the right answer, the green led (RGB) blinks
  if(userInput==sequence[control-1]){
          analogWrite(green, 255);
          delay(1);
          analogWrite(green, 0);
      }
      
   //if the user gets the worng answer   
   else if(userInput!=sequence[control-1]) {
          //turn the RGB led red
          analogWrite(red, 250);
          //show what would be the right answer
          tone(11, notes[notesSwitch][sequence[control-1]-1]);
          digitalWrite(LED[sequence[control-1]-1], HIGH); 
          //plays the gameover song 
          delay(1000);             
          noTone(11);
          delay(1000);
          tone(11, notes[notesSwitch][2]);
          delay(400);
          tone(11, notes[notesSwitch][4]);
          delay(400);
          tone(11, notes[notesSwitch][3]);
          delay(100);
          tone(11, notes[notesSwitch][2]);
          delay(100);
          tone(11, notes[notesSwitch][1]);
          delay(100);
          tone(11, notes[notesSwitch][0]);
          delay(1000);
          analogWrite(red, 0);
          digitalWrite(LED[sequence[control-1]-1], LOW);
          level = 0;
          vel = 300;
          notesSwitch = 0;
          myservo.write(180);
          
      }
}

void checkGameWon(){
 
  if(level == 20){
    
      //turn the RGB red
      digitalWrite(LED[0], HIGH);
      //make the winning song
      tone(11, notes[notesSwitch][0]);
      myservo.write(180); 
      delay(100);
      digitalWrite(LED[0], LOW);
      digitalWrite(LED[1], HIGH);
      tone(11, notes[notesSwitch][1]);
      delay(100);
      digitalWrite(LED[1], LOW);
      digitalWrite(LED[2], HIGH);
      tone(11, notes[notesSwitch][2]);
      delay(100);
      digitalWrite(LED[2], LOW);
      digitalWrite(LED[3], HIGH);
      tone(11, notes[notesSwitch][3]);
      delay(100);
      digitalWrite(LED[3], LOW);
      digitalWrite(LED[4], HIGH);
      tone(11, notes[notesSwitch][4]);
      delay(100);
      digitalWrite(LED[4], LOW);
      digitalWrite(LED[3], HIGH);
      tone(11, notes[notesSwitch][3]);
      delay(100);
      digitalWrite(LED[3], LOW);
      digitalWrite(LED[2], HIGH);
      tone(11, notes[notesSwitch][2]);
      delay(100);
      digitalWrite(LED[2], LOW);
      digitalWrite(LED[1], HIGH);
      tone(11, notes[notesSwitch][1]);
      delay(100);
      myservo.write(10);
      myservo.write(180); 
      digitalWrite(LED[1], LOW);
      digitalWrite(LED[0], HIGH);
      tone(11, notes[notesSwitch][0]);
      delay(100);
      digitalWrite(LED[0], LOW);
      for(int i=0;i<5;i++){
          delay(800);
          myservo.write(180);
          tone(11, notes[notesSwitch][2]);
          analogWrite(red, 0);
          analogWrite(green, 0);
          analogWrite(blue, 250);
          delay(400);
          tone(11, notes[notesSwitch][4]);
          analogWrite(red, 0);
          analogWrite(green, 250);
          analogWrite(blue, 0);
          delay(400);
          tone(11, notes[notesSwitch][3]);
          delay(100);
          tone(11, notes[notesSwitch][2]);
          analogWrite(red, 250);
          analogWrite(green, 0);
          analogWrite(blue, 0);
          myservo.write(10);
          delay(100);
          tone(11, notes[notesSwitch][1]);
          analogWrite(red, 0);
          analogWrite(green, 250);
          analogWrite(blue, 0);
          delay(100);
          analogWrite(red, 0);
          analogWrite(green, 0);
          analogWrite(blue, 250);
          if(i==4)
            tone(11, notes[notesSwitch][0]);
          else
            tone(11, notes[notesSwitch][i]);
          myservo.write(180);
          digitalWrite(LED[0], HIGH);
          digitalWrite(LED[1], HIGH);
          digitalWrite(LED[2], HIGH);
          digitalWrite(LED[3], HIGH);
          digitalWrite(LED[4], HIGH);
          delay(1000);
          myservo.write(10);
          digitalWrite(LED[0], LOW);
          digitalWrite(LED[1], LOW);
          digitalWrite(LED[2], LOW);
          digitalWrite(LED[3], LOW);
          digitalWrite(LED[4], LOW);
      }
      
      //setup all the values
      level = 0;
      vel = 300;
      notesSwitch = 0;
      myservo.write(10);
      myservo.write(180);
      noTone(11);
      delay(2000);
  }

}
