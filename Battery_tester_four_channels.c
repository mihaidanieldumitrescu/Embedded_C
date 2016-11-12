const int prag_nimh = 200;
int counter;

int refreshtoggle =0;
int led_status=1;
int  stand_by;
float tensiune=0.0;
typedef struct {
    int ore;
    int minute;
    int secunde;
    int adc_value;
    int capacity;
    int locked_state;
    char status_port[];
    char status_lock[];
} Status;

Status slot1,slot2,slot3,slot4;
//----------------------------------------------------------
//Functii reglare domeniu timp
//----------------------------------------------------------

 void increment_time_capacity(int discharge_threshold){
 int tog;
 
  if(!slot1.locked_state && slot1.adc_value >= discharge_threshold ) {
      slot1.secunde++;
      if (slot1.secunde ==60){
          slot1.secunde=0;
          slot1.minute++; 
      }

      if (slot1.minute == 60){
          slot1.ore++;
         // if ((slot1.secunde %3) ==0 || slot1.secunde == 0 )
          slot1.capacity  = slot1.capacity + 25;

      }
  if(!slot2.locked_state && slot2.adc_value >= discharge_threshold ){
      slot2.secunde++;
      if (slot2.secunde ==60) {
          slot2.secunde=0;
          slot2.minute++;
       /*   if ((slot2.minute % 3 == 0 || slot2.minute ==0)&0)
          slot2.capacity  = slot2.capacity + 25;*/
      }
      if (slot2.minute == 60)
          slot2.ore++;  
   }
  if(!slot3.locked_state && slot3.adc_value >= discharge_threshold ) {
      slot3.secunde++;
      if (slot3.secunde ==60)  {
          slot3.secunde=0;
          slot3.minute++;
       /* if ((slot3.minute % 3 == 0 || slot3.minute ==0)&0)
          slot3.capacity  = slot3.capacity + 25;   */
      }
      
      if (slot3.minute == 60)
                slot3.ore++;  
  }
  if(!slot4.locked_state && slot4.adc_value >= discharge_threshold ) {
      slot4.secunde++;
      if (slot4.secunde ==60) {
          slot4.secunde=0;
          slot4.minute++;
      /*  if ((slot4.minute % 3 == 0 || slot4.minute ==0)&0)
          slot4.capacity  = slot4.capacity + 25;  */
   }
       if (slot4.minute == 60)
          slot4.ore++;
   }
 } 
}

 void eroare(){

 //unul dintre comenzile astea face sa nu mai functioneze adc-ul
   C2ON_bit =0;
   C1ON_bit =0;
   ADON_bit =1;
   ADIE_bit =1;
}

void initializare(){
   GIE_bit  = 1;
   PEIE_bit = 1;
   TMR1IE_bit =1;

   IRCF3_bit = 1 ;
   IRCF2_bit = 1 ;
   IRCF1_bit = 1 ;
   IRCF0_bit = 1 ;

 //RXDTSEL_bit = 1;   //RA1
   TXCKSEL_bit = 0;   //RB7

   TMR1H =0xF6;
   TMR1L =0xC3;

   TMR1ON_bit =1;
}

void init_porturi(){
   ANSELA =  0b00000111;
   ANSELC =  0x01; //RC0

   PORTA = 0x00;
   PORTC = 0x00;
   TRISC5_bit = 0;  //IESIRE LED

   TRISC4_bit = 0;  // IESIRE 4
   TRISC3_bit = 0;  // IESIRE 3
   TRISC2_bit = 0;  // IESIRE 2
   TRISC1_bit = 0;  // IESIRE 1

   TRISC0_bit = 1;  // ADC 4
   TRISA2_bit = 1;  // ADC 2
   TRISA1_bit = 1;  // ADC 1
   TRISA0_bit = 1;  // ADC 0

   TRISA5_bit = 0;
   TRISA4_bit = 0;
   TRISA3_bit = 1;  //MCLR
   TRISB7_bit = 0;
}

void reset_time_cap(int slot){
    switch(slot){
        case 0:     break;
        case 1:     slot1.ore=slot1.minute=slot1.secunde=slot1.capacity=0; 
                    break;
        case 2:     slot2.ore=slot2.minute=slot2.secunde=slot2.capacity=0; 
                    break;
        case 3:     slot3.ore=slot3.minute=slot3.secunde=slot3.capacity=0; 
                    break;
        case 4:     slot4.ore=slot4.minute=slot4.secunde=slot4.capacity=0;  
                    break;
        default: break;
    }
}
//-----------------------------------------------
//Functii control porturi on/off
//-----------------------------------------------
void open_port(int sel){
    RC5_bit = 1;
        switch(sel){
            case 4: rc4_bit = 1;    //PORT4
                    strcpy(slot4.status_port,"Discharging");
                    break;
            case 2: rc3_bit = 1;
                   strcpy(slot3.status_port,"Discharging");
                    break;
            case 1: rc2_bit = 1;
                   strcpy(slot2.status_port,"Discharging");
                    break;
            case 0: rc1_bit = 1;     //PORT1
                    strcpy(slot1.status_port,"Discharging");
                    break;
            default: break;
        }
}

