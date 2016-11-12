sbit SemnalA at RC0_bit;
sbit SemnalB at RC1_bit;
sbit SemnalC at RC2_bit;
sbit status_led at RA0_bit;
long int tmr_intarziere=400;  // 40000-- 3,75 ms
long int tmr_cicluri =0;
int count=0;
int clank=0;
int multiplicator=0;
int treapta=16;
int stare_semnal=0;

void init(){

IRCF3_bit =1;
IRCF2_bit =1;
IRCF1_bit =1;
IRCF0_bit =1;

GIE_bit =1;
PEIE_bit=1;
  

TRISC =0b111100;
TRISA =0;
TRISA = 1<<4;
PORTA=0;
PORTC=0;
RA0_bit =0;


}

void initTmr(){

  TMR1IE_bit =1;
  TMR1H=0 ;
  TMR1L=0;
  TMR1ON_bit=0;
}
void intarziere_us (long int timp){
long long int count=0;
for (count=timp;count>0;count--){
Delay_1us();
}
}

void secventa_complexa (){
  PORTC =1 ;
  delay_ms(70);
  PORTC =3 ;
  delay_ms(20);
  PORTC =2 ;
  delay_ms(70);
  PORTC =6 ;
  delay_ms(20);
}

void secventa_simpla(){
  PORTC =1 ;
  delay_ms(90);

  PORTC =2 ;
  delay_ms(90);

  PORTC =4;
  delay_ms(90);
}

void genereaza_semnal_pas1(){
     PORTC = 1;
}
void genereaza_semnal_pas2(){
     PORTC = 2;
}

  void debugMode(int flag){
  while(0){
  
  genereaza_semnal_pas1();
  delay_us(10);
  genereaza_semnal_pas2();
  delay_us(10);
  }
  }
  
  
void main() {
  int i;
  init();
  initTmr();
  debugMode(0);
  
   while(1){
   if (RC2_bit){ //UP
       if(tmr_intarziere < 400)
                        tmr_intarziere+=10;
   }
   if (RC3_bit){ //DOWN
      if(tmr_intarziere > 10)
                        tmr_intarziere-=10;

   }
   if (RC4_bit){} // MOD SEMNAL
   if (RC5_bit){} // LATIME INTERSECTIE
   
 stare_semnal = (stare_semnal +1) % 2;
   switch (stare_semnal){
   case 0:  genereaza_semnal_pas1();
            break;
   case 1:
            genereaza_semnal_pas2();
            break;
   default: break;
   }
   
   for(i=tmr_intarziere;i>0;i--)
   delay_ms(1);
   
 }
 


}

interrupt(){
if (TMR1IF_bit)
{
  RA0_bit  = ~RA0_bit;
 if( tmr_cicluri >= tmr_intarziere )
     stare_semnal = (stare_semnal +1) % 2;
 TMR1H=0;
 TMR1L=0;
 tmr_cicluri++;
TMR1IF_bit=0;
}

}
