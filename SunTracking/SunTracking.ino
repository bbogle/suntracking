#include <Servo.h>

#define SERVOPINH  5 //수평서보
#define SERVOPINV  6 //수직서보

#define dtime   50  //지연매개변수, 값이 작을수록 해당 속도가 빨라지며 그 반대의 경우도 마찬가지（10~100） 
#define tol   50    //조도의 해당 범위가 작을수록 더 민감하고 반대로 느립니다. 
                    //(값은 10~100이고, 다른 주변 광도에 따라 감도가 다르며 실내 광원이 크게 변하고 변화가 작습니다.)
/* 위의 2개 매개변수는 너무 작습니다. 빛의 미묘한 변화에 극도로 민감하여 지터가 발생합니다.
   지터를 제거하기 위해 필터링을 사용하거나 매개변수를 조정하여 응답시간 또는 감도를 늦출 수 있습니다.*/

// 수평서보 설정
Servo horizontal; // 수평서보
int servoh = 90;   // 기본각도

int servohLimitHigh = 175;  //좌우각도 max
int servohLimitLow = 5;     //좌우각도 min

// 수직서보 설정
Servo vertical;     // 수직서보
int servov = 90;    // 기본각도

int servovLimitHigh = 180;  //상하각도 max
int servovLimitLow = 90;    //최대앙각이 너무 커서 센서가 프레임에 닿을 수 있습니다.


// 4개 센서용 배선
#define ldrlt  A2
#define ldrrt  A3
#define ldrld  A1
#define ldrrd  A0


void setup()
{
  Serial.begin(115200);
  horizontal.attach(SERVOPINH);
  vertical.attach(SERVOPINV);
  horizontal.write(servoh);
  vertical.write(servov);
  delay(100);

  //테스트실행
  //걸림이 있는지 확인하면서 수직 축의 작동을 테스트합니다.
  for (int i = servovLimitLow; i < servovLimitHigh; i += 2)
  { vertical.write(i);
    delay(30);
  }
  vertical.write((servovLimitLow + servovLimitHigh) / 2);
  delay(100);
  //테스트레벨
  for (int i = 0; i < 180; i += 2)
  { horizontal.write(i);
    delay(30);
  }
  horizontal.write((servohLimitHigh + servohLimitLow) / 2);
  //테스트에 문제가 없으면 여기에서 코드를 제거할 수 있습니다.

}

void loop()
{
  int lt = analogRead(ldrlt); //
  int rt = analogRead(ldrrt); //
  int ld = analogRead(ldrld); //
  int rd = analogRead(ldrrd); //4개 센서 각각의 조도 값 읽기

  int avt = (lt + rt) / 2; //
  int avd = (ld + rd) / 2; //
  int avl = (lt + ld) / 2; //
  int avr = (rt + rd) / 2; //평균인접 판독값

  int dvert = avt - avd; //
  int dhoriz = avl - avr;//그런 다음 위쪽 및 아래쪽 행의 평균과 왼쪽 및 오른쪽 행의 평균 계산

  Serial.print(lt);
  Serial.print(",");
  Serial.print(rt);
  Serial.print(",");
  Serial.print(ld);
  Serial.print(",");
  Serial.print(rd);
  Serial.print ("      |    ");

  Serial.print(avt);
  Serial.print(",");
  Serial.print(avd);
  Serial.print(",");
  Serial.print(avl);
  Serial.print(",");
  Serial.print(avr);
  Serial.print(",   ");
  Serial.print(dtime);
  Serial.print(",   ");
  Serial.println(tol);    
  //직렬 포트는 판독 값을 출력합니다. 디버깅이 정상인 후 위 시리얼출력코드를 삭제하여 속도를 향상시킬 수 있습니다.

  //차이가 허용 오차 내에 있는지 확인하고 그렇지 않으면 수직 각도를 변경합니다.
  if (-1 * tol > dvert || dvert > tol)
  {
    if (avt > avd)
    {
      servov = ++servov;
      if (servov > servovLimitHigh)
      {
        servov = servovLimitHigh;
      }
    }
    else if (avt < avd)
    {
      servov = --servov;
      if (servov < servovLimitLow)
      {
        servov = servovLimitLow;
      }
    }
    vertical.write(servov); //서보의 회전 각도가 빛과 반대인 경우 (180-servo) 또는 (servo)를 사용해 방향을 변경합니다.
  }

  //차이가 허용 오차 내에 있는지 확인하고 그렇지 않으면 수평 각도를 변경합니다.
  if (-1 * tol > dhoriz || dhoriz > tol)
  {
    if (avl > avr)
    {
      servoh = --servoh;
      if (servoh < servohLimitLow)
      {
        servoh = servohLimitLow;
      }
    }
    else if (avl < avr)
    {
      servoh = ++servoh;
      if (servoh > servohLimitHigh)
      {
        servoh = servohLimitHigh;
      }
    }
    else if (avl = avr)
    {
      // nothing
    }
    horizontal.write(servoh); //서보의 회전 각도가 빛과 반대인 경우 (180-servo) 또는 (servo)를 사용해 방향을 변경합니다.
  }
  delay(dtime);

}
