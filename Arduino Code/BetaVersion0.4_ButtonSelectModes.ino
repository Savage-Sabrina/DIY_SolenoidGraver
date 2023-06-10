#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


//Assingning Analog Pins

const int FrequencyPin = 1;               //Pin that sets the maximun frequency (ANALOG)
const int PercentagePin = 2;              //Pin from percentage of the cycle that turns the solenoid on (ANALOG)

//Assigning digital Pins

const int VariablePedalPin = 2;              // Pin to select variable pedal mode (DIGITAL)
const int SelectedInputPin = 3;                //Pin to select potentiometer mode (DIGITAL)
const int PulsesPin = 4;              //Pin to select pulses pedal mode (DIGITAL)
const int MosfetPin = 9;                  // the number of the LED pin (DIGITAL)
const int DigitalFootPedalPin = 10;       //Pin of the foot pedal potentiometer (DIGITAL)


//Assigning variables

float PeriodMosfet  = 0;                  //Time between the impacts its a ratio between 1/frequency
float FrequencyMosfet = 1;                //Frequency in hertz of the impacts means impacts per second
float PercentageMosfet = 5;              //Percentage of the cycle that turns the solenoid on
float FootPedal = 0;                      //State of the foot pedal
float AnalogPedal = 0;                    //Defines the analog value of the varible pedal
float TimeForPulses = 1000;               //Base time for pulses on pulse mode
float StartOfPulses;                      //Marker for start of pulses
float DutyRead;                           //Duty cycle
float Correction;                          //Correction value for different solenoids
int DirectFrequency=1;                  //Value for the frequency on direct mode

int Mode=2;                                 //Mode of action starts at the mode you've defined below 
int MosfetState = LOW;                    //Mosfet state for the action High means activated Low deactivated

int LastVariableState = LOW;
int VariableState;
long LastDebounceMode = 0;                // the last time the output pin was toggled

int LastSelectedInputRead = LOW;
int SelectedInputState;
long LastDebounceSelectedInput = 0;                              

int LastPulsesState = LOW;
int DownState;
long LastDebouncePulses = 0;

long LastDebounceLCD = 0;


unsigned long CurrentTime = 0;
unsigned long PreviousTime = 0;


long debounceDelay = 50;                  // the debounce time; increase if the output flickers

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  pinMode(VariablePedalPin, INPUT);          //Button to select the mode
  pinMode (SelectedInputPin, INPUT);
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
    lcd.print("Potenti   ");
  }
  if(Mode==2){
    lcd.print("Variable   ");
  }
  lcd.setCursor(0,1);
  lcd.print("Freq:  ");
  lcd.setCursor(7,1);
  lcd.print(FrequencyMosfet);
  
  
}




