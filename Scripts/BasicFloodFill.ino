/* Flood Fill Algorithm for micromouse
   Author: Tapti Robotics Team */
   
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
int inf = 255;

int pos_x = 0, pos_y=0;

bool wall[len][len][4] = {false}; 

//nomenclature 
//int nor=0,east=1,sou=2,west=3;
int f,r,b,l;
int orien = 0;

byte forw[len][len] = {inf};
byte rev[len][len] = {inf};

void init_walls(){
  for(int i=0;i<len;i++){
    wall[i][len-1][0] = true;
    wall[i][0][3] = true;
    wall[len-1][i][2] = true;
    wall[0][i][1] = true; 
  }
}

void init_forw(){
  for(int i=0;i<len;i++){
    for(int j=0;j<len;j++){
      forw[i][j] = inf;
    }
  }
  forw[7][7]=0;forw[7][8]=0;forw[8][7]=0;forw[8][8]=0;
}

void init_rev(){
  for(int i=0;i<len;i++){
    for(int j=0;j<len;j++){
      rev[i][j] = inf;
    }
  }
  rev[0][0]=0;
}

void updOri(){
  if(orien == 0) f=0,r=1,b=2,l=3;
  if(orien == 1) f=1,r=2,b=3,l=0;
  if(orien == 2) f=2,r=3,b=0,l=1;
  if(orien == 3) f=3,r=0,b=1,l=2;
}
 
void Move(int x){ //x is future orien - current orien
  if(x==0) { //Go Forward
    for(int i=0;i<steps_16;i++){
      Left.stepCCW();
      Right.stepCCW();
    }
  }
  if(x==1){ //Turn Right
    
    for(int i=0;i<steps_turn;i++){
      Left.stepCCW();
      Right.stepCW();
    }

    for(int i=0;i<steps_16;i++){
      Left.stepCCW();
      Right.stepCCW();
    }
    
    //orientation changes
    orien = (orien+1)%4;
    updOri();
  }
  if(x==2) {//Go Back

    for(int i=0;i<steps_16;i++){
      Left.stepCW();
      Right.stepCW();
    }
    //orientation changes
    //orien = (orien+2)%4;
    //updOri();
    
  }
  if(x==3) {//Turn Left

    for(int i=0;i<steps_turn;i++){
      Left.stepCW();
      Right.stepCCW();
    }

    for(int i=0;i<steps_16;i++){
      Left.stepCCW();
      Right.stepCCW();
    }
    //orientation changes
    orien = (orien+3)%4;
    updOri();
  }
}

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

void Fill(int i,int j, byte a[len][len]){
    if(!wall[i][j][0] && a[i][j+1]>a[i][j]) a[i][j+1] = a[i][j] + 1;
    if(!wall[i][j][3] && a[i][j-1]>a[i][j]) a[i][j-1] = a[i][j] + 1;
    if(!wall[i][j][1] && a[i-1][j]>a[i][j]) a[i-1][j] = a[i][j] + 1;
    if(!wall[i][j][2] && a[i+1][j]>a[i][j]) a[i+1][j] = a[i][j] + 1;
}

void Fill_n(int n, byte a[len][len]){
  
  if(n==inf) return; //OPtimize this
  
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      if(a[i][j] == n)
        Fill(i,j,a);
    }
  }

}

void moveFrom(int i,int j,byte a[len][len]){
    if(a[i][j] == 0) return;

    int least = inf;
    int newOrien = orien;

    if(!wall[i][j][0] && a[i][j+1]<least) least = a[i][j+1];    
    if(!wall[i][j][1] && a[i-1][j]<least) least = a[i-1][j];
    if(!wall[i][j][2] && a[i+1][j]<least) least = a[i+1][j];
    if(!wall[i][j][3] && a[i][j-1]<least) least = a[i][j-1];
    
    if(!wall[i][j][0] && a[i][j+1]==least) {newOrien = 0; pos_y += 1;}    
    else if(!wall[i][j][1] && a[i-1][j]==least)  {newOrien = 1; pos_x -= 1;}
    else if(!wall[i][j][2] && a[i+1][j]==least)  {newOrien = 2; pos_x += 1;}
    else if(!wall[i][j][3] && a[i][j-1]==least)   {newOrien = 3; pos_y -= 1;}

    Move((newOrien - orien)%4);
}

void updWall(int i,int j){
    if(USdist(USfT,USfE)<5) wall[i][j][f] = true;
    if(USdist(USlT,USlE)<5) wall[i][j][l] = true;
    if(USdist(USrT,USrE)<5) wall[i][j][r] = true;
}

void allign(){
//  while(USdist(USlT,USlE)<1.5 or USdist(USlT,USlE)>2000){
//    Left.stepCCW();
//    Right.stepCW();
//  }
//  while(USdist(USrT,USrE)<1.5 or USdist(USrT,USrE)>2000){
//    Left.stepCW();
//    Right.stepCCW();
//  }
//  while(USdist(USfT,USfE)<2.5 or USdist(USfT,USfE)>2000){
//    Left.stepCW();
//    Right.stepCW();
//  }
}

void setup() {

    Serial.begin(9600);

    delay(7000);

    //Left.setRpm(22);
    //Right.setRpm(22);

    init_walls();
    init_forw();
    
    while(forw[pos_x][pos_y] != 0){
      updWall(pos_x,pos_y);
      Fill_n(0,forw);
      moveFrom(pos_x,pos_y,forw);
      allign();
    }

    init_rev();

    while(rev[pos_x][pos_y] != 0){
      updWall(pos_x,pos_y);
      Fill_n(0,rev);
      moveFrom(pos_x,pos_y,rev);
      allign();
    }


    //Shortest path
    while(forw[pos_x][pos_y] != 0){
      //updWall(pos_x,pos_y);
      //Fill_n(0,forw);
      moveFrom(pos_x,pos_y,forw);
      allign();
    }
}

void loop() {
    //No need
}
