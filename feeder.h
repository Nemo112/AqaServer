#ifndef feeder
#define feeder
#define motorPin1  6
#define motorPin2  7
#define motorPin3  8
#define motorPin4  9 
#define delayTime 10


void turnAround(){
  for(int i=0;i<1000;i++){
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, HIGH);
    delay(delayTime);
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, HIGH);
    delay(delayTime);
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
    delay(delayTime);
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
    delay(delayTime);
  }
}


#endif
