void refreshLCD();

// LCD module connections

sbit LCD_RS at RC0_bit;
sbit LCD_EN at RC1_bit;
sbit LCD_D4 at RC5_bit;
sbit LCD_D5 at RC4_bit;
sbit LCD_D6 at RC3_bit;
sbit LCD_D7 at RC2_bit;
sbit LCD_RS_Direction at TRISC0_bit;
sbit LCD_EN_Direction at TRISC1_bit;
sbit LCD_D4_Direction at TRISC5_bit;
sbit LCD_D5_Direction at TRISC4_bit;
sbit LCD_D6_Direction at TRISC3_bit;
sbit LCD_D7_Direction at TRISC2_bit;// End LCD module connections

int _debugMode=0;
int minute=0;
int secunde=0;
int ore=0;
int capacitate_int=0;
int counter=0;
int adc = 0;
int intregi=0;
int zecimi=0;
int refresh_en=0;
int timer_on=0;
int toggleRefresh=0;
struct DisplayOut displayChr;
char* iesire_lcd;
char lcd_line1[] = "u:0000  00:00:00";
char lcd_line2[]=  "c:9999      tOFF";
const int curent_incarcare=500;
const int tensiune_min=200; // 1.0v

struct mainVar{
 int adc;
 int minute;
 int secunde;
 int ore;
 int capacitate_int;
};

struct DisplayOut{
  char tensiune[4];
  char durataT[8];
  char ore[2];
  char minute[2];
  char secunde[2];
  char capacitate[4];
  char stare[6];
};

char UART_Init(const long int baudrate)
{
  unsigned int x;
  x = (16000000 - baudrate*64)/(baudrate*64); //SPBRG for Low Baud Rate
  if(x>255) //If High Baud Rate required
  {
    x = (16000000 - baudrate*16)/(baudrate*16); //SPBRG for High Baud Rate
    BRGH_bit = 1; //Setting High Baud Rate
  }
  if(x<256)
  {
    SPBRG = x; //Writing SPBRG register
    SYNC_bit = 0; //Selecting Asynchronous Mode
    SPEN_bit = 1; //Enables Serial Port
    CREN_bit = 1; //Enables Continuous Reception
    TXEN_bit = 1; //Enables Transmission
    return 1;
  }
  return 0;
}

void UART_Write22(char dataa)
{
  while(!TRMT_bit); //Waiting for Previous Data to Transmit completly
  TXREG = dataa; //Writing data to Transmit Register, Starts transmission
}
void UART_Write_Text22(char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++)
    UART_Write22(text[i]);
}

void initializare(){
   GIE_bit  =0;
   PEIE_bit =1;
   TMR1IE_bit=1;
   
   T1CON =0x00;
   
   ADIE_bit =1;

   IRCF3_bit  =1;
   IRCF2_bit  =1;
   IRCF1_bit  =1;
   IRCF0_bit  =1;

   ANSELA  = 0b100;   //RA2
   TRISA2_bit =1;
   TRISA0_bit =0;
   ANSELC  = 0;
   C1ON_bit = 0;
   C2ON_bit = 0;
   TRISC = 0x00;
   TRISA1_bit=0;
   
   ADCS2_bit=1;
   ADCS1_bit=1;
   ADCS0_bit=1;
   
   UART_Init(19200);
   TXCKSEL_bit = 0;
 }
 
 void addToString( int line,int a, int b, char* tmp ) {
int i=0;
for (i=a;i<b;i++)
{
if( line ==1 )
 lcd_line1[i] = tmp[i];
else if ( line ==2 )
  lcd_line2[i] = tmp[i];
}
}

void Int2ChrSec(int sec){
   char* conversie= "00";

   IntToStr(sec,conversie);   //SECUNDE
   lcd_line1[14]  = conversie[4];
   lcd_line1[15]  = conversie[5];
   
   displayChr.secunde[0] = conversie[4];
   displayChr.secunde[1] = conversie[5];
}
void Int2ChrMin(int min){
   char* conversie= "00";
   if (min <=60)   {
   IntToStrWithZeros(min,conversie);   //MINUTE
   lcd_line1[11] = conversie[4] ;
   lcd_line1[12] = conversie[5];    
   
   displayChr.minute[0] = conversie[4];
   displayChr.minute[1] = conversie[5];
      }
   else   {
   
      lcd_line1[11] = '0' ;
      lcd_line1[12] = '0' ;      }
   
      displayChr.minute[0] = '0';
      displayChr.minute[1] = '0';



}
void Int2ChrOre(int oret){
   char* conversie= "00";
    IntToStrWithZeros(oret,conversie);      //ORE
    lcd_line1[8]   = conversie[4] ;
    lcd_line1[9]   = conversie[5];

    displayChr.ore[0] = conversie[4];
    displayChr.ore[1] = conversie[5];


}

double prelucreazaTensiune(){
      double tensiune_adc;
      tensiune_adc = 5.12 /1024 *adc*1000; // rezultatul va fi double sau int?
      return tensiune_adc;
}

void afiseazaTensiune(){
      char* conversie= "0000";
      double tensiune_adc;
      int conv_int=0;
      tensiune_adc = 5.12 /1024 *adc*1000; // rezultatul va fi double sau int?
      conv_int =(int)tensiune_adc;
      IntToStrWithZeros(conv_int, conversie);
      
        lcd_line1[2]   = conversie[2];
        lcd_line1[3]   = conversie[3];
        lcd_line1[4]   = conversie[4];
        lcd_line1[5]   = conversie[5];
        
        displayChr.tensiune[0] = conversie[2];
        displayChr.tensiune[1] = conversie[3];
        displayChr.tensiune[2] = conversie[4];
        displayChr.tensiune[3] = conversie[5];
      
}
 void setTimerON_OFF(){
   if(adc>200)  {
           TMR1ON_bit=1;
           timer_on=1;
}
   else {
           TMR1ON_bit=0;
           timer_on=0;
           RA1_bit=0;
 }
}

