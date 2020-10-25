//RFID reads the tag by Arduino UNO
unsigned int status = 0;
unsigned char val,RSSI,I,Q;
unsigned int num=1;
unsigned char Tag_num;
unsigned int flag=0;
unsigned char SCMD[] = {0x43,0x03,0x01}; //發送 43 03 01 會回傳EPC ID 或者
unsigned char MyEPCID[]={1,2,3,4,5,6,7,8,9,16,17,18};
unsigned char Rcard[12];
unsigned char Freq[3];
unsigned int count=0;

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
                                   }                                      
                  break;  
              default:
                  break;
              }
          }
}

void setup()
{
  Serial.begin(115200);
//  mySerial.begin(115200);
  Serial.write(SCMD,3);
}

void loop()
{
// if (status == 0)  
//     mySerial.write(SCMD,3);
  delay(500);
  scanwithRSSI();
  delay(500);
  Serial.write(SCMD,3);
  delay(5);
  for(int i=0;i<12;i++){
    if(MyEPCID[i]==Rcard[i]){
      count++;
      Rcard[i]=0;
    }
  }
  if(count==12){
    Serial.print("It is the right EPC ID");
  }
  count=0;
}
