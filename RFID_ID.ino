//RFID reads the tag by Arduino UNO
unsigned int status = 0;
unsigned char val,RSSI,I,Q;
unsigned int num=1;
unsigned char Tag_num;
unsigned int flag=0;
unsigned char SCMD[] = {0x43,0x03,0x01}; //發送 43 03 01 會回傳EPC ID 或者
unsigned char MyEPCID1[]={1,2,3,4,5,6,7,8,9,16,17,18};
unsigned char MyEPCID2[]={1,2,3,4,5,6,7,8,9,16,17,19};
unsigned char Rcard[12];
unsigned char RcardInit[12]={0};
unsigned int flag=0;
unsigned char Freq[3];
unsigned int count1=0;
unsigned int count2=0;
int AnalogPin=3;

//#include <SoftwareSerial.h>

void scanwithRSSI(void)
{
  //delay(10);
       while(Serial.available() > 0) {
            val = Serial.read();            
            switch(status) {
              case 0:  // parse 0x44
                   if (val == 0x44) 
                          {status = 1; /*Serial.write(val);*/}
                   break;
              case 1:   //parse 0x16
                   if (val == 0x16) 
                          {status = 2; /*Serial.write(val);*/}
                       else if (val == 0x05)  
                            //{Serial.print("No Tag"); }
                          {status = 22; /*Serial.write(val);*/}
                    break;
              case 2:
                   if (val > 0) 
                          {status = 3; Tag_num = val; /*Serial.write(val);*/}
                   break;
              case 3:
                   RSSI = val; 
                   //I = (val&0x0F)*2; Q = (val>>4)*2;
                   //Serial.write(val);
                   //Serial.print(RSSI);
                   status = 4;
                   break;
              case 4:
              case 5:
              case 6:
                  Freq[status - 4] = val;
                  status ++;
                  //Serial.write(val);
                  break;
              case 7:
                  if (val == 0x0E) 
                     {status = 8; /*Serial.write(val);*/}
                  break;
              case 8:
                  if (val == 0x30)
                     {
                     status = 9; /*Serial.write(val);*/
                     }
                  break;
              case 9:
                  if (val == 0x00)
                     {status = 10; /*Serial.write(val);*/}
                  break;
              case 10:
              case 11:
              case 12:
              case 13:
              case 14:
              case 15:
              case 16:
              case 17:
              case 18:
              case 19:
              case 20:
              case 21:
                  Rcard[status - 10] = val;
                  if (status == 10) {Serial.print(num); Serial.print(';'); Serial.print(' ');}    
                  status ++;
                  //Serial.write(val);
                  Serial.print(val,HEX);
                  Serial.print(' ');
                  if (status == 22) 
                     { status = 0; num++; 
                       Serial.println(' ');
                       if (Tag_num == 1)  
                               num=1;                              
                      }                 
                  break;
     
              case 22:
                  if (val == 0x00) {status = 23; /*Serial.write(val);*/}
                  break;
              case 23:
                  if (val == 0x00) {status = 24; /*Serial.write(val);*/}
                  break;
              case 24:
                  if (val == 0x00) {status = 0; /*Serial.write(val);*/
                                       Serial.println("No Tag");
                                       flag=1;  //check whether new tag is in or not
                                   }                                      
                  break;  
              default:
                  break;
              }
          }
}

void setup()
{
  pinMode(AnalogPin, OUTPUT); // 設定 pin 3 為輸出
  for(int i=0;i<128;i++){
    analogWrite(AnalogPin,i);
    delay(150);
  }
  Serial.begin(115200);
//  mySerial.begin(115200);
  Serial.write(SCMD,3);
}

void loop()
{
  delay(500);
  scanwithRSSI();
  delay(500);
  Serial.write(SCMD,3);
  delay(5);
  if(flag){                         //check whether new tag is in or not
    for(int i=0;i<12;i++){
    if(MyEPCID1[i]==Rcard[i]){
      count1++;
    }
    if(MyEPCID2[i]==Rcard[i]){
      count2++;
    }
    Rcard[i]=0;
  }
  if(count1==12){
    Serial.print("First Tag");
    flag=0;
    for(int i=127;i<255;i++){       //move from 5V to 10V
      analogWrite(AnalogPin,i);
      delay(150);
    }
    delay(1000);
    for(int i=255;i>128;i--){       //move back to the initial pos
      analogWrite(AnalogPin,i);
      delay(150);
    }
    delay(1000);
  }
  if(count2==12){
    Serial.print("Second Tag");
    flag=0;
    for(int i=127;i>0;i--){         //move from 5V to 0V
      analogWrite(AnalogPin,i);
      delay(150);
    }
    delay(1000);
    for(int i=0;i<128;i++){         //move back to the initial pos
      analogWrite(AnalogPin,i);
      delay(150);
    }
  }
  }
  count1=0;
  count2=0;
}
