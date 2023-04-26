//setup,loop and distance related code from https://electro-nx.blogspot.com/2019/01/pathfinder-robot-full.html
#include "IRremote.h" // add Elegoo IR remote library 
#include <AFMotor.h>  // Add Adafruit Motor Shield for Arduino kit library.
//task main()
#define TrigPIN   7  // Assign PIN 7 as TrigPIN (Connect Arduino UNO "7" PIN with Ultrasonic   Sonar Sensor "Trig" PIN).
#define EchoPIN 8  // Assign PIN 8 as EchoPIN (Connect   Arduino UNO "8" PIN with Ultrasonic Sonar Sensor "Trig" PIN).
#define DCMROFF 10  // This sets Offset   to allow differences between the two DC traction Motors.
#define LEDBPIN 0    // Assign PIN 5v as LEDBPIN (Connect Arduino UNO "5v" PIN with RGB Diffused   Common Cathode "LEDB" PIN).
#define LEDGPIN 0  // Assign PIN 0 as LEDGPIN   (Connect Arduino UNO "0" PIN with RGB Diffused Common Cathode "LEDG" PIN).
#define   LEDRPIN 0  // Assign PIN 0 as LEDRPIN (Connect Arduino UNO "0" PIN with RGB   Diffused Common Cathode "LEDR" PIN).
AF_DCMotor  M1 (1, MOTOR12_64KHZ);   // Create DCMotor #1 using M1 output, Set to 64kHz PWM frequency.
AF_DCMotor   M2 (2, MOTOR12_64KHZ); // Create DCMotor #2 using M2 output, Set to 64kHz PWM frequency.
AF_DCMotor M3 (3, MOTOR12_64KHZ);
AF_DCMotor M4 (4, MOTOR12_64KHZ);
int RightDistance, LeftDistance;  // Distances on either side.
float Distance = 0.00;            // Float type variable declaration.

int photocellPin = 0;     // the cell and 10K pulldown are connected to a0 // taken from https://learn.adafruit.com/photocells/arduino-code 
int photocellReading;  // the analog reading from the sensor divider


int receiver = 9; // Signal Pin of IR receiver to Arduino Digital Pin 9

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'                         // code taken from Elegoo handbook on IR sensors
decode_results results;      // create instance of 'decode_results'

/*-----( Function )-----*/
void translateIR() // takes action based on IR code received

// describing Remote IR codes 

{

  switch(results.value)

  {
  case 0xFFA25D: Serial.println("POWER"); break;
  case 0xFFE21D: Serial.println("FUNC/STOP"); break;
  case 0xFF629D: Serial.println("VOL+"); break;
  case 0xFF22DD: Serial.println("FAST BACK");    break;
  case 0xFF02FD: Serial.println("PAUSE");    break;
  case 0xFFC23D: Serial.println("FAST FORWARD");   break;
  case 0xFFE01F: Serial.println("DOWN");    break;
  case 0xFFA857: Serial.println("VOL-");    break;    // hexadecimal values for IR remote buttons not used
  case 0xFF906F: Serial.println("UP");    break;
  case 0xFF9867: Serial.println("EQ");    break;
  case 0xFFB04F: Serial.println("ST/REPT");    break;
  case 0xFF6897: Serial.println("0");    break;
  case 0xFF30CF: Serial.println("1");    break;
  case 0xFF18E7: Serial.println("2");    break;
  case 0xFF7A85: Serial.println("3");    break;
  case 0xFF10EF: Serial.println("4");    break;
  case 0xFF38C7: Serial.println("5");    break;
  case 0xFF5AA5: Serial.println("6");    break;
  case 0xFF42BD: Serial.println("7");    break;
  case 0xFF4AB5: Serial.println("8");    break;
  case 0xFF52AD: Serial.println("9");    break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");break;  

  default: 
    Serial.println(" other button : ");
    Serial.println(results.value);

  }// End Case

  delay(500); // Do not get immediate repeat


} //END translateIR



