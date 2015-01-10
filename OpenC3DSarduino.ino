
 


 //Actuador
 #define STEPPER_1 1
 
 #define LASER_1 2
 #define LASER_2 3
 #define LASER_3 5
 #define LASER_4 6
 

 //State
 #define ON 1
 #define OFF 0
 
 //Comands
 #define DEG 1 // Degrees
 #define STEP 2 // Steps
 #define SEN 3 //Sense
 #define SPEED 4 //Speed
 #define INI 5  // Initial position
 #define R 6  // Red Color
 #define G 7  // Green Color
 #define B 8  // Blue Color
 #define HOME 9 // Home


//PINS CONNECTION
#define STEPPER_1_DIR_PIN 9
#define STEPPER_1_STEP_PIN 8

#define LASER_1_PIN 2
#define LASER_2_PIN 3
#define LASER_3_PIN 4
#define LASER_4_PIN 5
#define HOME_PIN 6
#define END_PIN 10




typedef struct {
  
  int active;
  int steps;
  int dir;
  float degrees,speed;
  int ini;
  int r,g,b;
  float pabs;
} Actuador;

Actuador Stepper1,DCA,LED;
float aux;
unsigned long int stp;

void setup() {
Stepper1.dir=1;
Stepper1.speed=1;


  // initialize serial:
  pinMode(STEPPER_1_DIR_PIN, OUTPUT); 
  pinMode(STEPPER_1_STEP_PIN, OUTPUT);
 
  pinMode(LASER_1_PIN, OUTPUT); 
  pinMode(LASER_2_PIN, OUTPUT); 
  pinMode(LASER_3_PIN, OUTPUT); 
  pinMode(LASER_4_PIN, OUTPUT); 
  
  pinMode(HOME_PIN, INPUT);
  pinMode(END_PIN, INPUT);
  

  
 stp=0;
  Serial.begin(115200);
  //Serial.setTimeout(3);
  establishContact();
}

void loop() {

  

  while (Serial.available() > 0) {
 
    // look for the next valid integer in the incoming serial stream:
    int disp = Serial.parseInt();
    // do it again:
    int act = Serial.parseInt();
    // do it again:
    int cmd = Serial.parseInt();
    
    float val = Serial.parseFloat();

    // look for the newline. That's the end of your
    // sentence:
    if (Serial.read() == '\n') {
    
      
      if(disp==STEPPER_1)
      {
        if(act==ON)

        {
          if (cmd==SEN){
              if(val==0){
                Stepper1.dir=1;
              }
              if(val==1){
                Stepper1.dir=-1;
              }
           Serial.write(69);
          }
          
        if (cmd==SPEED){
            Stepper1.speed=(((float)val)/100.);
            Serial.write(69);
        }
        if (cmd==DEG){
            Stepper1.degrees=val*Stepper1.dir;
            stepper_rotateDeg(STEPPER_1_DIR_PIN,STEPPER_1_STEP_PIN,Stepper1.degrees, Stepper1.speed);
            Serial.write(69);
        }
        if (cmd==STEP){
            Stepper1.steps=val;
            stepper_rotate(STEPPER_1_DIR_PIN,STEPPER_1_STEP_PIN,Stepper1.steps, Stepper1.speed);
            Serial.write(69);
         }
        if (cmd==HOME){
          
         
        }
        
      }
    }
  
      
      if(disp==LASER_1)
      {
        if(act==ON)
        {
          digitalWrite(LASER_1_PIN, HIGH);Serial.write(70);
        }
        if(act==OFF)
        {
          digitalWrite(LASER_1_PIN, LOW);Serial.write(71);
          
        }
      }
    if(disp==LASER_2)
      {
        if(act==ON)
        {
          digitalWrite(LASER_2_PIN, HIGH);Serial.write(72);
          
        }
        if(act==OFF)
        {
          digitalWrite(LASER_2_PIN, LOW);Serial.write(73);
          
        }
      }
    if(disp==LASER_3)
      {
        if(act==ON)
        {
          digitalWrite(LASER_3_PIN, HIGH);Serial.println('F');
        
        }
        if(act==OFF)
        {
          digitalWrite(LASER_3_PIN, LOW);Serial.println('G');
          //delay(2);
        }
      }

    if(disp==LASER_4)
      {
        if(act==ON)
        {
          digitalWrite(LASER_4_PIN, HIGH);Serial.write(69);
          //delay(2);
        }
        if(act==OFF)
        {
          digitalWrite(LASER_4_PIN, LOW);Serial.write(69);
          //delay(2);
        }
      }

    }
  

  }

}


void stepper_rotate(int DIR_PIN,int STEP_PIN, long int steps, float speed){ 

  int dir = (steps > 0)? HIGH:LOW;
  
  digitalWrite(DIR_PIN,dir);


  steps = abs(steps);
   
  float usDelay = (1/speed) * 100;

  for(unsigned long int i=0; i < steps; i++){ 
    
 
    digitalWrite(STEP_PIN, HIGH); 
    delayMicroseconds(usDelay); 

    digitalWrite(STEP_PIN, LOW); 
    delayMicroseconds(usDelay); 
  } 
} 

void stepper_rotateDeg(int DIR_PIN,int STEP_PIN,float deg, float speed){ 

  int dir = (deg > 0)? HIGH:LOW;

  int steps = fabs(deg)*(1/0.1125); // Funtion of transmission relation
  float usDelay = (1/speed) * 100;

  for(int i=0; i < steps; i++){ 
    digitalWrite(STEP_PIN, HIGH); 
    delayMicroseconds(usDelay); 

    digitalWrite(STEP_PIN, LOW); 
    delayMicroseconds(usDelay); 
  } 
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("Echo3");   // send an initial string
    delay(300);
  }
}



