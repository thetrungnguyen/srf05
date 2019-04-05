#include <LiquidCrystal.h>
#include <EEPROM.h>
//---------------- lcd ------------------  
LiquidCrystal lcd(12, 11, 5, 6, 9, 10);  // EN,RS,D4,D5,D6,D7

//---------------- define pin ------------------  
#define clk  4 //chan CLK cua volume noi voi chan so 4 cua arduino
#define data 3 //chan data cua volume noi voi chan so 3 cua arduino
#define loa    A4 //chan cua Loa noi voi chan A4 cua arduino
#define TRIG_PIN 8 //chan trig cua sensor noi voi chan so 8 cua arduino
#define ECHO_PIN 7 //chan echo cua sensor noi voi chan so 7 cua arduino
#define TIME_OUT 5000 
int A; // tru?c c?a a
int a;
float biendo=100; //khoang cach loa se thong bao
//---------------- valuable ------------------ 
int mode=0;
int mainmode=0;//trang thai hien tai cua chuong trinh
long distance;
float GetDistance()
{
  //duration: thoi gian tu luc phat xung den khi nhan duoc xung 
  long duration, distanceCm;
  
   //phat song 10 microsecond tu chan Trig sau do Trig tao 1 xung High o Echo den khi nhan duoc xung phan xa tu Echo.
   //khoang cach se bang thoi gian tu luc phat song cho den khi nhan duoc song phan hoi
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  //thoi gian tin hieu phan hoi lai Echo
  duration = pulseIn(ECHO_PIN, HIGH, TIME_OUT);
 
  // tinh khoang cach
  //toc do am thanh trong khong khi la 29.1 microsecond/cm
  //29.1 microsend        =>     1cm
  //duration microsecnd   =>     distanceCm
  // suy ra distanceCm = (duration * 1) / 29,1
  //song duoc phat di va quay tro lai nen phai chia cho 2
  distanceCm = duration / 29.1 / 2;
  
  return distanceCm;
}

void setup() //khong can quan tam
{

//---------------------------- Pinmode---------------------
  /////////////////// input  /////////////////////

    pinMode(2,INPUT_PULLUP);
    pinMode(3,INPUT_PULLUP);
    pinMode(4,INPUT_PULLUP);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);               
//---------------------------- Interrupt ---------------------
  attachInterrupt(0, ngat, LOW);
//---------------------------- LCD , serial ---------------------  
  lcd.begin (16,2);
  Serial.begin(9600);
  A= digitalRead(clk);

}

void loop()
{                                                                                                                                                                                                         
    
  //-------------- dieu kien mode ---------------------  
  if(mode==1)
  {
    detachInterrupt(0); //ngat phan cung
    delay(50);
    lcd.setCursor(0,0);lcd.print("cam bien srf05");
    mode=0;
    mainmode++;
    if(mainmode>2) mainmode=0;
    while(digitalRead(2)==0);lcd.clear();
  }
  attachInterrupt(0, ngat, LOW);
   

      //---------------------------- mode ---------------------  
        if(mainmode==0)//luc do khoang cach binh thuong
        {lcd.setCursor(0,0);lcd.print("CAM  BIEN SRF05 "); 
        dokhoangcach();

  }

        if(mainmode==1)//luc nhan volume lan dau tien de dieu chinh khoang cach
        {     
          up_down();        
          chopbiendo();
          
        }

        if(mainmode==2)//an volume lan 2 de luu khoang cach
        {    
           lcd.setCursor(0,0);lcd.print("luu gia tri");
           delay(1000); mainmode=0;   
          
        }


}

//--------------------------------------- chuong trinh con -------------------------------------------  

void up_down()//dieu chinh khoang cach tren volume
{
a =digitalRead(clk);
if(a != A)
    {if(digitalRead(data) != a )
      {
      biendo = biendo + 1; // chinh len
      if(biendo < 10) biendo = 10;
      lcd.print(biendo); 
      
      }
    else {biendo = biendo - 1;     // chinh xuong
        if(biendo > 200) biendo = 200;
        lcd.setCursor(5,1);
        lcd.print(biendo); 
        }
    }
A=a;
}

void chopbiendo()//in khoang cach ra LCD luc dang xoay volume
{

    lcd.setCursor(0,0);lcd.print("khoang cach bao: ");
    lcd.setCursor(5,1);lcd.print(biendo); lcd.setCursor(13,1);  lcd.print("cm"); lcd.print("    "); 
  
}

void ngat()
{
  mode=1;  pip_1();
  for(int j=0;j<100;j++);
}
void pip_1()//loa keu
{
  digitalWrite(loa,HIGH);
  delay(50);
  digitalWrite(loa,LOW);
  delay(50);
}

void dokhoangcach()
{long distance = GetDistance();
 
  if (distance <= 0)//chua co gia tri
  {
        lcd.setCursor(0,1);lcd.print("kh/cach : ");
        lcd.print("  "); lcd.print("cm"); lcd.print("    ");
  }
  else //da co gia tri
  {   
        lcd.setCursor(0,1);lcd.print("kh/cach : ");
        lcd.print(distance); lcd.print("cm"); lcd.print("    ");
        //neu khoang cach dc do <= khoang cach da cai dat thi loa keu
        if(distance<= biendo)
        {pip_1();}
         
}
  delay(100);
       

     
}
