/*
  #################################################################
  #                                                               #
  #                     Motorcontroller                           #
  #                                                               #
  #---------------------------------------------------------------#
  #                                                               #
  #              Pin 5: Enable Pin for Motor 1                    #
  #              Pin 6: Enable Pin for Motor 2                    #
  #                                                               #
  #               Pin 8: PWM 1 for Motor  1                       #
  #               Pin 9: PWM 2 for Motor  1                       #
  #                                                               #
  #               Pin 10: PWM 1 for Motor 2                       #
  #               Pin 11: PWM 2 for Motor 2                       #
  #                                                               #
  #---------------------------------------------------------------#
  #---------------------------------------------------------------#
  #                                                               #
  #                         Lights                                #
  #                                                               #
  #---------------------------------------------------------------#
  #                                                               #
  #                   Pin 7 : Blinker Left                        #
  #                   Pin 4 : Blinker Right                       #
  #                   Pin 12: Breaking Light                      #
  #                                                               #
  #---------------------------------------------------------------#
  #---------------------------------------------------------------#
  #                                                               #
  #                         Ultrasonic                            #
  #                                                               #
  #---------------------------------------------------------------#
  #                                                               #
  #                       Pin 2 : Trigger                         #
  #                       Pin 3 : Echo                            #
  #---------------------------------------------------------------#
  #---------------------------------------------------------------#
  #                                                               #
  #                            CNY70                              #
  #                                                               #
  #---------------------------------------------------------------#
  #                                                               #
  #               analog Pin 5 : Optoelectronic 1                 #
  #               analog Pin 4 : Optoelectronic 2                 #
  #                                                               #
  #---------------------------------------------------------------#
  #################################################################
*/
//Define Start Variables\\

char command = 'S'; //Set the command to S (Stop)
char prevCommand = 'A';

#define MotorI 5
#define MotorII 6

#define Motor1_1 8
#define Motor1_2 9

#define Motor2_1 10
#define Motor2_2 11

#define BlinkerL 7
#define BlinkerR 4
#define Bremse 12

#define LED 13

#define Trigger 2
#define Echo 3
#define Led 13

int entfernung = 1; //Variable to save the distance
int centi = 1;      //Variable to save distance in cm

int lastf = 0;      //Variable to store if the car was driving straight

int ledState = LOW; //Startvalue of the clock

unsigned long previousMillis = 0;
const long interval = 667;        //Interval of the clock

bool bl = false;    //Boolean for left blinker
bool br = false;    //Boolean for right blinker

int sensorValue = 0;    //Save last Value of Optoelectronic 1
int sensorValueTwo = 0; //Save last Value of Optoelectronic 2


void setup()
{
  Serial.begin(9600);

  pinMode(Echo, INPUT);
  pinMode(Trigger, OUTPUT);
  pinMode(Led, OUTPUT);

  pinMode(MotorI, OUTPUT);
  pinMode(MotorII, OUTPUT);

  pinMode(Motor1_1, OUTPUT);
  pinMode(Motor1_2, OUTPUT);

  pinMode(Motor2_1, OUTPUT);
  pinMode(Motor2_2, OUTPUT);

  pinMode(BlinkerL, OUTPUT);
  pinMode(BlinkerR, OUTPUT);
  pinMode(Bremse, OUTPUT);

  pinMode(LED, OUTPUT);

  digitalWrite(MotorI, HIGH);
  digitalWrite(MotorII, HIGH);
}