void close_port(int sel){
      switch(sel){
          case 4: Rc4_bit = 0;     //PORT4
                  strcpy(slot4.status_port,"Closed");
            //      RC5_bit =0;
                  break;
          case 2: Rc3_bit = 0;
                  strcpy(slot3.status_port,"Closed");
             //     RC5_bit =0;
                  break;
          case 1: Rc2_bit = 0;
                  strcpy(slot2.status_port,"Closed");
            //      RA5_bit =0;
                  break;
          case 0: Rc1_bit = 0;   //PORT1
                  strcpy(slot1.status_port,"Closed");
            //      RA4_bit=0;
                  break;
          default: break;
     }
}

void standby(){


}

void depanare() {
    IRCF3_bit =1;
    IRCF2_bit =1;
    IRCF1_bit =1;
    IRCF0_bit =1;
    TRISA = 0xFF;
    TRISC = 0x00;
    ANSELA = 0x01;
    PORTC= 0x00;

    adc_init();
}

void read_4adc_val() {
  slot1.adc_value = adc_read(0);
  slot2.adc_value = adc_read(1);
  slot3.adc_value = adc_read(2);
  slot4.adc_value = adc_read(4);
}

//----------------------------------------------------------------
//Functii control parametrii inchidere/deschidere porturi
//----------------------------------------------------------------

void open_close_ports(int treapa_inferioara){

  if(!slot1.locked_state && slot1.adc_value >= treapa_inferioara )
            open_port(0); else close_port(0);
  if(!slot2.locked_state && slot2.adc_value >= treapa_inferioara )
            open_port(1); else close_port(1);
  if(!slot3.locked_state && slot3.adc_value >= treapa_inferioara )
            open_port(2); else close_port(2);
  if(!slot4.locked_state && slot4.adc_value >= treapa_inferioara )
            open_port(4); else close_port(4);
 }

 //-------------------------------------------------------------------------------------------
 //Functie intrerupere a reimprospatarii valorilor ADC odata ce acumulatorul a fost descarcat  
 //-------------------------------------------------------------------------------------------
 
 void lock_unlock_at(int minval,int maxval){

   //SLOT 1
   if (slot1.adc_value <maxval && slot1.adc_value >minval) {
     slot1.locked_state=1;
     strcpy(slot1.status_lock, "Discharge complete");
   } else if (slot1.adc_value< minval) {
       slot1.locked_state=0;
       strcpy(slot1.status_lock, "Battery not detected in slot 1");
       reset_time_cap(1);
   }
  //SLOT 2
  if (slot2.adc_value<maxval && slot2.adc_value >minval)  {
     slot2.locked_state=1;
     strcpy(slot2.status_lock, "Discharge complete");
   } else if (slot2.adc_value < minval){
     slot2.locked_state=0;
     strcpy(slot2.status_lock, "Battery not detected in slot 2");
     reset_time_cap(2);
   }
  //SLOT 3
  if (slot3.adc_value <maxval && slot3.adc_value >minval) {
     slot3.locked_state=1;
     strcpy(slot3.status_lock, "Discharge complete");
      }
   else if (slot3.adc_value< minval){
      slot3.locked_state=0;
      strcpy( slot3.status_lock, "Battery not detected in slot 3");
      reset_time_cap(3);
   }
  //SLOT 4
  if (slot4.adc_value<maxval && slot4.adc_value  >minval) {
     slot4.locked_state=1;
     strcpy( slot4.status_lock, "Discharge complete");
   } else if (slot4.adc_value  < minval){
      slot4.locked_state=0;
      strcpy( slot4.status_lock, "Battery not detected in slot 4");
      reset_time_cap(4);
   }

}

//-----------------------------------------------------------------
//Functii transmisie/prelucrare mesaje stare prin UART
//-----------------------------------------------------------------

