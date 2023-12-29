int R1 = 32;
int led = 21;//led
int R2 = 18; //sump
int R3 = 19; //level


#define FLOAT_HIGH 33
#define FLOAT_LOW 25
#define SUMP 26


int  on_button = 15;
int off_button = 23;


int data1;
int data2;
int percentage = 0;


//#define TRIGGER_PIN 14
//#define ECHO_PIN 27


#define MAX_PERCENTAGE 100



unsigned long previousMillis = 0;  // Variable to store the last time the LED was updated
const long interval = 5000;
int count = 0;
int count1 = 1;
int count2 = 0;

const int publishQoS = 1;
const int subscribeQoS = 1;

int device1;

const int resetPin = 4;
unsigned long buttonPressStart = 0;
bool reset_button_pressed = false;

void intializepins()
{
  Serial.begin(115200);
  pinMode(resetPin, INPUT);
  pinMode(R1, OUTPUT); //PINMODE DECLARATION
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(led, OUTPUT);//led
  pinMode(FLOAT_HIGH, INPUT_PULLUP);
  pinMode(FLOAT_LOW, INPUT_PULLUP);
  pinMode(SUMP, INPUT_PULLUP);

  pinMode(on_button, INPUT_PULLUP);
  pinMode(off_button, INPUT_PULLUP);
  digitalWrite(R1, LOW);
  digitalWrite(led, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(R3, LOW);
}
void blinkled3time()
{
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
}
