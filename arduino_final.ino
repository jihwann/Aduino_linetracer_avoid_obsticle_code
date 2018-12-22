#include <TimerOne.h>
#include <SoftwareSerial.h>     // 시리얼 통신 라이브러리 호출
#define TIMER_US 100  
int pinECHO = 2;
int pinTRIG = 10;
int distance;
int blueRx = 4;     // Rx (보내는핀 설정)
int blueTx = 5;     // Tx (받는핀 설정)
int sensorCenter = A0;
int sensorRight = A2;
int sensorLeft = A1;
int count = 1;
int val1; // 초음파 거리
int center_data;
int left_data;
int right_data;

int center_data2;
int left_data2;
int right_data2;
 
SoftwareSerial mySerial(blueRx, blueTx);      // 시리얼 통신을 위한 객체 선언
int phoneData;      // 시리얼 데이터를 받기 위한 변수 선언
  // 회전 방향 변수 선언
#define FORWARD  0
#define BACKWARD 1
  // 왼쪽, 오른쪽 바퀴 변수 선언
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1
const byte PWMA = 3;      // PWM control (speed) for motor A
const byte PWMB = 11;     // PWM control (speed) for motor B
const byte DIRA = 12;     // Direction control for motor A
const byte DIRB = 13;     // Direction control for motor B
 


void setup()
{
  Serial.begin(9600);       // 시리얼 포트 설정
  mySerial.begin(9600);      // 선언된 객체와 시리얼 포트 설정
  pinMode(pinECHO, INPUT);
  pinMode(pinTRIG, OUTPUT);
  pinMode(sensorLeft, INPUT);
  pinMode(sensorRight, INPUT);
  pinMode(sensorCenter, INPUT);

  Timer1.initialize(TIMER_US);                        // Initialise timer 1
  setupArdumoto();      // Set all pins as outputs
}
 
void loop()
{
  val1 = distance/58;
  
  // line sensing
  center_data=  analogRead(sensorCenter);
  left_data =  analogRead(sensorLeft);
  right_data =  analogRead(sensorRight);

  // line traiser
  if(left_data > 500){  // 우측바퀴만 전진  // 좌회전
      driveArdumoto(MOTOR_LEFT, FORWARD, 00);
      driveArdumoto(MOTOR_RIGHT, BACKWARD, 120);
    }
    else if(right_data > 500){  // 좌측 바퀴만 전진  // 우회전
      driveArdumoto(MOTOR_LEFT, BACKWARD, 120);
      driveArdumoto(MOTOR_RIGHT, FORWARD, 0);
    }
    else if((left_data<500) && (center_data >500) && (right_data<500)){ // 전진
      driveArdumoto(MOTOR_LEFT, BACKWARD, 120);
      driveArdumoto(MOTOR_RIGHT, BACKWARD, 120);
    }
    else if((left_data<500) && (center_data >500) && (right_data>500)){ // 약간 우회전
      driveArdumoto(MOTOR_LEFT, BACKWARD, 100);
      driveArdumoto(MOTOR_RIGHT, FORWARD, 0);
    }
    else if((left_data < 300) && (center_data < 300) && (right_data < 300) && (count >= 5)){
        delay(1000);

        while(1){
          center_data2=  analogRead(sensorCenter);
          left_data2 =  analogRead(sensorLeft);
          right_data2 =  analogRead(sensorRight);

          robotForward(30);
          
          if((left_data2 > 300) || (center_data2 > 300) || (right_data2 > 300)){
            robotStop();
          }
        }
      
      }
    else{
      driveArdumoto(MOTOR_LEFT, BACKWARD, 120);
      driveArdumoto(MOTOR_RIGHT, BACKWARD, 120);
    }

  Serial.println(val1);
  // case by obstacle
  if(val1 <12 && val1 > 3) {
    switch(count) {
      case 1:
      
      //오른쪽
      driveArdumoto(MOTOR_LEFT, BACKWARD, 130);
      driveArdumoto(MOTOR_RIGHT, FORWARD, 130);
      delay(300);
      
      //전진
      driveArdumoto(MOTOR_LEFT, BACKWARD, 180);
      driveArdumoto(MOTOR_RIGHT, BACKWARD, 180);
      delay(750);
      
      //왼쪽
      driveArdumoto(MOTOR_LEFT, FORWARD, 160);
      driveArdumoto(MOTOR_RIGHT, BACKWARD, 160);
      delay(440);
/*
      do{ // 전진
        //center_data2=  analogRead(sensorCenter);
        left_data2 =  analogRead(sensorLeft);
        right_data2 =  analogRead(sensorRight);
        Serial.print(left_data2);
        Serial.print("   ");
        Serial.print(right_data2);
        Serial.println("");
        driveArdumoto(MOTOR_LEFT, BACKWARD, 130);
        driveArdumoto(MOTOR_RIGHT, BACKWARD, 120);
      }while(left_data2>400 && right_data2 <400);
*/
      // 여기 부분을 인터럽트 써야 할 듯 
      // 이유 : 위에 코드는 analogRead() 함수를 딱 한번만 수행하기 때문에 전진하지 않음.
      Timer1.attachInterrupt( timerIsr );
      robotStop();
      delay(200);
      count++;
      break;
 
    case 2:
       //후진
      driveArdumoto(MOTOR_LEFT, FORWARD, 120);
      driveArdumoto(MOTOR_RIGHT, FORWARD, 120);
      delay(450);
      
      //오른쪽 --> time : 600
      driveArdumoto(MOTOR_LEFT, BACKWARD, 130);
      driveArdumoto(MOTOR_RIGHT, FORWARD, 130);
      delay(500);
      
      //전진
      driveArdumoto(MOTOR_LEFT, BACKWARD, 150);
      driveArdumoto(MOTOR_RIGHT, BACKWARD, 150);
      delay(550);
      
      //왼쪽
      driveArdumoto(MOTOR_LEFT, FORWARD, 130);
      driveArdumoto(MOTOR_RIGHT, BACKWARD, 130);
      delay(600);
      
      Timer1.attachInterrupt( timerIsr );
      robotStop();
      delay(300);

      //오른쪽으로 살짝 틀어줘야 라인을 잘 따라감 (안그럼 반대로 감)
//      driveArdumoto(MOTOR_LEFT, BACKWARD, 160);
//      driveArdumoto(MOTOR_RIGHT, BACKWARD, 20);
//      delay(300);
      driveArdumoto(MOTOR_LEFT, BACKWARD, 130);
      driveArdumoto(MOTOR_RIGHT, FORWARD, 0);
      delay(300);

      
      count++;
      break;
 
      case 3:
      //오른쪽
      driveArdumoto(MOTOR_LEFT, BACKWARD, 120);
      driveArdumoto(MOTOR_RIGHT, FORWARD, 120);
      delay(350);
      
      //전진
      driveArdumoto(MOTOR_LEFT, BACKWARD, 170);
      driveArdumoto(MOTOR_RIGHT, BACKWARD, 170);
      delay(870);
      
      //왼쪽
      driveArdumoto(MOTOR_LEFT, FORWARD, 160);
      driveArdumoto(MOTOR_RIGHT, BACKWARD, 160);
      delay(500);
 
      Timer1.attachInterrupt( timerIsr );
      robotStop();
      delay(200);
      count++;
      break;
 
      case 4:
       //후진
      driveArdumoto(MOTOR_LEFT, FORWARD, 130);
      driveArdumoto(MOTOR_RIGHT, FORWARD, 130);
      delay(400);
 
       //왼쪽
      driveArdumoto(MOTOR_LEFT, FORWARD, 130);
      driveArdumoto(MOTOR_RIGHT, BACKWARD, 130);
      delay(400);
      
      //전진
      driveArdumoto(MOTOR_LEFT, BACKWARD, 150);
      driveArdumoto(MOTOR_RIGHT, BACKWARD, 150);
      delay(500);
      
      //오른쪽
      driveArdumoto(MOTOR_LEFT, BACKWARD, 120);
      driveArdumoto(MOTOR_RIGHT, FORWARD, 120);
      delay(400);
      
      Timer1.attachInterrupt( timerIsr );
      robotStop();
      delay(200);
      count++;
      break;

    }
  }

  
  
  // obstacle sensing
  wave();
}

