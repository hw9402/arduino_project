#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Wire.h, LiquidCrystal_I2C.h => lcd 모듈을 위한 헤더파일
#include <DHT.h> // => 온습도센서를 위한 헤더파일

#define DHTPIN 7 // 온습도센서 핀을 7로 정함
#define DHTTYPE DHT11 // 온습도센서 타입을 DHT11로 지정

#define RED 3
#define GREEN 4
#define BLUE 5 // RED 3 / GREEN 4 / BLUE 5 => RGB 핀 정의

DHT dht(DHTPIN, DHTTYPE); // DHT센서 초기화

LiquidCrystal_I2C lcd(0x27, 16, 2); // lcd모듈 초기화

void setup() {
  Serial.begin(9600); // 시리얼 통신 시작
  lcd.begin(); // lcd 시작
  lcd.backlight(); // lcd 백라이트 켜기

  dht.begin(); // 온습도센서 시작
}

void loop() {
  float h = dht.readHumidity(); // 습도를 저장하는 변수 선언 및 초기화
  float t = dht.readTemperature(); // 온도를 저장하는 변수 선언 및 초기화
  float di = discomfortIndex(t, h); // 불쾌지수를 저장하는 변수 선언 및 초기화

  lcd.clear(); // lcd에 켜져있는 내용 지우기

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(500);
    return;
  } // 온습도센서가 제대로 작동하지 않으면 시리얼통신을 통해 알려주는 조건문

  lcd.setCursor(0,0); // lcd 속 커서 좌표를 (0,0)으로 지정
  lcd.print("T/H: ");
  lcd.print(t);
  lcd.print(" ");
  lcd.print(h);
  lcd.setCursor(0,1); // lcd 속 커서 좌표를 (0,0)으로 지정
  lcd.print(" DI: ");
  lcd.print(di);
  // ex) __________________
         |T/H: 24.67 68.03|
         | DI: 59.40      |
         ------------------

  Serial.println(di);
  
  if(di>=80){
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
  } // 불쾌지수가 80이상이라면 빨간색 led 켜기
  else if(di<80 && di>=68){
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
  } // 불쾌지수가 68이상 80미만이라면 초록색 led 켜기 
  else{
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, HIGH);
  } // 불쾌지수가 위에 있는 어떤 조건에도 해당되지 않는다면 파란색 led 켜기
  delay(1000);
}

float discomfortIndex(float temp, float humid){
  float di = (1.8f*temp)-(0.55*(1-humid/100.0f)*(1.8f*temp-26))+32;
  delay(500);
  return di;
} // 온도와 습도를 가지고 불쾌지수를 구하는 함수
