int t=250;
void setup() {
 Serial.begin(9600);
 Serial1.begin(9600);
 Serial2.begin(9600);
 Serial3.begin(115200);
}

void loop() {
 while(Serial3.available()){
  char read = Serial3.read();
    switch(read){
    case 'a':
      Serial3.print(getPM2_5());
      break;
    case 'b':
      Serial3.print(getPM10());
      break;
    case 'c':
      Serial3.print(getPM1_0());
      break;
    case 'd':
      Serial3.print(getTemp());
      break;
    case 'e':
    Serial3.print(gethumi());
      break;
    case 'f':
    Serial3.print(getTVOC(),3);
      break;
   case 'g':
   Serial3.print(getCH_4(),3);
   break;
    case 'h':
      Blink(300);
   break;
    case 'i':
      Blink(800);
   break;
   default:
   break;
    
  }
 }
}

int getPM2_5(){
int PM2_5 = 0;
byte ret[2];
byte buffer[65];
  Serial1.readBytes(buffer, 64);
  for (int i = 0; i <33; i++) {
    if ((buffer[i]==66)&&((buffer[i+1]==77))) {
      ret[0] = buffer[i + 6];
      ret[1] = buffer[i + 7];
      PM2_5  = (int)(ret[1] + (ret[0] *256));
      break;
  }
}
 return PM2_5;
}

int getPM10(){
int PM10 = 0;
byte ret[2];
byte buffer[65];
  Serial1.readBytes(buffer, 64);
  for (int i = 0; i <33; i++) {
    if ((buffer[i]==66)&&((buffer[i+1]==77))) {
      ret[0] = buffer[i + 8];
      ret[1] = buffer[i + 9];
      PM10 = (int)(ret[1] + (ret[0] * 256));
     
      break;
  }
}
 return PM10;
}

int getPM1_0(){
int PM1_0 = 0;
byte ret[2];
byte buffer[65];
  Serial1.readBytes(buffer, 64);
  for (int i = 0; i <33; i++) {
    if ((buffer[i]==66)&&((buffer[i+1]==77))) {
      ret[0] = buffer[i + 4];
      ret[1] = buffer[i + 5];
      PM1_0 = (int)(ret[1] + (ret[0] * 256));
     
      break;
  }
}
 return PM1_0;
}

int getTemp(){
byte inBytes[40];
memset(inBytes,0,40);
int temph,templ;
int temp;
   Serial2.readBytes(inBytes,40);
       for(int i=0;i<17;i++){
      if((inBytes[i]==170) && (inBytes[i+1]==13) && (inBytes[i+2]==3)){
         temph = (int) inBytes[i+11];
         templ = (int) inBytes[i+12];
         temp = ((int)(temph*256+templ))/10;
         break;
      }    
}
 
 return temp;
}
int gethumi(){
byte inBytes[40];
memset(inBytes,0,40);
int humih,humil;
int humi;
   Serial2.readBytes(inBytes,40);
       for(int i=0;i<17;i++){
      if((inBytes[i]==170) && (inBytes[i+1]==13) && (inBytes[i+2]==3)){
         humih = (int) inBytes[i+9];
         humil = (int) inBytes[i+10];
         humi = ((int)(humih*256+humil))/10;  
          
          break;
      }   
}

return humi;
}

float getTVOC(){
byte inBytes[40];
memset(inBytes,0,sizeof(byte)*40);
int tvoch,tvocl;
float tvoc = 0.00;
   Serial2.readBytes(inBytes,40);
       for(int i=0;i<40;i++){
      if((inBytes[i]==170) && (inBytes[i+1]==13) && (inBytes[i+2]==3)){
         tvoch = (int) inBytes[i+7];
         tvocl = (int) inBytes[i+8];
         if(tvoch == 255 && tvocl == 255){
          continue;
         }
         else tvoc = ((float)(tvoch*256+tvocl))/100;
         break;
      } 
}

return abs(tvoc);
}

float getCH_4(){
byte buffer[10];
memset(buffer,0,10);
int conf=0;
float data=0.0;
int datah=0;
int datal=0;

int INFO[7] = {0x42,0x4d,0x01,0x00,0x00,0x00,0x90};
          for(int j=0;j<7;j++){
          Serial.write(INFO[j]);
         }
         while(!Serial.available()){}
         Serial.readBytes(buffer,10);
         switch(buffer[5]){
          case 01:
            conf = 1;
            break;
          case 02:
            conf = 10;
            break;
          case 03:
            conf = 100;
            break;
          case 04:
            conf = 1000;
            break;
         }
         datah = ((int)buffer[6]);
         datal = ((int)buffer[7]);;
         data = ((float)(datah*256+datal))/conf;
         
       return data;   
}
void Blink(int i){
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(i);                       // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(i);  
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(i);                       // wait for a second
  digitalWrite(13, LOW);    
}
