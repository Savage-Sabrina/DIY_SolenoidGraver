#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


//Assingning Analog Pins

const int FrequencyPin = 1;               //Pin that sets the maximun frequency (ANALOG)
const int PercentagePin = 2;              //Pin from percentage of the cycle that turns the solenoid on (ANALOG)

//Assigning digital Pins

const int SelectModePin = 2;              // Pin to select mode of action (DIGITAL)
const int UpButtonPin = 3;                //Pin to change the frequency UP on direct mode (DIGITAL)
const int DownButtonPin = 4;              //Pin to change the frequency Down on direct mode (DIGITAL)
const int MosfetPin = 9;                  // the number of the LED pin (DIGITAL)
const int DigitalFootPedalPin = 10;       //Pin of the foot pedal potentiometer (DIGITAL)


//Assigning variables

float PeriodMosfet  = 0;                  //Time between the impacts its a ratio between 1/frequency
float FrequencyMosfet = 1;                //Frequency in hertz of the impacts menas impacts per second
float PercentageMosfet = 5;              //Percentage of the cycle that turns the solenoid on
float FootPedal = 0;                      //State of the foot pedal
float AnalogPedal = 0;                    //Defines the analog value of the varible pedal
float TimeForPulses = 1000;               //Base time for pulses on pulse mode
float StartOfPulses;                      //Marker for start of pulses
int DirectFrequency=1;                  //Value for the frequency on direct mode

int Mode;                                 //Mode of action
int MosfetState = LOW;                    //Mosfet state for the action High means activated Low deactivated

int LastModeState = LOW;
int ModeState;
long LastDebounceMode = 0;                // the last time the output pin was toggled

int LastUpState = LOW;
int UpState;
long LastDebounceUp = 0;                              

int LastDownState = LOW;
int DownState;
long LastDebounceDown = 0;

long LastDebounceLCD = 0;


unsigned long CurrentTime = 0;
unsigned long PreviousTime = 0;


long debounceDelay = 50;                  // the debounce time; increase if the output flickers

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  pinMode(SelectModePin, INPUT);          //Button to select the mode
  pinMode(UpButtonPin, INPUT);
  pinMode(MosfetPin, OUTPUT);             //Set solenoid pin as output to Mosfet
  pinMode(DigitalFootPedalPin, INPUT);    //Set the pedal input when pressed
  Serial.begin(115200);                   //Higher baudrate to get millis acuratelly

  //LCD Startup
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Mode:");
  WriteOnLCD();
  }

void WriteOnLCD(){          //Function that will be called to write the information on the LCD

  lcd.setCursor(5,0);
  if (Mode==1){
    lcd.print("Pulses     ");
  }
  if(Mode==0){
    lcd.print("Potenti    ");
  }
  if(Mode==2){
    lcd.print("Direct    ");
  }
  lcd.setCursor(0,1);
  lcd.print("Freq:  ");
  lcd.setCursor(7,1);
  lcd.print(FrequencyMosfet);
  
  
}




