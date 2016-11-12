#uses MikroC PWM libraries

void setPeriod();

int timp_ciclu; // ex 25 ms

 int a=0xFE,b=0xFE;
 int adc_value=0;
 int adc_zero_correction=0;
 int period=0;
 int counter=0;
 int time_10us=0;
 int count=0;
 int AsauB=1;

void initializare()
{
    IRCF3_bit =1;
    IRCF2_bit =1;
    IRCF1_bit =1;
    IRCF0_bit =1;

    GIE_bit =1;
    PEIE_bit=1;
    TMR1IE_bit =1;

    TMR1H =0xFF;
    TMR1L =0xD7;

    TRISC = 0x00;
    TRISA = 0x11001111;
    RA4_bit =1;
    RA5_bit =1;

    ADC_Init();
    PORTC = 0x00;
    
PWM2_Init(5000);  // Initialize PWM2
PWM2_Start();  // start PWM2
PWM2_Set_Duty(0); // Set current duty for PWM2

}

void debugMode(int x){
   while(x){

      RC5_bit =1;
      Delay_50us();
      RC5_bit =0;
      Delay_50us();
   }
 }
 
 void calcIntarziere(){
 
 }

void main() {

    initializare();
    debugMode(1);

    adc_value = ADC_Read(2);
    adc_zero_correction = adc_value-90;
    
    while(1){


        RC4_bit=1;     // Test durata sa ruleze urmatoarele instructiuni
        adc_value = ADC_Read(2);
        adc_zero_correction = adc_value-90;
        setPeriod();

    RC4_bit=0; //Sfarsitul testului
    TMR1ON_bit=1;
    delay_ms(100);

 }
 

}

void setPeriod(){

  if       (adc_zero_correction >= 0 && adc_zero_correction < 93)
      period = 10;
  else if  (adc_zero_correction >= 93 && adc_zero_correction < 186)
      period = 20;
  else if  (adc_zero_correction >= 186 && adc_zero_correction < 279)
      period = 30;
  else if  (adc_zero_correction >= 279 && adc_zero_correction < 372)
      period = 40;
  else if  (adc_zero_correction >= 372 && adc_zero_correction < 465)
       period = 50;
  else if  (adc_zero_correction >= 465 && adc_zero_correction < 558)
       period = 60;
  else if  (adc_zero_correction >= 558 && adc_zero_correction < 651)
       period = 70;
  else if  (adc_zero_correction >= 651 && adc_zero_correction < 744)
       period = 80;
  else if  (adc_zero_correction >= 744 && adc_zero_correction < 837)
       period = 90;
    else if  (adc_zero_correction >= 837 && adc_zero_correction <= 934)
       period = 100;

  PWM2_Set_Duty(period); // Set current duty for PWM2             

}
