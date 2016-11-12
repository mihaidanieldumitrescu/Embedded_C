#define SPRAY_TM 22

sbit DS at RC4_bit;
sbit SH_CP at RC3_bit;
sbit ST_CP at RC5_bit;

int minute=21;
int secunde=55;
int triggerOnce=0;
int isOn=1;
int timeToNextSpray=0;
int tmrStep=0;
char toBCD=0;


void initializare(){
    IRCF2_bit=1;
    IRCF2_bit=1;
    IRCF1_bit=1;
    IRCF0_bit=1;
    TRISA =0x00;
    TRISC =0x00;
    GIE_bit =1;
    PEIE_bit =1;
    TMR1IE_bit=1;
    TMR1ON_bit =1;
    C1ON_bit =0;
    C2ON_bit =0;
    PORTC = 0;
    PORTA = 0;

}


void trimiteBCD(int cifra){
   int i;
   C_bit = 0; //Carry bit
   SH_CP = 0;
   ST_CP = 0;
   toBCD = (cifra/10)<<4;
   toBCD = toBCD ^ ( cifra % 10);
   Delay_ms(300);
   for(i=7;i>=0;i--){
     toBCD =  toBCD << 1;
     DS = C_bit;   // incarca 1 bit
     SH_CP = 1;
     Delay_ms(10); //impuls ceas
     SH_CP = 0;
     }
     ST_CP =1;
     delay_ms(10); // validare iesire
     ST_CP =0;

}

 void outToBCD(int x){
   int deci=0;
   if (x<10) {
       RA1_bit= 0;
       RA2_bit= 0;
       RC0_bit= 0;
       RC1_bit= 0;
       RC2_bit= x>>3;
       RC3_bit= x>>2;
       RC4_bit= x>>1;
       RC5_bit= x;
     } else {
       deci=x / 10;
       RA1_bit= deci>>3;
       RA2_bit= deci>>2;
       RC0_bit= deci>>1;
       RC1_bit= deci;
       x=x%10;
       RC2_bit= x>>3;
       RC3_bit= x>>2;
       RC4_bit= x>>1;
       RC5_bit= x;
 }
 
 }
 
void refresh_BCD(){
   if(isON)
      trimiteBCD(timeToNextSpray);
   else
      trimiteBCD(secunde);
}

void spray(){

    RA0_bit =1;
    delay_ms(100);
    RA0_bit =0;
    triggerOnce =0;
    }

void timers(){

      if ( secunde == 60 )   //after 60 sec
     {
         minute++;
         secunde = 0;
         triggerOnce=1;
     }
     if ( minute == 1440 )   //after 24H reset
         minute=0;
     if ( minute <= 242 ) {  //isOn 4H
          isOn =1;
          timeToNextSpray = (  ( minute /SPRAY_TM)+ 1 )* SPRAY_TM - minute;
          }
     else { 
          isOn=0;
          timeToNextSpray =99;
     }
}

void debugBCD(int t){
    while(t){
      trimiteBCD(17);
  }
}

void main() {

     initializare();
     debugBCD(0);
     while(1){
       timers();
       refresh_BCD();
       if (isOn){
         if ((minute % SPRAY_TM == 0)){
                     if(triggerOnce)
                      spray();
          }
       }
     }
}

interrupt (){
   if(TMR1IF_bit){
     if (tmrStep == 61)
        { 
          tmrStep =0;
          secunde++;
        }
      tmrStep++;
      TMR1IF_bit =0;
   }
}