void setup()   /*----( SETUP: RUNS ONCE )----*/
{
 
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver
  pinMode (TrigPIN, OUTPUT);  // Declare TrigPIN as "Output   PIN".
  pinMode (EchoPIN, INPUT);   // Declare EchoPIN as "Output PIN".
 pinMode (LEDBPIN, OUTPUT);  // Declare LEDBPIN as "Output PIN".
  pinMode   (LEDGPIN, OUTPUT);  // Declare LEDGPIN as "Output PIN".
  pinMode (LEDRPIN,   OUTPUT);  // Declare LEDRPIN as "Output PIN".  

}/*--(end setup )---*/



  



//2016.12.9
int Search (void) {                   //   Integer type variable declaration.
  float Duration = 0.0;               // Float   type variable declaration.
  float CM = 0.0;                     // Float type   variable declaration.
  digitalWrite (TrigPIN, LOW);        // TrigPIN output   as 0V (Logic low level).
  delayMicroseconds (2);              // Delay for 2us,   Send 10 us high pulse to Ultrasonic Sonar Sensor "TrigPIN".
  digitalWrite   (TrigPIN, HIGH);       // TrigPIN output as 5V (Logic high level).
  delayMicroseconds   (10);             // Delay for 10us.
  digitalWrite (TrigPIN, LOW);        //   TrigPIN output as 0V (Logic low level).
  Duration = pulseIn (EchoPIN, HIGH);   // Start counting time, Upto again EchoPIN back to logic "High Level" and putting   the "Time" into variable called "Duration".
  CM = (Duration/58.8);               //   Convert Distance into CM.
  return CM;                          // Return to   CM.
}

  



void Forward () {   // Forward loop.

 M1.run   (FORWARD); // Turn DCMotor #1 to Forward.
 M2.run   (FORWARD); // Turn DCMotor #1 to Forward.
 M3.run   (FORWARD); // Turn DCMotor #1 to Forward.
 M4.run   (FORWARD); // Turn DCMotor #1 to Forward.

}
void Backward () {    // Backward loop.
  M1.run (BACKWARD);   // Turn DCMotor #1 to Backward.
  M2.run (BACKWARD);  // Turn DCMotor #2 to   Backward.
  M3.run (BACKWARD);  // Turn DCMotor #2 to   Backward.
  M4.run (BACKWARD);  // Turn DCMotor #2 to   Backward.
  delay (500);       // Delay for 1s.
}
void TurnRight () {           //   Right Turn loop.
  M1.run (BACKWARD);          // Turn DCMotor #1 to Backward.
   M2.run (FORWARD);           // Turn DCMotor #2 to Forward.
  M1.setSpeed (100+DCMROFF);   // Calibrate the Speed of DCMotor #1.
  M3.run (BACKWARD);          // Turn DCMotor #1 to Backward.
   M4.run (FORWARD);           // Turn DCMotor #2 to Forward.
  M3.setSpeed (100+DCMROFF);   // Calibrate the Speed of DCMotor #1.
  delay (300);                // Delay   for 0.7s.
}
void TurnLeft () {            // Left Turn loop.
  M1.run (FORWARD);            // Turn DCMotor #1 to Forward.
  M2.run (BACKWARD);          // Turn   DCMotor #2 to Backward.
  M2.setSpeed (100+DCMROFF);  // Calibrate the Speed   of DCMotor #2.
  delay (300);                // Delay for 0.7s.
  M3.run (FORWARD);            // Turn DCMotor #1 to Forward.
  M4.run (BACKWARD);          // Turn   DCMotor #2 to Backward.
  M4.setSpeed (100+DCMROFF);  // Calibrate the Speed   of DCMotor #2.
  delay (300);                // Delay for 0.7s.
}
void   TurnAround () {          // Trun Around loop.
  M1.run (FORWARD);           //   Turn DCMotor #1 to Forward.
  M2.run (BACKWARD);          // Turn DCMotor #2   to Backward.
  M2.setSpeed (100+DCMROFF);  // Calibrate the Speed of DCMotor   #2.
  delay (700);               // Delay for 2.1s.
  M3.run (FORWARD);           //   Turn DCMotor #1 to Forward.
  M4.run (BACKWARD);          // Turn DCMotor #2   to Backward.
  M4.setSpeed (100+DCMROFF);  // Calibrate the Speed of DCMotor   #2.
  delay (700);               // Delay for 2.1s.
  
}
void Stop () {      //   Stop loop.
  M1.run (RELEASE); // Release DCMotor #1.
  M2.run (RELEASE);   // Release DCMotor #2.
  delay (100);      // Delay for 0.1s.
  M3.run (RELEASE); // Release DCMotor #1.
  M4.run (RELEASE);   // Release DCMotor #2.
  delay (100);      // Delay for 0.1s.
}



