#include <CheapStepper.h>

const int stepsPerRevolution = 4096;
const int steps_16 = 3133; //0.765
const int steps_turn = 1220; //0.3

CheapStepper Left (6,7,8,9); 
CheapStepper Right (10,11,12,13); 

//UltraSonic Sensors
int USfT = 3, USfE = 18;
int USlT = 4, USlE = 2;
int USrT = 5, USrE = 19;

const int len = 16;
int inf = len*len;

int pos_x = 0, pos_y=0;


float USdist(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return 0.01723*pulseIn(echoPin, HIGH); //dist in cm
}

/*Check Walls-----------------------------------------------------------------------------------------------------*/
bool forWall(){
  if(USdist(USfT,USfE) < 5) return true
  return false;
}

bool leftWall(){
  if(USdist(USlT,USlE) < 5) return true
  return false;
}

bool rightWall(){
  if(USdist(USrT,USrE) < 5) return true
  return false;
}
/*----------------------------------------------------------------------------------------------------------------*/


/*Movement--------------------------------------------------------------------------------------------------------*/
void moveFor(){
  for(int i=0;i<steps_16;i++){
      Left.stepCCW();
      Right.stepCCW();
   }
}

void moveBack(){
  for(int i=0;i<steps_16;i++){
      Left.stepCW();
      Right.stepCW();
   }
}

void turnRight(){
  for(int i=0;i<steps_turn;i++){
      Left.stepCCW();
      Right.stepCW();
  }
}

void turnLeft(){
  for(int i=0;i<steps_turn;i++){
      Left.stepCW();
      Right.stepCCW();
  }
}
/*----------------------------------------------------------------------------------------------------------------*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

}
