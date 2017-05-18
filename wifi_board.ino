#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
SocketIOClient client;
const char* ssid     = "WSN_LAB_2.4G-1";
const char* password = "wsn62283228";
//用来防止PM2.5传感器崩溃，仅此一句。
char host[] = "10.107.30.55";
int port = 8887;
char *s="";
byte buffer[20];
byte recv[40];//储存接收的数据包，用于计算实际数据
byte ret[2];
char data[3];
char string[25];
int PM2_5,PM10,PM1_0,TEMP,HUMI;
float TVOC,HCHO;

extern String RID;
extern String Rname;
extern String Rcontent;

void Blink(int fre){
  switch(fre){
    case 1:
      Serial.print('h');
      break;
    case 0:
      Serial.print('i');
      break;
}
}
int get_PM2_5()
{
    int pm2_5 = 0;
    Serial.print('a');
    while(Serial.available()<0){}
    pm2_5 = Serial.read();
    return pm2_5;
}
int get_PM10()
{
    int pm10 = 0;
    Serial.print('b');
    while(Serial.available()<0){}
    pm10 = Serial.read();
    return pm10;
}
int get_PM1_0()
{
    int pm1_0 = 0;
    Serial.print('c');
    while(Serial.available()<0){}
    pm1_0 = Serial.read();
    return pm1_0;
}
int get_Temp()
{
    int temp = 0;
    Serial.print('d');
    while(Serial.available()<0){}
    temp = Serial.read();
    return temp;
}
int get_Humi()
{
    int humi = 0;
    Serial.print('e');
    while(Serial.available()<0){}
    humi = Serial.read();
    return humi;
}
float get_Tvoc()
{
    float tvoc = 0;
    Serial.print('f');
    while(Serial.available()<0){}
    tvoc = Serial.read();
    return tvoc;
}
float get_HCHO()
{
    float hcho = 0;
    Serial.print('g');
    while(Serial.available()<0){}
    hcho = Serial.read();
    return hcho;
}

void publishdata(char * s)
{
      int count = 0;
      client.send("arduinoPublish", "publishData", s);
     while(true)
     {
        if (client.monitor()) 
        {
          if (RID == "arduinoPublishSucceed" && Rname == "info") 
          {
            Serial.println(s);
            Serial.println("publish success！！");
            Blink(1);
            delay(2000);
            break;
          } 
          else 
          {
            Blink(0);
            count++;
            delay(1000);
            if(count >= 10)
            {
              count = 0;
              break;
             }
          }
        }
     }
      Blink(1);
    }
   

void login()
{
  int count2 = 0;
  if (client.connected())
  {
    client.send("arduinoAuth", "userData", "Thread///Thread");
   while (true)
    {
      if (client.monitor()) 
      {
        if (RID == "arduinoAuthSucceed" && Rname == "message") 
        {
          Blink(1);
          Serial.println("login success!!");
        } 
        else if (RID == "arduinoAuthFailed" && Rname == "message") 
        {
          Blink(0);
          break;
        }
      }
      else
      {
        if(count2 >= 10)
        {
          count2 = 0;
          break;
        }
        else
        {
          count2++;
          delay(1000);  
        }
      }
    }
  }
  else
  {
    Blink(0);
  }
 Blink(1);
}

void connecting()
{
  if (!client.connect(host, port)) 
  {
    Serial.println("connect failed.");
    delay(2000);
    connecting();
  } 
  else 
  {
    Serial.println("connect success.");
    //连接成功后调用登录函数
    login();
  }
  Serial.println("connecting completed");
}


void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
  Serial.println("connect to net success！！");
  connecting();
}


void loop() 
{ 
  
  PM2_5 = get_PM2_5();
   itoa(PM2_5,string,10);
   s = "indoor/sensor7/Thread/sensor7/PM2_5/";
   strcat(s, string);
   publishdata(s);
   
  delay(1000); 
  PM10 = get_PM10();
   itoa(PM10,string,10);
   s = "indoor/sensor7/Thread/sensor7/PM10/";
   strcat(s, string);
   publishdata(s);
   
  delay(1000);  
  PM1_0 = get_PM1_0();
    itoa(PM1_0,string,10);
    s = "indoor/sensor7/Thread/sensor7/PM1_0/";
    strcat(s, string);
    publishdata(s);
    
  delay(1000);   
  TEMP = get_Temp();
    itoa(TEMP,string,10);
    s = "indoor/sensor8/Thread/sensor8/TEMP/";
    strcat(s, string);
    publishdata(s);
    
  delay(1000);   
  HUMI = get_Humi();
    itoa(HUMI,string,10);
    s = "indoor/sensor8/Thread/sensor8/HUMI/";
    strcat(s, string);
    publishdata(s);
    
  delay(1000);  
  TVOC = get_Tvoc();
    dtostrf(TVOC,1,2,string);
    s = "indoor/sensor9/Thread/sensor9/TVOC/";
    strcat(s, string);
    publishdata(s);
    
  delay(1000);
  HCHO = get_HCHO();
    dtostrf(HCHO,1,2,string);
    s = "indoor/sensor9/Thread/sensor9/HCHO/";
    strcat(s, string);
    publishdata(s);
   
  delay(6000);
}


