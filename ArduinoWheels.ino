
//setting all 4 Pins for steper motor and driving directions
boolean reverse = 0;
boolean forward = 1;
int STPIN_1 = 10;
int STPIN_2 = 11;
int STPIN_3 = 12;
int STPIN_4 = 13;
int Mdelay = 4; // this defines the delay between the motor magnets turning on which in turn increases the speed as it is decreased
//Recommended minimum value for Mdelay is 4

//setting Pins for distance sensor
int Trig = 2;
int Echo = A5;
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int Hand = 9+15; // distance from hand in cm, opens when hand is 15 cm from end of mouth
int MaxDistance = 15 + 40; // distance that the hand can be from the sensor to be detected in cm

//setting Pins for Main lights to indicate forward and reverse movement 
int GLED = 7;
int RLED = 8;




void setup()
{
  //setup Stepper Motor 4 Pins(Magnets)
  pinMode(STPIN_1, OUTPUT);
  pinMode(STPIN_2, OUTPUT);
  pinMode(STPIN_3, OUTPUT);
  pinMode(STPIN_4, OUTPUT);
  //setup distance sensor
  pinMode(Trig, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(Echo, INPUT); // Sets the echoPin as an INPUT
  //setup of Serial for feedback
  Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed
  //setup LEDpins
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
}




//Distance functions which get the average of reads that can be set with delay in between

int getDistance(){
  // Clears the trigPin condition
  digitalWrite(Trig, LOW);
  delayMicroseconds(1);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds 
  digitalWrite(Trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(Trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds between sensor and obstruction
  duration = pulseIn(Echo, HIGH);
  // Calculating the distance from obstruction
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back) 
  // Displays the distance on the Serial Monitor
  return distance;
}

int Average (int Array[], int amountData){ // like the name implies takes the average of the array of distances recorded
   int n = amountData-1;
   int sum = 0;
   for(int i=0; i<n; i++){  //Summing all members of the Array
    sum = sum + Array[i];
   }
   int SUM = sum/n; //dividing sum by number of data points
   return SUM;
}

//Getting Array of values and Averaging them
int variableAverage(int amountData, int delays){
  
  int sensorValues[amountData]; //setup values array
  
  for(int i=0; i<amountData; i++){ // this collects all the data points into the same array (sensorValues[])
    sensorValues[i] = getDistance(); //Saving values of distance
    delay(delays);
    //Serial.print(sensorValues[i]);
    //Serial.print(", "); 
  }
  //Serial.println(); //this Serial.print and ones above print the recorded values for the average
  //Serial.print("and Average is ");
  
  int varAverage = Average(sensorValues, amountData);
  
  //Serial.print(varAverage);
  //Serial.println(" cm"); //these Serial.print and the one above prints the final average
  return varAverage;
}



// Main functions

int step_number = 0;
int StepMotor(bool dir){ // Execution of movement
  if(dir==1){ //turning on magnets in sequence in order to move forwards
    switch(step_number){ //the switch function only runs one of the cases each time the function is called
      case 0:
      digitalWrite(STPIN_1, HIGH);// Turn on Magnet 1 and turn off the rest
      digitalWrite(STPIN_2, LOW);
      digitalWrite(STPIN_3, LOW);
      digitalWrite(STPIN_4, LOW);
      break;
      case 1:
      digitalWrite(STPIN_1, LOW);
      digitalWrite(STPIN_2, HIGH);// Turn on Magnet 2 and turn off the rest
      digitalWrite(STPIN_3, LOW);
      digitalWrite(STPIN_4, LOW);
      break;
      case 2:
      digitalWrite(STPIN_1, LOW);
      digitalWrite(STPIN_2, LOW);
      digitalWrite(STPIN_3, HIGH);// Turn on Magnet 3 and turn off the rest
      digitalWrite(STPIN_4, LOW);
      break;
      case 3:
      digitalWrite(STPIN_1, LOW);
      digitalWrite(STPIN_2, LOW);
      digitalWrite(STPIN_3, LOW);
      digitalWrite(STPIN_4, HIGH);// Turn on Magnet 4 and turn off the rest
      break;
    } 
  }
  else{ //turning on magnets in sequence in order to move in reverse
    switch(step_number){ 
      case 0:
      digitalWrite(STPIN_1, LOW);
      digitalWrite(STPIN_2, LOW);
      digitalWrite(STPIN_3, LOW);
      digitalWrite(STPIN_4, HIGH);// Turn on Magnet 4 and turn off the rest
      break;
      case 1:
      digitalWrite(STPIN_1, LOW);
      digitalWrite(STPIN_2, LOW);
      digitalWrite(STPIN_3, HIGH);// Turn on Magnet 3 and turn off the rest
      digitalWrite(STPIN_4, LOW);
      break;
      case 2:
      digitalWrite(STPIN_1, LOW);
      digitalWrite(STPIN_2, HIGH);// Turn on Magnet 2 and turn off the rest
      digitalWrite(STPIN_3, LOW);
      digitalWrite(STPIN_4, LOW);
      break;
      case 3:
      digitalWrite(STPIN_1, HIGH);// Turn on Magnet 1 and turn off the rest
      digitalWrite(STPIN_2, LOW);
      digitalWrite(STPIN_3, LOW);
      digitalWrite(STPIN_4, LOW);
    } 
  }
  
  step_number++; //for stopping more efficiently
    if(step_number > 3){
      step_number = 0;
    }
}



int DistanceDriving(){ //determines direction and executes movement
  int Fdistance = variableAverage(2,1);
  //Serial.println(Fdistance);
  
  if(Fdistance > Hand and Fdistance < MaxDistance){
    digitalWrite(GLED, HIGH); // set Eye LEDs to green
    digitalWrite(RLED, LOW); // reset red LED
    
    for(int i = 0; i <= 3; i++){
      StepMotor(1); // set motor to go forwards 
    delay(Mdelay);
    } 
    
    //Serial.println(" Forward");
  }
  else if(Fdistance <= Hand){
    digitalWrite(RLED, HIGH); // set Eye LEDs to red
    digitalWrite(GLED, LOW); // reset green LED
    for(int i = 0; i <= 3; i++){
      StepMotor(0); // set motor to go backwards
    delay(Mdelay);
    }
    
    //Serial.println(" Backwards");
  }
  else{
    // Motor doesn't need to run
    digitalWrite(GLED, LOW); // reset green LED
    digitalWrite(RLED, LOW); // reset red LED
  }
  return;
}

//LED functions

void loop(){
  
  DistanceDriving();
  
}
