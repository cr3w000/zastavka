/*
KMZ Brno
Ovladani zastavky pro Faller car autobus
(c)2018 T. Bartulec
*/

//Nastaveni portu
const int analogInPin = A2;  // Analog input pin 
const int digitalOutPin = 13; // Digital output pin 

//magnet state mapping to pin value
const int MAGNET_OFF = 1;
const int MAGNET_ON = 0;

//velikost pole pro pocitani prumeru
const int PRUMER_SIZE = 10;
//Prah pro detekci prujezdu
const int PRAH = 20;
//Cas v zastavce. t=2*CAS_V_ZASTAVCE [ms]
const int CAS_V_ZASTAVCE = 4000;

int state = 0;

int sensorValue = 0;        // value read
int outputValue = 0;        // magnet control
int prumer_iterator = 0;
int prumer_matrix[PRUMER_SIZE];
int prumer;
int timer = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(digitalOutPin, OUTPUT);
  digitalWrite(digitalOutPin, MAGNET_OFF);


}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  //store sensor value
  prumer_matrix[prumer_iterator] = sensorValue;
  prumer_iterator++;

  //calculate median
  if(prumer_iterator == PRUMER_SIZE ){
    prumer=0;
    for(int i=0;i<PRUMER_SIZE;i++){
      prumer+=prumer_matrix[i];
    }
    prumer = prumer / PRUMER_SIZE;
    prumer_iterator=0;

    if(prumer < PRAH){
    //new state will be 1. check current one
      if(state == 0){
        //previous state was 0, report state change
        Serial.println();
        Serial.println("State changed to 1");

        //activate output and start timer
        digitalWrite(digitalOutPin, MAGNET_ON);
        Serial.println("Vystup ON");
        timer = CAS_V_ZASTAVCE;
      }
      state = 1;
    }
    else
    {
      //new state will be 0. check current one
      if(state == 1){
        //previous state was 1, report state change
        Serial.println();
        Serial.println("State changed to 0");     
      }
      state = 0;
    }
 
    // print the results to the Serial Monitor:
    //  Serial.print("prumer = ");
    //  Serial.println(prumer);
 }
  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  if(timer > 0){
    if(timer == 1){
      digitalWrite(digitalOutPin, MAGNET_OFF);
      Serial.println("Vystup OFF");
    }
    timer--;
 }
   
 delay(2);
}
