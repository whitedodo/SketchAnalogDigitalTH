/*
  Subject: 온습도 시간 측정(Temperature / Humidity and Time Analysis)
  Author: Dodo(rabbit.white@daum.net)
  Filename: SketchAnalogDigitalTH.ino
  Create Date: 2018-08-14
  Description:
*/

#include <DHT.h>
#include <time.h>

const int temperaturePin = 0;
const int DHTPIN = 2;
#define DHTTYPE DHT22  // DHT타입: DHT11, DHT22

extern volatile unsigned long timer0_millis; // 타이머 변수
unsigned long timeVal;  // 이전 시간
unsigned long readTime; // 현재 타이머 시간
int year, month, day;
int hour, min, sec;
boolean state = false;


DHT dht(DHTPIN, DHTTYPE);  // DHT설정 - dht (디지털2, dht11)

float dht_temperature;
float dht_humidity;

void setup() {  
  Serial.begin(9600); //시리얼 통신속도 설정

  setDate(2018, 8, 14);
  setTime(18, 20, 50);

  while (! Serial){
    loop();
  }
  
  timer0_millis = ((long)hour*3600+min*60+sec)*1000;
  state = true;
  timeVal = millis();
}
 
void loop() {
  char type = NULL;
  int reading = analogRead(temperaturePin); // 센서값 읽어오기

  if ( Serial.available()){
    type = Serial.read();
  }
  
  printTemperatureHumidity();
  delay(1000);
  printDateTime();
  
  Serial.print(",");
  Serial.print( (5.0*reading*100.0) / 1024.0 );
  Serial.print(",");
  Serial.print( dht_temperature );
  Serial.print(",");
  Serial.print( dht_humidity );
  Serial.print("%");
  Serial.print(",");
  printType(type);
  Serial.println("");
  delay(2000); // 측정 간격 설정 (2000 = 2초)
}

void setDate(int y, int m, int d){
  year = y;
  month = m;
  day = d;
}

void setTime(int h, int m, int s){
  hour = h;
  min = m;
  sec = s;
}

void printTemperatureHumidity(){
  
  int h = dht.readHumidity();  // 변수 h에 습도 값을 저장;
  int t = dht.readTemperature();  // 변수 t에 온도 값을 저장;
  
  if ( isnan(h) || isnan(t) ) {
    dht_temperature = t;
    dht_humidity = h;
  }else{
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}

void printDateTime(){

  // 년, 월, 일
  Serial.print( year );
  Serial.print( "-" );
  
  if ( month < 10 )
  {
    Serial.print("0");
  }
  
  Serial.print( month );
  Serial.print( "-" );
  Serial.print( day );
  Serial.print( " " );
  
  if( state==true ){ // 시리얼모니터 출력 여부
    
    if(millis()-timeVal>=1000){
      // 1초 단위 출력
      readTime = millis()/1000;
      
      if(millis()>=86400000){
        timer0_millis=0;
      }
     
      timeVal = millis();
   
      sec = readTime%60;
      min = (readTime/60)%60;
      hour = (readTime/(60*60))%24;       
      
      Serial.print(hour);
      Serial.print(":");
      Serial.print(min);
      Serial.print(":");

      if ( sec < 10 ){
        Serial.print("0");
      }
      Serial.print(sec);      
    }
  } // end of if
  
}

void printType(char type){

  if ( type == 'a' ){
    Serial.print( "에어컨(Airconditioner)" );
  }else if ( type == 'w') {
    Serial.print( "선풍기(ElectricFan)" ); 
  }else if ( type == 'n') {
    Serial.print( "보통(Normal)" ); 
  }else{
    Serial.print( "출력중" ); 
  }
}

