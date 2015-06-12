/* OpenColor3DScan communication
   based on Arduino serial event example
 */

#define FUNCT_TRANS_REL    0.1125 // motor function of transmission relation
#define MAX_BYTES_MESSAGE  200

// CONNECTION PINS
#define STEPPER_1_DIR_PIN       9
#define STEPPER_1_STEP_PIN      8

#define LASER_1_PIN             2
#define LASER_2_PIN             3
#define LASER_3_PIN             4
#define LASER_4_PIN             5
#define HOME_PIN                6
#define END_PIN                 10

#define STEPPER_1 10

#define LASER_1 1
#define LASER_2 2
#define LASER_3 3
#define LASER_4 4

// STATE
#define ON 1
#define OFF 0

// COMMANDS
#define DEG 1      // Degrees
#define STEP 2     // Steps
#define SEN 3      //Sense
#define SPEED 4    //Speed
#define INI 5      // Initial position

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

int disp, act, cmd;
int val;

boolean bError = false;

typedef struct {
  int dir;
  float vel;
  float deg;
  int steps;
} Stepper;

Stepper stepper1;

void setup() {
  // setup pins
  pinMode(STEPPER_1_DIR_PIN, OUTPUT);
  pinMode(STEPPER_1_STEP_PIN, OUTPUT);
  
  pinMode(LASER_1_PIN, OUTPUT);
  pinMode(LASER_2_PIN, OUTPUT);
  pinMode(LASER_3_PIN, OUTPUT);
  pinMode(LASER_4_PIN, OUTPUT);
  
  pinMode(HOME_PIN, INPUT);
  pinMode(END_PIN, INPUT);
  
  // init serial comm
  Serial.begin(115200);
  inputString.reserve(MAX_BYTES_MESSAGE);
  sendHelloMessageToOpenColor3DScan();
  
  // init steper1
  stepper1.dir = 1;
  stepper1.vel = 1;
  stepper1.deg = 1;
  stepper1.steps = 1;
}

void loop() {
	if(stringComplete) {
		//Serial.println(inputString);
		if(inputString.compareTo("hello") == 0){
			sendHelloMessageToOpenColor3DScan();
		}
		else{
                    // parse data
                    // http://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string
                    String chopString1 = getValue(inputString, ' ', 0);
                    String chopString2 = getValue(inputString, ' ', 1);
                    String chopString3 = getValue(inputString, ' ', 2);
                    String chopString4 = getValue(inputString, ' ', 3);
                    
                    if(chopString1 != NULL){
                      disp = chopString1.toInt();
                    }
                    if(chopString2 != NULL){
                      act = chopString2.toInt();
                    }
                    if(chopString3 != NULL){
                      cmd = chopString3.toInt();
                    }
                    if(chopString4 != NULL){
                      val = chopString4.toInt();
                    }
                    
                    //Serial.println(disp);		    
                    //Serial.println(act);
		    //Serial.println(cmd);
		    //Serial.println(val);
                    bError = true;
                    
                    if(disp == LASER_1){
                      if(act == ON){
                        digitalWrite(LASER_1_PIN, HIGH);
                        //Serial.println("laser1 on");
                        bError = false;
                      }
                      if(act == OFF){
                        digitalWrite(LASER_1_PIN, LOW);
                        //Serial.println("laser1 off");
                        bError = false;
                      }
                    }
                    else if(disp == LASER_2){
                      if(act == ON){
                        digitalWrite(LASER_2_PIN, HIGH);
                        //Serial.println("laser2 on");
                        bError = false;
                      }
                      if(act == OFF){
                        digitalWrite(LASER_2_PIN, LOW);
                        //Serial.println("laser2 off");
                        bError = false;
                      }
                    }
                    else if(disp == STEPPER_1){
                      if(act == ON){
                        if(cmd == SEN){
                          if(val == 0){
                            stepper1.dir = 1; // clockwise?
                            //Serial.println("stepper dir1");
                          }
                          if(val == 1){
                            stepper1.dir = -1;
                            //Serial.println("stepper dir-1");
                          }
                          bError = false;
                        }
                        if(cmd == SPEED){
                          stepper1.vel = ( (float)val /100.0);
                          //Serial.println("stepper vel");
                          bError = false;
                        }
                        if(cmd == DEG){
                          stepper1.deg = val * stepper1.dir;
                          //Serial.println("stepper rotate deg");
                          stepperRotateDeg(STEPPER_1_DIR_PIN, STEPPER_1_STEP_PIN, stepper1.deg, stepper1.vel);
                          bError = false;
                        }
                        if(cmd == STEP){
                          stepper1.steps = val;
                          //Serial.println("stepper rotate steps");
                          stepperRotateSteps(STEPPER_1_DIR_PIN, STEPPER_1_STEP_PIN, stepper1.steps, stepper1.vel);
                          bError = false;
                        }
                      }
                    }
                    else{
                      bError = true;
                    }
                
                    if(bError == true){
                      sendErrorMessageToOpenColor3DScan();
                    }
                    else{
                      sendAckMessageToOpenColor3DScan();
                    }

		}// end else where we parse data

		// prepare for next string
		inputString = "";
		stringComplete = false;
                disp = -1;
                act = -1;
                cmd = -1;
                val = -1;
	}
}

void serialEvent() {
	while (Serial.available()) {
		char inChar = (char)Serial.read(); // get the new byte:
	
		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:
		if (inChar == '\n') {
			stringComplete = true;
		}
		else{
			inputString += inChar;
		} 
	}
}

void sendErrorMessageToOpenColor3DScan(){
  Serial.print(0);
  Serial.print(0);
  Serial.print(0);
  Serial.print('A');
}

void sendAckMessageToOpenColor3DScan(){
  Serial.print(1);
  Serial.print(1);
  Serial.print(1);
  Serial.print('A');
}

void sendHelloMessageToOpenColor3DScan(){
  Serial.println("Hello OpenColor3DScan");
}

String getValue(String data, char separator, int index){
  
    int maxIndex = data.length()-1;
    int j=0;
    String chunkVal = "";

    for(int i=0; i<=maxIndex && j<=index; i++){
      chunkVal.concat(data[i]);

      if(data[i]==separator){
        j++;

        if(j>index){
          chunkVal.trim();
          return chunkVal;
        }    

        chunkVal = "";    
      }  
    }  
}

void stepperRotateSteps(int DIR_PIN,int STEP_PIN, long int steps, float speed){

  int dir = (steps > 0)? HIGH:LOW;
  digitalWrite(DIR_PIN, dir);

  steps = abs(steps);
  steps = min(steps, 1000000);
  float usDelay = 20;
  if(speed != 0){
    usDelay = (1/speed) * 100;
  }

  for(int i=0; i<steps; i++){
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(usDelay);

    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(usDelay);
  }
}

void stepperRotateDeg(int DIR_PIN,int STEP_PIN,float deg, float speed){
  
  int dir = (deg > 0)? HIGH:LOW;
  digitalWrite(DIR_PIN, dir);
  
  int steps = fabs(deg)*(1/FUNCT_TRANS_REL);
  steps = min(steps, 1000000);
  float usDelay = 20;
  if(speed != 0){
    usDelay = (1/speed) * 100;
  }
  
  for(int i=0; i<steps; i++){
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(usDelay);

    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(usDelay);
  }
}