void loop() {
  int ModeRead = digitalRead(SelectModePin);
  int UpRead = digitalRead(UpButtonPin);
  int DownRead = digitalRead(DownButtonPin);
  
  
  if (millis()>=LastDebounceLCD){
    LastDebounceLCD=millis()+5000;
    //WriteOnLCD();
  }


  if (ModeRead != LastModeState) {                  //Reading the state for the Mode Button, function to change the mode of the machine
    // reset the debouncing timer
    LastDebounceMode = millis();
    //Serial.println(ModeState);
  } 
  
  if ((millis() - LastDebounceMode) > debounceDelay) {
    // if the button state has changed:
    if (ModeRead != ModeState) {
      ModeState = ModeRead;
      if (ModeState == HIGH) {
        
        if (Mode<2){
          Mode+=1;
        }
        else {
          Mode=0;
        }
        
        StartOfPulses=CurrentTime;
        PreviousTime=CurrentTime;
      }
    }
  }
  LastModeState = ModeRead;

  if (UpRead != LastUpState) {              //Reading the state for the up Button
    // reset the debouncing timer
    LastDebounceUp = millis();
    //Serial.println(ModeState);
  } 
  
  if ((millis() - LastDebounceUp) > debounceDelay) {
    // if the button state has changed:
    if (UpRead != UpState) {
      UpState = UpRead;
      
      if (UpState == HIGH) {
        
        if (DirectFrequency<=9){
          DirectFrequency+=1;
        }
        else if(DirectFrequency<=24){
          DirectFrequency+=5;
        }
        
      }
    }
  }
  LastUpState = UpRead;

  if (DownRead != LastDownState) {      //Reading the state for the Down Button
    // reset the debouncing timer
    LastDebounceDown = millis();
    //Serial.println(ModeState);
  } 
  
  if ((millis() - LastDebounceDown) > debounceDelay) {
    // if the button state has changed:
    if (DownRead != DownState) {
      DownState = DownRead;
      if (DownState == HIGH) {
        
        if (DirectFrequency>=11){
          DirectFrequency-=5;
        }
        else if(DirectFrequency>=2){
          DirectFrequency-=1;
        }
        
      }
    }
  }
  LastDownState = DownRead;

  
  
  
  if (Mode==1){               //Pulses mode 
    // read the state of the switch into a local variable
    CurrentTime = millis();
    //Serial.println(PercentageMosfet);

    FrequencyMosfet = map(analogRead(FrequencyPin),0,1023,1,50);
    AnalogPedal = map(round(analogRead(A3)),0,1024,0,100);
    float DutyRead = (map(analogRead(PercentagePin),0,1023,10,350));
    PercentageMosfet = DutyRead/1000;;

    
    
    PeriodMosfet = (1000/FrequencyMosfet);
    //Serial.println(PeriodMosfet);
    if (AnalogPedal>20){
      
      
      if (CurrentTime - StartOfPulses <= TimeForPulses/2){

        if(CurrentTime - PreviousTime >= PeriodMosfet){
          PreviousTime+=PeriodMosfet;
          if(MosfetState==LOW){
            MosfetState=HIGH;
            digitalWrite(MosfetPin, MosfetState);

          }
        }
      
        if(CurrentTime - PreviousTime >= (PeriodMosfet*PercentageMosfet)/100) {
          if(MosfetState==HIGH){
            MosfetState=LOW;
            digitalWrite(MosfetPin, MosfetState);
            

          }
        }
      }
      else if(CurrentTime - StartOfPulses >= TimeForPulses){
        StartOfPulses=CurrentTime;
        
        digitalWrite(MosfetPin,LOW);
      }
      else{
        digitalWrite(MosfetPin,LOW);
      }
    }
    else{
      digitalWrite(MosfetPin,LOW);
      StartOfPulses=CurrentTime;
      WriteOnLCD(); 
    }
  }      //Finish mode 0 (Pulses in a second)
  
  if (Mode==0){                   //Frequency selected on the potentiometers
    CurrentTime = millis();
    
    FrequencyMosfet = map(analogRead(FrequencyPin),0,1023,2,50);
    AnalogPedal = map(round(analogRead(A3)),0,1024,0,255);
    //Serial.println(PercentageMosfet);
    float DutyRead = (map(analogRead(PercentagePin),0,1023,10,350));
    PercentageMosfet = DutyRead/1000;
    
    PeriodMosfet = (1000/FrequencyMosfet);
    
    if (AnalogPedal>20){
      if(CurrentTime - PreviousTime >= PeriodMosfet){
        PreviousTime+=PeriodMosfet;
        if(MosfetState==LOW){
          MosfetState=HIGH;
          analogWrite(MosfetPin, AnalogPedal);
        }
      }
    
      if(CurrentTime - PreviousTime >= (PeriodMosfet*PercentageMosfet)) {
        if(MosfetState==HIGH){
          MosfetState=LOW;
          analogWrite(MosfetPin, 0);
        }
      }
    }
    else{
      digitalWrite(MosfetPin,LOW);
      StartOfPulses=CurrentTime;
      WriteOnLCD();      
    }    
  }      //Finish mode 1 (Direct Potentiometer Mode)

  if (Mode==2){                   //Frequency selected on the potentiometers
    CurrentTime = millis();
    
    FrequencyMosfet = map(analogRead(FrequencyPin),0,1023,1,50);
    AnalogPedal = map(round(analogRead(A3)),0,1024,0,255);
    //Serial.println(PercentageMosfet);
    float DutyRead = (map(analogRead(PercentagePin),0,1023,10,350));
    PercentageMosfet = DutyRead/1000;
    
    PeriodMosfet = (1000/FrequencyMosfet);
    
    if (AnalogPedal>20){
      if(CurrentTime - PreviousTime >= PeriodMosfet){
        PreviousTime+=PeriodMosfet;
        if(MosfetState==LOW){
          MosfetState=HIGH;
          digitalWrite(MosfetPin, MosfetState);
        }
      }
    
      if(CurrentTime - PreviousTime >= (PeriodMosfet*PercentageMosfet)) {
        if(MosfetState==HIGH){
          MosfetState=LOW;
          digitalWrite(MosfetPin, MosfetState);
        }
      }
    }
    else{
      digitalWrite(MosfetPin,LOW);
      StartOfPulses=CurrentTime;
      WriteOnLCD();       
    }    
  }      //Finish mode 2 (Direct Frequency)

}      //Loop end
