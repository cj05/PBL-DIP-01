// C++ code
//
// in ms UNITS, for 1 second type 1000
//modify the timing parameters here
//OFFCD is the Time people have to wait before it is turned on again
//ONCD is the maximum time cars have to wait before it is turned off
//PCD is the Time that will turn off after last sensor detection
//PWNCD is the Time that will warn before turning off after last sensor detection
//WNCD is the Time that will warn before turning off for maximum limit
//BLDEL is time of the OFF blink
//BLON is the time of ON blink

#define OFFCD 9000
#define ONCD 30000 
#define WCD 3000 
#define PCD 12000 
#define PWNCD 5000
#define WNCD 5000
#define BLDEL 500
#define BLON 500

// do not modify
int state = 0;
long long timer = 0;
long long del = 0;
long long Pdel = 0;
bool k = false;

void setup()
{
  //initializing
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(9, 255);
  Serial.begin(9600);
  state = 0;
  del=0;
  delay(3000);
}

void loop()
{
  //loop
  while (state == 0) {
    timer = millis();
    if ((digitalRead(3)||digitalRead(2))&&del<=0) {
      state = 1;
      del = WCD;
    } else {
      analogWrite(5, 255);
      analogWrite(6, 0);
      analogWrite(9, 255);
      digitalWrite(12, LOW);
      digitalWrite(11, LOW);
      digitalWrite(13, LOW);
      del+=timer-millis();
      del=del<0?0:del;
    }
  }
  while (state == 1){
    timer = millis();
    if(del<=0){
      state = 2;
      del = ONCD;
      Pdel = PCD;
    }
    analogWrite(5, 200);
    analogWrite(6, 0);
    analogWrite(9, 255);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(13, (del%(BLDEL+BLON)<BLON)?1:0);
    del+=timer-millis();
    del=del<0?0:del;
  }
  while (state == 2) {
    timer = millis();
    if (del<=0||Pdel<=0) {
      state = 0;
      del = OFFCD;
    } else {

      if (digitalRead(3)||digitalRead(2)){
        Pdel = PCD;
        //Serial.println("Triggered!!!!");
      }
        
      if(del<WNCD||Pdel<PWNCD){
        k = min(del,Pdel)%(BLDEL+BLON)<BLON;
        analogWrite(6, (k)?0:255);
        analogWrite(5, 0);
        analogWrite(9, 255);
        digitalWrite(12, (k)?1:0);
        analogWrite(11, (k)?200:0);
        digitalWrite(13, (del%(BLDEL+BLON)<BLON)?1:0);
      }
      else{
        analogWrite(6, 255);
        analogWrite(5, 0);
        analogWrite(9, 255);
        digitalWrite(12, HIGH);
        analogWrite(11, 200);
        digitalWrite(13, (del%(BLDEL+BLON)<BLON)?1:0);
      }
      del+=timer-millis();
      Pdel+=timer-millis();
      del=del<0?0:del;
      Pdel=Pdel<0?0:Pdel;
    }
  }
}
//cj code please dont modify it