void loop () {                                    //   Main loop.
if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  

 if(results.value == 3810010651){ // code for when the power button is pressed
   M1.run (RELEASE); // Release DCMotor #1.
   M2.run (RELEASE);   // Release DCMotor #2.
   delay (100);      // Delay for 0.1s.
   M3.run (RELEASE); // Release DCMotor #1.
   M4.run (RELEASE);   // Release DCMotor #2.
   delay (100);      // Delay for 0.1s.
 }
 if(results.value == 718767612 ){   // code for when the play button is pressed
   M1.run (FORWARD); // Release DCMotor #1.
   M2.run (FORWARD);   // Release DCMotor #2.
   delay (100);      // Delay for 0.1s.
   M3.run (FORWARD); // Release DCMotor #1.
   M4.run (FORWARD);   // Release DCMotor #2.
   delay (100);      // Delay for 0.1s.
 }
  photocellReading = analogRead(photocellPin);  // reads voltage from Arduino 
Serial.print("Analog reading = ");
  Serial.println(photocellReading);     // the raw analog reading
  delay(100);
  
 if(photocellReading > 290){  // When photocell reading is higher than a certain reading
   M1.setSpeed (400);                      // Robot set to lowest speed
   M2.setSpeed (400);
   M3.setSpeed (400);
   M4.setSpeed (400);
 }
  

  delay (100);                                    //   Delay for 0.1s.
  Distance = Search ();                           // Measuring   the Distance in CM.
  if (Distance < 5) {                             // if obstacle found below 5cm
    digitalWrite (LEDBPIN, HIGH);                  //   LEDBPIN output as 0V (Logic low level).
    digitalWrite (LEDGPIN, HIGH);                  //   LEDGPIN output as 0V (Logic low level).
    digitalWrite (LEDRPIN, LOW);                 //   LEDRPIN output as 5V (Logic high level).
    Backward(); 
   M1.setSpeed (400);                      // Robot set to lowest speed
   M2.setSpeed (400);
   M3.setSpeed (400);
   M4.setSpeed (400);


  }
  else if (Distance >5) { // If obstacle found above 5cm
    digitalWrite (LEDBPIN, HIGH);                  //   LEDBPIN output as 0V (Logic low level).
    digitalWrite (LEDGPIN, LOW);                  //   LEDGPIN output as 0V (Logic low level).
    digitalWrite (LEDRPIN, LOW);                 //   LEDRPIN output as 5V (Logic high level). 
    M1.setSpeed (0); // robot set to highest speed
    M2.setSpeed (0);
    M3.setSpeed (0);
    M4.setSpeed (0);
    Forward ();                                   //   Robot move to Forward direction.
  }
  else if (Distance >10 ) { // If obstacle found above 10cm
       digitalWrite   (LEDBPIN, LOW);                  // LEDBPIN output as 0V (Logic low level).
       digitalWrite (LEDGPIN, HIGH);                 // LEDGPIN output as 5V (Logic   high level).
       digitalWrite (LEDRPIN, LOW);                  // LEDRPIN output   as 0V (Logic low level).
       M1.setSpeed (0);
       M2.setSpeed (0);
       M3.setSpeed (0);
       M4.setSpeed (0);
       Forward ();                                   // Robot move to Forward direction.
   }
  else {                                          // If obstacle cannot be   found in 10cm.
     digitalWrite (LEDBPIN, HIGH);                 //   LEDBPIN output as 5V (Logic high level).
     digitalWrite (LEDGPIN, HIGH);                 //   LEDGPIN output as 5V (Logic high level).
     digitalWrite (LEDRPIN, HIGH);                 //   LEDRPIN output as 5V (Logic high level).
       M1.setSpeed (0);
       M2.setSpeed (0);
       M3.setSpeed (0);
       M4.setSpeed (0);
       Forward ();                                   // Robot move   to Forward direction.
  }
}