void afiseazaStare(){
   if(timer_on)
   {
   lcd_line2[12] = ' ';
   lcd_line2[13] = 't';
   lcd_line2[14] = 'O';
   lcd_line2[15] = 'N';
   
   displayChr.stare[0] = ' ';
   displayChr.stare[1] = 't';
   displayChr.stare[2] = 'O';
   displayChr.stare[3] = 'N';
   }
   
   else{
   lcd_line2[12] = 't';
   lcd_line2[13] = 'O';
   lcd_line2[14] = 'F';
   lcd_line2[15] = 'F';
   
   displayChr.stare[0] = 't';
   displayChr.stare[1] = 'O';
   displayChr.stare[2] = 'F';
   displayChr.stare[3] = 'F';
   }

}
void refreshAll1Sec(int tog){
if(tog){
  //SECUNDE
   if(secunde == 60 ){
   minute++;
   if ((minute % 3 == 0 || minute ==0))   // La fiecare 3 minute
      capacitate_int  = capacitate_int + 25;
   secunde=0;
   }
   //MINUTE
   if(minute == 60 ){
   ore++;
   minute=0;
   }
  toggleRefresh=0;
}
}

void afiseazaCeas(){
   //SECUNDE
   if(secunde == 60 ){
   minute++;
   if ((minute % 3 == 0 || minute ==0))   // La fiecare 3 minute
      capacitate_int  = capacitate_int + 25;
   secunde=0;
   }
   Int2ChrSec(secunde);
   
   //MINUTE
   if(minute == 60 ){
   ore++;
   minute=0;

    lcd_line1[11]   = '0' ;
    lcd_line1[12]   = '0';
   }

 //   Int2ChrMin(minute);
    
 //   Int2ChrOre(ore);

}

void afiseazaCapacitate(){
   char* conversie= "0000";
   char  tmp[16];
   IntToStrWithZeros(capacitate_int,conversie);

  lcd_line2[2] = conversie[2] ;
  lcd_line2[3] = conversie[3]  ;
  lcd_line2[4] = conversie[4] ;
  lcd_line2[5] = conversie[5] ;
  
  displayChr.capacitate[0] = conversie[2];
  displayChr.capacitate[1] = conversie[3];
  displayChr.capacitate[2] = conversie[4];
  displayChr.capacitate[3] = conversie[5];

}


void debugMode(int isSet){
while(isSet){

delay_ms(1000);

}
}

void refreshLcd(){
 
 
//afiseazaTensiune();
//afiseazaCeas();
//afiseazaCapacitate();
 afiseazaStare();

Lcd_Out(1,1,lcd_line1);
Lcd_Out(2,1,lcd_line2);
}

//MAIN
void main(){
   int adc =0;

   initializare();

   ADC_Init();
   RA2_bit =0;
   Lcd_Init();
   Lcd_Cmd(_LCD_CURSOR_OFF);

   TMR1H=0xF6;
   TMR1L=0xC3;
   RA1_bit=1;
   TMR1ON_bit =1;

   UART_Init(19200);
  
   debugMode(0);
   while (1){
   
    adc= adc_read(2);
   //REFRESH LCD

   if(adc>200)  {
           TMR1ON_bit=1;
           timer_on=1;
}
   else {
           TMR1ON_bit=0;
           timer_on=0;
 }
 
 refreshAll1Sec(toggleRefresh);
   refreshLcd();
   displayChr.secunde[1] ='4';
   displayChr.secunde[0]='5';
   UART_Write_Text22("Secunde: ");
   UART_Write_Text22("9");
   UART_Write_Text22("10");
   UART_Write_Text(0x0D);
   UART_Write_Text(0x0A);
   delay_ms(1000);

}}

interrupt(){
  if (TMR1IF_bit)
     {
     if (counter > 61) {

                if(adc >= tensiune_min)
                  secunde++;
                refresh_en=1;
                counter=0;
                TMR1H=0xF6;
                TMR1L=0xC3;
                RA1_bit = ~RA1_bit;
                toggleRefresh=1;
                TMR1IF_bit =0; }
    else 
    {
    counter++;
    TMR1IF_bit =0;
    }

   }
else if (ADIF_bit)
{
if(!ADGO) {
          adc = ADRESH+ADRESL;
          ADGO_bit=1;
          }
          ADIF_bit =0;
}

}

 /*notite program:
   -- ceva strica afisarea pe tot ecranul.
   -- v1.1 -- implementat : "Pornire timer in functie de valoarea adc [ start daca ADC > 3.50v ]"
   -- de folosit constate pentru lcd si de limitat spatiile pentru valori
   -- de rezolvat TIMER1 in loc de delay si ADC in intreruperi
   -- atentie calibrare secunda ( delay_ms )
   -- conversia INT -- str sau mai rau Float -- str nu funtioneaza, si afiseaza porcarii pe lcd
   -- REZOLVAT adc strica afisarea pe ecran
   -- REZOLVAT -- afisarea capacitatii nu mai functioneaza (IntTOstr(adc, conversie))
   -- totul merge ok pana cand este nevoie sa se afiseze valoarea adc
   --------------------------------------------------------------------------------------------------------
   -- v1.0 -- dupa capacitatea de incarcare, masoara capacitatea totala, cu pasul de 25 mH la 3 fiecare 3 minute
           -- de implementat afisarea valorii tensiunii pe adc si pornirea ceasului intre tensiunile [3.5, 4.2] volti
 */