void timerIsr()
{
    //trigger_pulse();                                 // Schedule the trigger pulses
    center_data2 =  analogRead(sensorCenter);
    left_data2 =  analogRead(sensorLeft);
    right_data2 =  analogRead(sensorRight);

    if((left_data2 < 300) && (center_data2 < 300) && (right_data2 < 300)){
      driveArdumoto(MOTOR_LEFT, BACKWARD, 85);
      driveArdumoto(MOTOR_RIGHT, BACKWARD, 85);
    }
    else{
      Timer1.detachInterrupt();
    }
    
}


void wave(){
  digitalWrite(pinTRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTRIG, LOW);
  //delayMicroseconds(2);
  distance = pulseIn(pinECHO, HIGH);
  int val1 = distance/58;  // 펄스시간을 거리로 계산
  mySerial.write(val1);
  mySerial.print(val1);
  mySerial.println("cm");
  //Serial.println(val1);
  delay(50);
}
  // setupArdumoto initialize all pins
void setupArdumoto()
{
  // All pins should be setup as outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  // Initialize all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}
  // stopArdumoto makes a motor stop
void stopArdumoto(byte motor)
{
  driveArdumoto(motor, 0, 0);
}
 
void driveArdumoto(byte motor, byte dir, byte spd)
{
  if(motor == MOTOR_LEFT)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if(motor == MOTOR_RIGHT)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  } 
}
 
void robotRight(int velocity)  // 오른쪽으로
{
  driveArdumoto(MOTOR_LEFT, FORWARD, velocity);
  driveArdumoto(MOTOR_RIGHT, FORWARD, velocity);
}
void robotForward(int velocity)  //전진
{
  driveArdumoto(MOTOR_LEFT, BACKWARD, velocity);
  driveArdumoto(MOTOR_RIGHT, BACKWARD, velocity);
}
 
void robotRightTurn(int velocity){   //우회전
  driveArdumoto(MOTOR_LEFT, FORWARD, 0);
  driveArdumoto(MOTOR_RIGHT, FORWARD, velocity);
}
 
void robotBackward(int velocity)  // 후진
{
  driveArdumoto(MOTOR_LEFT, BACKWARD, velocity);
  driveArdumoto(MOTOR_RIGHT, FORWARD, velocity);
}
 
void robotLeft(int velocity)  // 왼쪽으로
{
  driveArdumoto(MOTOR_LEFT, BACKWARD, velocity);
  driveArdumoto(MOTOR_RIGHT, BACKWARD, velocity);
}
void robotStop()
{
  stopArdumoto(MOTOR_LEFT);
stopArdumoto(MOTOR_RIGHT);
}
