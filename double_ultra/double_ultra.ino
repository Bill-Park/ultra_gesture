/*
 Copyright (c) 2014 NicoHood
 See the readme for credit to other people.

 Consumer example (former Media example)

 Press a button to play/pause music player
 See HID Project documentation for more Consumer keys.
 https://github.com/NicoHood/HID/wiki/Consumer-API
*/
int echo1 = 2 ;
int trig1 = 3 ;
int echo2 = 4 ;
int trig2 = 5 ;
int play = 6 ;

int flag = 0 ;
int updown = 0 ;
int tem = 2000 ;

int base_dis = 700 ;

void setup() {
  pinMode(echo1, INPUT) ;
  pinMode(trig1, OUTPUT) ;
  pinMode(echo2, INPUT) ;
  pinMode(trig2, OUTPUT) ;
  pinMode(play, INPUT_PULLUP) ;
  // Sends a clean report to the host. This is important on any Arduino type.
  Consumer.begin();
  Serial.begin(9600) ;
}

void loop() {
  long distance1 = ultra(trig1, echo1) ;
  long distance2 = ultra(trig2, echo2) ;

  if (distance1 < base_dis) {
    flag = 1 ;
  }
  else if (distance2 < base_dis) {
    flag = 2 ;
  }
  if (flag == 1) {
    long time = millis() ;
    while (1) {
      if (time < millis() - 300) {  //0.3초가 지났을 때
        if (ultra(trig1, echo1) < base_dis) {  //여전히 감지된다면
          Consumer.write(MEDIA_VOLUME_MUTE) ;  //mute실행
          delay(100) ;
        }
        flag = 0 ;
        break ;
      }
      if (ultra(trig2, echo2) < base_dis) {  //반대편이 감지된다면
        Consumer.write(MEDIA_VOLUME_UP) ;  //volume up실행
        delay(80) ;
        time = millis() ;  //시간 초기화  
      }
    }
  }
  else if (flag == 2) {
    long time = millis() ;
    while (1) {
      if (time < millis() - 300) {
        if (ultra(trig2, echo2) < base_dis) {
          Consumer.write(MEDIA_VOLUME_MUTE) ;
          delay(100) ;
        }
        flag = 0 ;
        break ;
      }
      if (ultra(trig1, echo1) < base_dis) {
        Consumer.write(MEDIA_VOLUME_DOWN) ;
        delay(80) ;
        time = millis() ;
      }
    }
  }
  
  if (digitalRead(play) == LOW) {  //스위치가 눌리면
    while (digitalRead(play) == LOW) ;  //뗄때까지 대기
    Consumer.write(MEDIA_PLAY_PAUSE) ;  //재생/정지 실행
  }
  
}

long ultra(int trig, int echo)
{
  long duration[3] ;
  for (int i = 0; i < 3; i++) {  //3번 값을 읽고 저장
    digitalWrite(trig, LOW) ;
    delayMicroseconds(2) ;
    digitalWrite(trig, HIGH) ;
    delayMicroseconds(10) ;
    digitalWrite(trig, LOW) ;
    duration[i] = pulseIn(echo, HIGH) ;
  }

  return (duration[0] + duration[1] + duration[2]) / 3 ;  //값의 평균을 출력
}