void loop() {
  int VariableRead = digitalRead(VariablePedalPin);
  int SelectedInputRead = digitalRead (SelectedInputPin);
  int PulsesRead = digitalRead(PulsesPin);
  
  
  if (VariableRead != LastVariableState) {                  //Reading the state for the Mode Button, function to change the mode of the machine
    // reset the debouncing timer
    LastDebounceMode = millis();
    //Serial.println(VariableState);
  } 
  
  if ((millis() - LastDebounceMode) > debounceDelay) {
    // if the button state has changed:
    if (VariableRead != VariableState) {
      VariableState = VariableRead;
      if (VariableState == HIGH) {
        
        Mode=2;
        

      }
    }
  }
  LastVariableState = VariableRead;

  if (SelectedInputRead != LastSelectedInputRead) {              //Reading the state for the up Button
    // reset the debouncing timer
    LastDebounceSelectedInput = millis();
    //Serial.println(VariableState);
  } 
  
  if ((millis() - LastDebounceSelectedInput) > debounceDelay) {
    // if the button state has changed:
    if (SelectedInputRead != SelectedInputState) {
      SelectedInputState = SelectedInputRead;
      
      if (SelectedInputState == HIGH) {
        Mode = 0;

      }
    }
  }
  LastSelectedInputRead = SelectedInputRead;

  if (PulsesRead != LastPulsesState) {      //Reading the state for the Down Button
    // reset the debouncing timer
    LastDebouncePulses = millis();
    //Serial.println(VariableState);
  } 
  
  if ((millis() - LastDebouncePulses) > debounceDelay) {
    // if the button state has changed:
    if (PulsesRead != DownState) {
      DownState = PulsesRead;
      if (DownState == HIGH) {
        Mode = 1;
        
      }
    }
  }
  LastPulsesState = PulsesRead;

  
  
  
  if (Mode==1){               //Pulses mode 
    // read the state of the switch into a local variable
    CurrentTime = millis();
    //Serial.println(PercentageMosfet);

    FrequencyMosfet = map(analogRead(FrequencyPin),0,1023,1,40);
    AnalogPedal = map(round(analogRead(A3)),0,1024,0,255);
    DutyRead = (map(analogRead(PercentagePin),0,1023,10,350));
    PercentageMosfet = DutyRead/1000;;
    PeriodMosfet = (1000/FrequencyMosfet);

    if (AnalogPedal>20){
      
      
      if (CurrentTime - StartOfPulses <= TimeForPulses/2){

        if(CurrentTime - PreviousTime >= PeriodMosfet){
          PreviousTime+=PeriodMosfet;
          digitalWrite(MosfetPin, HIGH);
        }
      
        if(CurrentTime - PreviousTime >= (PeriodMosfet*PercentageMosfet)) {
          digitalWrite(MosfetPin, LOW);
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
      //Serial.println(StartOfPulses);
    }
  }      //Finish mode 0 (Pulses in a second)
  
  if (Mode==0){                   //Frequency selected on the potentiometers
    CurrentTime = millis();
    
    FrequencyMosfet = map(analogRead(FrequencyPin),0,1023,2,60);
    AnalogPedal = map(round(analogRead(A3)),0,1024,0,255);
    //Serial.println(PercentageMosfet);
    DutyRead = (map(analogRead(PercentagePin),0,1023,10,350));
    PercentageMosfet = DutyRead/1000;
    
    PeriodMosfet = (1000/FrequencyMosfet);
    
    if (AnalogPedal>20){
      if(CurrentTime - PreviousTime >= PeriodMosfet){
        PreviousTime+=PeriodMosfet;
        digitalWrite(MosfetPin, HIGH);

      }
    
      if(CurrentTime - PreviousTime >= (PeriodMosfet*PercentageMosfet)) {
        digitalWrite(MosfetPin, LOW);
      }
    }
    else{
      digitalWrite(MosfetPin,LOW);
      StartOfPulses=CurrentTime;
      PreviousTime=CurrentTime;
      WriteOnLCD();      
    }    
  }      //Finish mode 1 (Direct Potentiometer Mode)

  if (Mode==2){                   //Frequency selected on the potentiometers
    CurrentTime = millis();
    AnalogPedal = analogRead(A3); //map(round(analogRead(A3)),0,1024,0,255);
    FrequencyMosfet = map(AnalogPedal,0,1023,2,60);
    PeriodMosfet = (1000/FrequencyMosfet);
    Correction = (map(analogRead(PercentagePin),0,1023,1,10));  //The correction value is to accomodate different sizes of solenoid, it adjusts the curve of duty cycle up or down. Put two '/' at the begining if not using potentiometers and read line 240
    //Correction = 10 //If you are not using the potentiometer to correct the value, please test with differents values of the correction and uncomment (Remove the two '/' on the begining of the line)
    
    DutyRead = Correction*FrequencyMosfet + 4;
    //Serial.println(Correction);
    PercentageMosfet = DutyRead/1000;

    
    if (AnalogPedal>20){
      if(CurrentTime - PreviousTime >= PeriodMosfet){
        PreviousTime+=PeriodMosfet;
        digitalWrite(MosfetPin, HIGH);

      }
    
      if(CurrentTime - PreviousTime >= (PeriodMosfet*PercentageMosfet)) {
        digitalWrite(MosfetPin, LOW);
      }
    }
    else{
      digitalWrite(MosfetPin,LOW);
      StartOfPulses=CurrentTime;
      PreviousTime=CurrentTime;
      WriteOnLCD();       
    }    
  }      //Finish mode 2 (Direct Frequency)


}      //Loop end
