#define SH 4
#define INTEGRATION_TIME 10000000

volatile int i = 0;
volatile uint16_t adc_results[2099];

ISR(ADC_vect){ //acd interruption, save reading to memory and reset flag.
  if(i < 2094){
    adc_results[i++] = ADC;
  }
   TIFR1 = 0b00000100;
}

void setup_timers(){
  GTCCR |= 0b10000001;//stops all timers for synchronization
  //timer 1, reset pulse
  TCCR1A = 0b00111111;
  TCCR1B = 0b00011001;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
  TCNT1 = 15;
  OCR1A = 799;//timer TOP
  OCR1B = 45;//ADC trigger
  OCR1C = 788;//OCR1C = 792;//rst pulse

  PORTB |= 0b10000000;
  DDRB |= 0b10000000;

  //timer 3, clock
  TCCR3A = 0b00110011;
  TCCR3B = 0b00011001;
  TCNT3 = 0;
  OCR3A = 1599;
  OCR3B = 799;
  

  PORTE |= 0b00010000; //PH4
  DDRE |= 0b00010000;

  //timer 4, reset boost pulse
  TCCR4A = 0b00100011;
  TCCR4B = 0b00011001;
  TCNT4 = 23;
  OCR4A = 799;
  OCR4B = 788;

  PORTH |= 0b00010000;
  DDRH |= 0b00010000;
  
  GTCCR &= ~0b10000000;//starts timers
}

void read_sensor(){
  i = 0;//go to start of the results vector
  
  digitalWrite(SH, HIGH);
  __builtin_avr_delay_cycles(32);//high period of sh-pulse
  digitalWrite(SH, LOW);//integration time ends

  //start adc
  ADCSRA = 0b10101000; //enable adc, auto-trigger, interrupt on complete, prescaler divider = 2
  ADMUX  = 0b01000000; //adc0 input, right adjusted; AVCC for reference
  ADCSRB = 0b00000101; //timer 1 compare match b trigger 

  //GTCCR &= ~0b10000000;//starts timers
  
  TIFR1 = 0b00000100;//clears adc interrupt flag to allow interuption rising edge
  setup_timers();
  
  while(i < 2094); //wait to read all outputs
  

  while(TCNT3 > 800); //be sure clock 2 is low
  
  GTCCR |= 0b10000001;//stops all timers for synchronization
}

void setup() {
  SREG |= 0b10000000;
  pinMode(SH, OUTPUT);

  Serial.begin(115200);
}

void loop() {


  // DUMMY READ ==========================================================================================
  //The sensor is integrating while data is beign sent over serial, the outputs must be flushed ------
  read_sensor();
  //END OF DUMMY READ ====================================================================================

  __builtin_avr_delay_cycles(INTEGRATION_TIME/62.5); //waits for the propper integration time

  read_sensor();

  Serial.print("data\n");//marks the start of data
  Serial.write((uint8_t*)adc_results, 2*2094);//send content of adc_results byte by byte
  
}  



  
