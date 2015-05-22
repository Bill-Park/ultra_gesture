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
  //  Serial.print("distance 1 :  ") ;
  //  Serial.println(distance1) ;

  if (distance1 < 1000) {
    flag = 1 ;
  }
  else if (distance2 < 1000) {
    flag = 2 ;
  }
  if (flag == 1) {
    long time = millis() ;
    while (1) {
      if (time < millis() - 300) {
        if (ultra(trig1, echo1) < 1000) {
          Consumer.write(MEDIA_VOLUME_MUTE) ;
          delay(100) ;
        }
        flag = 0 ;
        break ;
      }
      if (ultra(trig2, echo2) < 1000) {
        Consumer.write(MEDIA_VOLUME_UP) ;
        delay(80) ;
        time = millis() ;
      }
    }
  }
  else if (flag == 2) {
    long time = millis() ;
    while (1) {
      if (time < millis() - 300) {
        if (ultra(trig2, echo2) < 1000) {
          Consumer.write(MEDIA_VOLUME_MUTE) ;
          delay(100) ;
        }
        flag = 0 ;
        break ;
      }
      if (ultra(trig1, echo1) < 1000) {
        Consumer.write(MEDIA_VOLUME_DOWN) ;
        delay(80) ;
        time = millis() ;
      }
    }
  }
  //  delay(300) ;
  
  if (digitalRead(play) == LOW) {
    while (digitalRead(play) == LOW) ;
    Consumer.write(MEDIA_PLAY_PAUSE) ;
  }
  
}

long ultra(int trig, int echo)
{
  long duration[3] ;
  for (int i = 0; i < 3; i++) {
    digitalWrite(trig, LOW) ;
    delayMicroseconds(2) ;
    digitalWrite(trig, HIGH) ;
    delayMicroseconds(10) ;
    digitalWrite(trig, LOW) ;
    duration[i] = pulseIn(echo, HIGH) ;
  }

  return (duration[0] + duration[1] + duration[2]) / 3 ;
}