void newline(int x){
  int i;
  for (i=0;i<x;i++) {
      UART1_Write(0x0D);
      UART1_Write(0x0A);
  }
}
void scrie_log(int slot){
char slot_nr[8], adcval[8], status[100], time[8], capacity[8];

    //SLOT nr

    switch(slot){
      case 0:     strcpy(slot_nr ,"1"); break;
      case 1:     strcpy(slot_nr ,"1"); break;
      case 2:     strcpy(slot_nr ,"2");  break;
      case 3:     strcpy(slot_nr ,"3");  break;
      case 4:     strcpy(slot_nr ,"4");  break;
      default: break;
    }

    switch(slot){
      case 0:     IntToStr(slot1.adc_value*5,adcval); break;
      case 1:     IntToStr(slot1.adc_value*5,adcval); break;
      case 2:     IntToStr(slot2.adc_value*5,adcval); break;
      case 3:     IntToStr(slot3.adc_value*5,adcval); break;
      case 4:     IntToStr(slot4.adc_value*5,adcval); break;
      default: break;
    }
    //STATUS
    
    switch(slot){
      case 0:      strcpy(status ,slot1.status_lock); break;
      case 1:      strcpy(status ,slot1.status_lock);   break;
      case 2:      strcpy(status ,slot2.status_lock); break;
      case 3:      strcpy(status ,slot3.status_lock);  break;
      case 4:      strcpy(status ,slot4.status_lock);  break;
      default: break;
    }

    switch(slot){
      case 0:     IntToStr(slot1.secunde,time);  break;
      case 1:     IntToStr(slot1.secunde,time);  break;
      case 2:     IntToStr(slot2.secunde,time);  break;
      case 3:     IntToStr(slot3.secunde,time);  break;
      case 4:     IntToStr(slot4.secunde,time);  break;
      default: break;
    }

    switch(slot){
      case 0:     IntToStr(slot1.capacity,capacity); break;
      case 1:     IntToStr(slot1.capacity,capacity);  break;
      case 2:     IntToStr(slot2.capacity,capacity);  break;
      case 3:     IntToStr(slot3.capacity,capacity);  break;
      case 4:     IntToStr(slot4.capacity,capacity);  break;
      default: break;
    }
    if(slot != 1)
    newline(1);
    
    UART1_Write_Text( "Slot");
    UART1_Write_Text(slot_nr);
    UART1_Write_Text (": ");
    newline(1);
    UART1_Write_Text ("-----------");
    newline(1);
    UART1_Write_Text( "Voltage: "  );
    UART_Write_Text(adcval);
    newline(1);
    UART1_Write_Text( "Status: "  );
    UART_Write_Text(status);
    newline(1);
    UART1_Write_Text( "Time:  "  );
    UART_Write_Text(time);
    newline(1);
    UART1_Write_Text( "Capacity: "  );
    UART_Write_Text(capacity);
    newline(1);
}

//------------------------------------------------------
//Definitiile valorilor initiale pentru debug
//------------------------------------------------------
void init_debug_val(int toggle){
  if(toggle){
      slot1.ore=0;
      slot1.minute=0;
      slot1.secunde=0;
      slot1.adc_value=0;
      slot1.capacity=0;
      slot1.locked_state=0;
      strcpy(slot1.status_port,"Opened");
      strcpy(slot1.status_lock,"Discharging");

      slot2.ore=10;
      slot2.minute=22;
      slot2.secunde=38;
      slot2.adc_value=512;
      slot2.capacity=0;
      slot2.locked_state=0;
      strcpy(slot2.status_port,"Opened");
      strcpy(slot2.status_lock,"Discharging");

      slot3.ore=10;
      slot3.minute=22;
      slot3.secunde=38;
      slot3.adc_value=512;
      slot3.capacity=0;
      slot3.locked_state=0;
      strcpy(slot3.status_port,"Opened");
      strcpy(slot3.status_lock,"Discharging");

      slot4.ore=10;
      slot4.minute=22;
      slot4.secunde=38;
      slot4.adc_value=512;
      slot4.capacity=0;
      slot4.locked_state=0;
      strcpy(slot4.status_port,"Opened");
      strcpy(slot4.status_lock,"Discharging");
  }
}

void main() {

  initializare();
  init_porturi();
  init_debug_val(1);

  UART1_Init(19200);

  while(1)  {
      read_4adc_val();
      delay_ms(100);
      open_close_ports(200);
      lock_unlock_at(80,200);

      if (refreshtoggle ==1) { // refresh > 1 sec
      increment_time_capacity(200);
         refreshtoggle=0;

           scrie_log(1);
           scrie_log(2);
           scrie_log(3);
           scrie_log(4);
           newline(5);

      }
  }
}

 void interrupt() {
  if(TMR1IF_bit) {
    counter++;
    TMR1H =0;
    TMR1L =0;
    
    if(counter ==61) {
       TMR1H =0xF6;
       TMR1L =0xC3;
    }
    
    if(counter==62) {
      //1 sec
      refreshtoggle=1;
      counter =0;
    }
    
    TMR1IF_bit=0;
    }
}
  
/*
----------------------------------------------------------------------------------------------------------------------

Slot1: 3.0 v
Status: Discharge complete

Time:  03:02:04
Capacity: 1950mAh
------------------
Slot2: 3.0v
Status: Discharge complete

Time:  00:00:00
Capacity: 0000mAh
------------------
Slot3: 0.0v
Status: Empty slot

Time:  00:00:00
Capacity: 0000mAh
------------------
Slot4: 3.6v
Status: Discharging

Time:  01:02:35
Capacity: 1242 mAh
------------------
*/
