#include <M5StickCPlus2.h>

#include "back.h"
#include "player.h"
#include "enemy.h"
#include "ani.h"
#include "gameover.h"


#define TFT_GREY 0x5AEB

int8_t xj,yj,bj;

//int player
// float pX=random(30,100);
float pX=70;
float pY=200;
int pW=25;
int pH=31;
float pXs=0.2;
float pYs=0.2;
int pos[4]={2,35,68,101};

//bulet
float bx[10];
float by[10];
int bw=2;   //player bullet size
float bs=0.35; //player bulett speed

//enemies
float ex[8];
float ey[8];
int ef[8];
int ew=32;
int eh=32;
float esy=0.11;
float esx=0;
int eFrame[8];
int el[8];


float pres1=0;//debounce left button
float pres2=0;//debounce right button
int counter=0;
int fire=0;
int score=0;
int timeAlive=0;
int currentTime=0;
int newLevelTime=15;

void setup() {
  M5.begin();
  M5.Display.setRotation(0);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setSwapBytes(true); 
  for(int i=0;i<8;i++)
  {
    ex[i]=pos[random(0,4)];
    ey[i]=-50;
    eFrame[i]=random(0,4);
    el[i]=3;
  }

  for(int i=0;i<10;i++)
  by[i]=250;

}


int count=0;
int startCounter=0;
int endCounter=0;
int aniFrame=0;

int fase=0; //fase 0=start screen 1=play 2=gameover


void loop() {

  // IMU Joystick
  auto imu_update = M5.Imu.update();

  if (imu_update) {

      auto data = M5.Imu.getImuData();

      float xAccel = data.accel.x;
      float yAccel = data.accel.y;

      xj = xAccel * 30;
      yj = yAccel * 30;

  }

  
  M5.update();

  // Fire
  if (StickCP2.BtnA.wasPressed()) {
      StickCP2.Speaker.tone(8000, 20);
      bj = 1;
  }

  checkButton();
  bj = 0;
  
  if(fase==0){
     delay(500);
     M5.Display.fillScreen(TFT_BLACK);
     M5.Display.setTextColor(TFT_WHITE,TFT_BLACK);
     M5.Display.pushImage(0,0,135,240,back);
     while(digitalRead(37)==1){
        M5.Display.pushImage(34,0,102,84,ani[aniFrame]);
        delay(40);
        aniFrame++;
         if(aniFrame==25)
          aniFrame=0; 
        }
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setTextColor(TFT_WHITE,0x7820);
    M5.Display.fillRect(0,0,135,17,0x7820);
    M5.Display.drawString("SCORE:",2,1,2);
    M5.Display.drawString("TIME:",70,1,2);
    M5.Display.drawLine(0,17,135,17,0x6577);
    M5.Display.drawLine(0,18,0,240,0x6577);
    M5.Display.drawLine(134,18,134,240,0x6577);
     
     fase=1; 
  }

  if(fase==1){
 
    if(millis()-currentTime>1000)
    {
      timeAlive++;
      M5.Display.drawString(String(timeAlive),105,1,2);
    
      currentTime=millis();
      if(timeAlive==newLevelTime)
        {
          esy=esy+0.02;
          newLevelTime=timeAlive*2;
        }
    }

    if(startCounter==0)
    if(count<8){
      startCounter=1;
      ey[count]=19;
      count++;
      if(count==8)
      count=0;
      endCounter=random(180,500);
      
    }
    
    draw();
    
    for(int i=0;i<10;i++){
        if(by[i]>23 && by[i]<240)
        by[i]=by[i]-bs;
      
      }

    for(int i=0;i<8;i++){
        if(ey[i]!=-50.00){
        ey[i]=ey[i]+esy;
        ex[i]=ex[i]-esx;
        }

        if(ey[i]>242)
            {
            
            ex[i]=pos[random(0,4)];
            ey[i]=-50;
            eFrame[i]=random(0,4);
            esy=esy+0.0025;
            el[i]=3;
            }
      }   

    if(fire>0)
    fire++;
    if(fire>60)
    fire=0;

    if(startCounter>0)
    startCounter++;
    if(startCounter>=endCounter)
    startCounter=0;

    collision();
   
  }

 if(fase==2)
    {
      M5.Display.fillScreen(TFT_BLACK);
      M5.Display.pushImage(0,0,135,240,gameover);
       M5.Display.drawString("SCORE: "+String(score),25,120,2);
      M5.Display.drawString("TIME: "+String(timeAlive),25,140,2);
      delay(500);
      while(digitalRead(37)==1);
      restart();
      fase=0;
      
    }
}

void draw()
  {
 
  M5.Display.pushImage(pX,pY,pW,pH,player);

      for(int i=0;i<10;i++)
      { 
      
        M5.Display.fillCircle(bx[i],by[i],bw+2,TFT_BLACK);
        if(by[i]>23)
        M5.Display.fillCircle(bx[i],by[i],bw,TFT_RED);
      }

   for(int i=0;i<8;i++)
      { 
        if(ex[i]<-32)
        M5.Display.fillRect(0,14,32,32,TFT_BLACK);
        else
        M5.Display.pushImage(ex[i],ey[i],eh,ew,enemy[eFrame[i]]);
        
      }    
  }

void checkButton()
    {
    if(xj>10)
    if(pX>2)
    pX=pX-pXs;

if(xj<-10)
    if(pX<135-pW-1)
    pX=pX+pXs;

   if(bj==0){
    if(pres2==0)
       {
        if(fire==0){
        pres2=1;
        bx[counter]=pX+pW/2;
        by[counter]=pY-bw/2;
        counter++;
        fire=1;
        if(counter>=10)
        counter=0;}
       }}else pres2=0; 
       
    }

    void collision()
    {
      for(int i=0;i<10;i++){
        for(int j=0;j<8;j++){

            if(pX+pW/2>ex[j] && pX+pW/2<ex[j]+ew && ey[j]+26>pY && ey[j]<230)
                 fase=2;
        
           if(bx[i]>ex[j]+2 && bx[i]<(ex[j]+ew-2) && by[i]<ey[j]+20)
               {
                 score++;
                 M5.Display.drawString(String(score),48,1,2);
                 by[i]=250;
                 el[j]=el[j]-1;
                 if(el[j]==0){
                  score=score+5;
                 M5.Display.fillRect(ex[j],ey[j],32,32,TFT_BLACK);
                 ex[j]=pos[random(0,4)];
                 ey[j]=-50;
                 eFrame[j]=random(0,4);
                 el[j]=3;}
                }
        } }
      
      }
      void restart(){
        esy=0.11;
          for(int i=0;i<8;i++)
      {
      ex[i]=pos[random(0,4)];
      ey[i]=-50;
      eFrame[i]=random(0,4);
      el[i]=3;
      }

      for(int i=0;i<10;i++)
      by[i]=250;

      pres1=0;//debounce left button
      pres2=0;//debounce right button
      counter=0;
      fire=0;
      score=0;
      timeAlive=0;
      currentTime=0;
      newLevelTime=15;
      count=0;
      startCounter=0;
      endCounter=0;
      aniFrame=0;
      }