void loop() {


  //         Programm to measure the distance         \\ 
  entfernung = 0; //Set distance to 0                  ||
  centi = 0;      //Set distance in cm to 0            ||
  //                                                   ||
  digitalWrite(Trigger, HIGH);                         ||
  delayMicroseconds(10);                               ||
  digitalWrite(Trigger, LOW);                          ||
  entfernung = pulseIn(Echo, HIGH);                    ||
  centi = entfernung / 2 / 29, 1;                      ||
  //                                                   ||
  if ((centi <= 10) and (centi >= 0) and (lastf == 1)) {
  //                                                   ||
    digitalWrite(Motor1_1, LOW); //Break               ||
    digitalWrite(Motor1_2, LOW); //Break               ||
    digitalWrite(Motor2_1, LOW); //Break               ||
    digitalWrite(Motor2_2, LOW); //Break               ||
    digitalWrite(Bremse, HIGH);  //Break light         ||
  }//                                                  ||
  //___________________________________________________||
 


 
  //                   Line holding warning                    \\
  sensorValue = analogRead(5);    //Value of Optoelectronic 1   ||
  sensorValueTwo = analogRead(4); //Value of Optoelectronic 2   ||
  //                                                            ||
  if ((sensorValue > 100) or (sensorValueTwo > 100)) {//        ||
    digitalWrite(LED, HIGH);      //Detected black -> LED on    ||
  }//                                                           ||
  //____________________________________________________________|| 


   //                   Bluetooth commands                       \\
  if (Serial.available()) {       //Look if it's connected
    prevCommand = command;        //Save last executed command                                                   
    command = Serial.read();      //Recieve the serial Data      
    if (command != prevCommand) { //Prevent commands from being executed multiple times

      digitalWrite(Bremse, LOW);  //Turn breaking light off

      //Serial.println(command);    //For debugging     
      switch (command) {            //Find out which command was sent
        case 'F':                   //Front

          analogWrite(MotorI, 255);
          analogWrite(MotorII, 255);
          digitalWrite(Motor1_1, LOW);
          digitalWrite(Motor1_2, HIGH);
          digitalWrite(Motor2_1, LOW);
          digitalWrite(Motor2_2, HIGH);
          lastf = 1;
          break;
        case 'B':                   //Back
          analogWrite(MotorI, 255);
          analogWrite(MotorII, 255);

          digitalWrite(Motor1_1, HIGH);
          digitalWrite(Motor1_2, LOW);
          digitalWrite(Motor2_1, HIGH);
          digitalWrite(Motor2_2, LOW);
          lastf = 0;
          break;
        case 'S':                   //Stop
          digitalWrite(Motor1_1, LOW);
          digitalWrite(Motor1_2, LOW);
          digitalWrite(Motor2_1, LOW);
          digitalWrite(Motor2_2, LOW);

          digitalWrite(Bremse, HIGH);
          lastf = 0;
          break;
        case 'G':                 
          analogWrite(MotorI, 150);
          analogWrite(MotorII, 255);

          digitalWrite(Motor1_1, LOW);
          digitalWrite(Motor1_2, HIGH);
          digitalWrite(Motor2_1, LOW);
          digitalWrite(Motor2_2, HIGH);
          lastf = 1;
          break;
        case 'I':                   
          analogWrite(MotorI, 255);
          analogWrite(MotorII, 150);

          digitalWrite(Motor1_1, LOW);
          digitalWrite(Motor1_2, HIGH);
          digitalWrite(Motor2_1, LOW);
          digitalWrite(Motor2_2, HIGH);
          lastf = 1;
          break;
        case 'L':                
          analogWrite(MotorI, 50);
          analogWrite(MotorII, 255);

          digitalWrite(Motor1_1, LOW);
          digitalWrite(Motor1_2, HIGH);
          digitalWrite(Motor2_1, LOW);
          digitalWrite(Motor2_2, HIGH);
          lastf = 0;
          break;
        case 'R':                   
          analogWrite(MotorI, 255);
          analogWrite(MotorII, 50);

          digitalWrite(Motor1_1, LOW);
          digitalWrite(Motor1_2, HIGH);
          digitalWrite(Motor2_1, LOW);
          digitalWrite(Motor2_2, HIGH);
          lastf = 0;
          break;
        case 'H':               
          analogWrite(MotorI, 150);
          analogWrite(MotorII, 255);

          digitalWrite(Motor1_1, HIGH);
          digitalWrite(Motor1_2, LOW);
          digitalWrite(Motor2_1, HIGH);
          digitalWrite(Motor2_2, LOW);
          lastf = 0;
          break;
        case 'J':                 
          analogWrite(MotorI, 255);
          analogWrite(MotorII, 150);

          digitalWrite(Motor1_1, HIGH);
          digitalWrite(Motor1_2, LOW);
          digitalWrite(Motor2_1, HIGH);
          digitalWrite(Motor2_2, LOW);
          lastf = 0;
          break;
        case 'W':
          bl = true;
          break;
        case 'w':
          bl = false;
          digitalWrite(BlinkerL, LOW);
          break;
        case 'U':
          br = true;
          break;
        case 'u':
          br = false;
          digitalWrite(BlinkerR, LOW);
          break;
        case 'X':
          bl = true;
          br = true;
          break;
        case 'x':
          br = false;
          digitalWrite(BlinkerR, LOW);
          bl = false;
          digitalWrite(BlinkerL, LOW);
          break;
        case 'v':
          digitalWrite(LED, LOW);
          break;
        case 'V':
          digitalWrite(LED, LOW);
          break;
      }
    }
  }


//                                 Programm to read Thermistor value                          \\
double Thermistor(int RawADC) { //                                                             || 
  double Temp;                                                                                 ||
  Temp = log(10000.0 * ((1024.0 / RawADC - 1)));  //Convert Temp                               ||
  //         =log(10000.0/(1024.0/RawADC-1))      //for pull-up configuration                  ||
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp ); //      ||
  Temp = Temp - 273.15;             // Convert Kelvin to Celcius                               ||
  Temp = (Temp * 9.0) / 5.0 + 32.0; // Convert Celcius to Fahrenheit                           ||
  return Temp; //                                                                              ||
} //                                                                                           ||
//_____________________________________________________________________________________________|| 
