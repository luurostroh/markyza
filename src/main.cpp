#include <Arduino.h>
 #define Vmin 45
//vystupy pwm
#define pwmM1 9
#define pwmM2 10
#define pwmM3 11
//vystupy digital
#define smer_1 12
#define smer_2 13

//analog vstupy
#define regM1 A0
#define regM2_3fwd A1
#define regM2_3bck A2
//digital vstupy
#define koncakM2 0
#define koncakM3 1
#define tl_start 2
#define tl_napinani_1 3
#define tl_napinani_2 4
#define prepinac_smer 5
uint8_t speed_M1 = 0;
uint8_t speed_M2_3 = 0;
int analog_val = 0;

void stopMotory();

void setup() 
{
  Serial.begin(9600);
  pinMode(pwmM1,OUTPUT);
  pinMode(pwmM2,OUTPUT);
  pinMode(pwmM3,OUTPUT);
  pinMode(smer_1,OUTPUT);
  pinMode(smer_2,OUTPUT);
  pinMode(tl_start,INPUT_PULLUP);
  pinMode(prepinac_smer,INPUT_PULLUP);  
  pinMode(koncakM2,INPUT_PULLUP); 
  pinMode(koncakM3,INPUT_PULLUP); 
  pinMode(tl_napinani_1,INPUT_PULLUP);
  pinMode(tl_napinani_2,INPUT_PULLUP);
}

void loop() 
{

  //kontrola koncaku
  while(digitalRead(koncakM2) || digitalRead(koncakM3));
  //vysunuti/zasunuti
  if(!digitalRead(tl_start))
  {
    Serial.println("tl_start_on");
    //nastaveni smeru pohybu
    uint8_t smer = digitalRead(prepinac_smer);
    if(smer == 1)
    {
      digitalWrite(smer_1,1);
      digitalWrite(smer_2,0);
    }
    else
    {
      digitalWrite(smer_1,0);
      digitalWrite(smer_2,1);
    }
    //precti hodnoty potenciometru
    //nastavovat se bude 50 - 100% duty cycle (hodnota 127 - 255)
    analog_val = analogRead(regM1);
    speed_M1 = Vmin + (analog_val/20);//cca 3,3 - 8 V pri 12V zdroji
    Serial.print("speed 1  = ");Serial.println(speed_M1);
    analog_val = smer==1?  analogRead(regM2_3fwd) : analogRead(regM2_3bck);
    speed_M2_3 = Vmin + (analog_val/20); //cca 3,3 - 8 V pri 12V zdroji
    Serial.print("speed 2  = ");Serial.println(speed_M2_3);
    //nastav 
    analogWrite(pwmM1,speed_M1);   
    analogWrite(pwmM2,speed_M2_3); 
    analogWrite(pwmM3,speed_M2_3);    
    while (1)
    {

      // analog_val = analogRead(regM1);
      // speed_M1 = 12 + (analog_val/8);
      // analogWrite(pwmM1,speed_M1);
      delay(10);
      //pri rozepnuti koncaku pri pohybu zpet zastav motory
     if(smer == 0 && (digitalRead(koncakM2) || digitalRead(koncakM3)))break;
     if(digitalRead(tl_start))break;
    }
   //Serial.println("tl_start_off");
    //stop vsechn motory
    stopMotory();

  }
  
  //napinani
  else if(!digitalRead(tl_napinani_1) || !digitalRead(tl_napinani_2))
  {
    digitalWrite(smer_1,1); 
    digitalWrite(smer_2,0);
    analog_val = analogRead(regM2_3fwd);
    speed_M2_3 = Vmin + (analog_val/20); //cca 3,3 - 8 V pri 12V zdroji
    if(!digitalRead(tl_napinani_1))analogWrite(pwmM2,speed_M2_3);  
    if(!digitalRead(tl_napinani_2))analogWrite(pwmM3,speed_M2_3); 
    //jed dokud je nektere tlacitko seple
    while(!digitalRead(tl_napinani_1) || !digitalRead(tl_napinani_2));
    //stop vsechn motory
    stopMotory();
  }

}

void stopMotory()
{
    digitalWrite(pwmM1,0);   
    digitalWrite(pwmM2,0); 
    digitalWrite(pwmM3,0);
    digitalWrite(smer_1,0); 
    digitalWrite(smer_2,0);
}