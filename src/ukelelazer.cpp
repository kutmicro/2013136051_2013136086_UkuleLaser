#include <SoftwareSerial.h>
#define btn1  11  // 버튼1의 아두이노 핀번호 정의
#define btn2  10  // 버튼2의 아두이노 핀번호 정의
#define btn3  9   // 버튼3의 아두이노 핀번호 정의
#define btn4  8   // 버튼4의 아두이노 핀번호 정의
#define laz1 A0
// 3:midi rx , 4:midi reset  아두이노 핀 3번 4번은 이미 사용중

SoftwareSerial mySerial(2, 3); //SW시리얼핀 정의 D3이 MIDI신호 전송용,  D2는 미사용 

byte note = 0; //The MIDI연주될 note(음계)
byte resetMIDI = 4; // VS1053 Reset용 핀
byte ledPin = 13; //MIDI 트래픽 표시용 LED

boolean br1;  // 버튼1 상태 확인용 입력값 임시저장용
boolean br2;  // 이하, 위와 유사
boolean br3;
boolean br4;

int lazVal = 0;
int check = 0;
int patch = 0; //악기 대응, 연주될 악기 종류 (0~127: 기본 128 가지 선택가능)

int note60 = 60; //버튼1의  note(음계)  가령 "도"  0~127까지 지정가능 (정확한 음계 설정은 MIDI관련정보참고)
int note62 = 62; //버튼2의  note(음계)  가령 "레"
int note64 = 64; //버튼3의  note(음계)  가령 "미"
int note65 = 65; //버튼4의  note(음계)  가령 "파"
int note67 = 67; //버튼4의  note(음계)  가령 "파"
int note69 = 69; //버튼4의  note(음계)  가령 "파"
int note71 = 71; //버튼4의  note(음계)  가령 "파"

byte byteData;

void setup() {
  Serial.begin(38400);

  //미디컨트롤을 위한 소프트웨어 시리얼을 준비합니다.
  mySerial.begin(38400);

  //VS1053를 초기화하고 사용할 준비를 합니다.
  pinMode(resetMIDI, OUTPUT);
  digitalWrite(resetMIDI, LOW);
  delay(100);
  digitalWrite(resetMIDI, HIGH);
  delay(100);

  pinMode( btn1, INPUT);      // 버튼1 입력용 핀모드를  입력모드로 전환
  digitalWrite( btn1, HIGH);  // 내부 PullUp 설정, 스위치의 나머지 한선은 GND에 물리면 됩니다.(초간단)
  pinMode( btn2, INPUT);      // 이하, 위와 유사
  digitalWrite( btn2, HIGH);
  pinMode( btn3, INPUT);
  digitalWrite( btn3, HIGH);
  pinMode( btn4, INPUT);
  digitalWrite( btn4, HIGH);
}

void loop() {
  br1 = digitalRead(btn1);
  br2 = digitalRead(btn2);
  br3 = digitalRead(btn3);
  br4 = digitalRead(btn4);

  lazVal = analogRead(laz1); //read analog 
  
  check = setNote1(br1, lazVal) + setNote2(br2, lazVal) + setNote3(br3, lazVal) + setNote4(br4, lazVal);
/*  
  if(setNote1(br1, lazVal) == 59 || setNote1(br2, lazVal) == 59 || (br3, lazVal) == 59 || setNote1(br4, lazVal) == 59) {
    noteOn(0, 59, 100);
    delay(500);
    noteOff(0, 59, 0);
    Serial.print("default");
    Serial.print("\n");
  }  */ 
  if(check==1000) {
    noteOn(0, note60, 100);
    delay(500);
    noteOff(0, note60, 0);
    Serial.print(1000);
  }
  else if(check==111){
    noteOn(0, note62, 100);
    delay(500);
    noteOff(0, note62, 0);
    Serial.print(111);
  } 
  else if(check==100){
    noteOn(0, note64, 100);
    delay(500);
    noteOff(0, note64,0);
    Serial.print(100);
  }
  else if(check==101){
    noteOn(0, note65,100);
    delay(500);
    noteOff(0, note65,0);
    Serial.print(101);
  }
  else if(check==1110){
    noteOn(0, note67,100);
    delay(500);
    noteOff(0, note67,0);
    Serial.print(1110);
  }
  else if(check==11){
    noteOn(0, note69,100);
    delay(500);
    noteOff(0, note69,0);
    Serial.print(11);
  }
  else if(check==1111){
    noteOn(0, note71,100);
    delay(500);
    noteOff(0, note71,0);
    Serial.print(1111);
  }
  if(Serial.available() > 0)
  {
    byteData =  Serial.read();
    mySerial.write( byteData );
  }  
  delay(50);
}
int setNote1(boolean bnt, int val){
  if (val < 900) {
    if (bnt == true)
      return 1;
  }
  else 
    return 0;
}
int setNote2(boolean bnt, int val){
  if (val < 900) {
    if (bnt == true)
      return 10;
  }
  else 
    return 0;
}
int setNote3(boolean bnt, int val){
  if (val < 900) {
    if (bnt == true)
      return 100;
  }
  else 
    return 0;
}
int setNote4( boolean bnt, int val){
  if (val < 900) {
    if (bnt == true)
      return 1000;
  }
  else 
    return 0;
}
//노트 온 미디 메세지를 송출합니다. 버튼이 눌린상태와 같습니다.
//채널 범위는 0-15 입니다.
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//노트 오프 미디 메세지를 송출합니다. 버튼이 눌리지 않은 상태와 같습니다.
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

void talkMIDI(byte cmd, byte data1, byte data2) {
  digitalWrite(ledPin, HIGH);
  mySerial.write(cmd );
  mySerial.write(data1 );

  //모든 명령은 1바이트를 지니며, 모든 cmds는   0xBn보다  2 데이터 바이트를 덜 지닙니다.  
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if( (cmd & 0xF0) <= 0xB0)
    mySerial.write(data2 );

  digitalWrite(ledPin, LOW);
}