// Carp Constantin - Media Control Keyboard
#include <LiquidCrystal.h>

const byte potentiometru = 1; // potentiometru sensor pin A1
const byte X_axis = 2; // joystick x axis pin A2
const byte Y_axis = 3; // joystick y axis pin A3

const byte button_pause = 3; // pause button digital pin 3
const byte button_resume = 2; // resume button digital pin 2
const byte button_mute = 12; // mute button digital pin 12

const byte led_vol_indic = 5; // red vol indicating led pin 5 pwm
const byte led_pause = 11; // white pause led pin 11
const byte led_resume = 10; // red resume led pin 10
const byte led_mute_blue = 13; // blue mute led pin 13
const byte led_mute_green = 18; // green mute led pin A4->D18
const byte led_song_yellow = 19; // yellow prev/next led pin A5->D19

int16_t last_vol;
int16_t last_X_pos;
int16_t last_Y_pos;
String fst;
String snd;
byte mute;
String x;
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7; // LCD pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600); // Initialize Serial
  Serial.setTimeout(1);
  
  // init leds outputs
  pinMode(led_resume, OUTPUT);
  pinMode(led_pause, OUTPUT); 
  pinMode(led_mute_blue, OUTPUT); 
  pinMode(led_mute_green, OUTPUT);
  pinMode(led_song_yellow, OUTPUT);
  pinMode(led_vol_indic, OUTPUT);
  
  // init buttons
  pinMode(button_pause, INPUT_PULLUP);
  pinMode(button_resume, INPUT_PULLUP);
  pinMode(button_mute, INPUT_PULLUP);
  
  // attach interactions
  attachInterrupt(digitalPinToInterrupt(button_pause), pause_music, CHANGE); // set interrup on button press
  attachInterrupt(digitalPinToInterrupt(button_resume), resume_music, CHANGE); // set interrup on button press

  // variables
  last_vol = -1;
  last_X_pos = 0;
  last_Y_pos = 0;
  mute = 0;
  fst = ' ';
  snd = ' ';
 
  // manul interrupts
  PCICR |= B00000001; //Bit0 = 1 -> "PCIE0" enabeled (PCINT0 to PCINT7)
  PCMSK0 |= B00010000; //Bit4 = 1 -> "PCINT5" enabeled -> D12 will trigger interrupt

  // init system as unmuted
  digitalWrite(led_mute_green, HIGH);

  // display
  lcd.begin(16,2); // Initialize lcd
  lcd.print("Tastatura");
}

ISR (PCINT0_vect) // For PCINT of pins D8 a D13
{
  static int mute_state = 1; // contor for mute state
  static unsigned long last_interrupt_time = 0; // debouncing
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200){
    if (mute > 0)
      mute_state++;
    Serial.println("mute"); // send pause signal
    print_command("Mute"); 
    digitalWrite(led_pause, LOW); // set pause indicator low
    digitalWrite(led_resume, LOW); // set resume indicator low
    if (mute_state % 2 != 0){
      digitalWrite(led_mute_blue, HIGH); // Indicate muted
      digitalWrite(led_mute_green, LOW); // Indicate unmuted
    } 
    if (mute_state % 2 == 0){
      digitalWrite(led_mute_green, HIGH); // Indicate unmuted
      digitalWrite(led_mute_blue, LOW); // Indicate muted
    }
    mute_state++; // change the mute state or next interrupt
  }
  last_interrupt_time = interrupt_time; // save last time for debounce
  mute = 0;
}

void pause_music()
{
  static unsigned long last_interrupt_time = 0; // debouncing
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200){
    Serial.println("pause"); // send pause signal
    //print_command("Pause"); 
    digitalWrite(led_pause, HIGH); // set pause indicator high
    digitalWrite(led_resume, LOW); // set resume indicator low
    digitalWrite(led_mute_blue, LOW); // set mute indicator low
  }
  last_interrupt_time = interrupt_time;
}
void resume_music()
{
  static unsigned long last_interrupt_time = 0; // debouncing
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200){
    Serial.println("resume"); // send pause signal 
    //print_command("Play");
    digitalWrite(led_resume, HIGH); // set pause indicator high
    digitalWrite(led_pause, LOW); // set resume indicator low
    digitalWrite(led_mute_blue, LOW); // set mute indicator low
  }
  last_interrupt_time = interrupt_time;
}

void blink_led(){

  digitalWrite(led_song_yellow, HIGH);
  delay(100);
  digitalWrite(led_song_yellow, LOW);
  delay(100);
  digitalWrite(led_song_yellow, HIGH);
  delay(100);
  digitalWrite(led_song_yellow, LOW);
}

void print_command(String last_comm){
  lcd.setCursor(0,1);
  fst = "";
  snd = "Command: ";
  fst.concat(snd);
  fst.concat(last_comm);
  lcd.print(fst);
}

void loop() {
  
  int16_t pot_value = analogRead(potentiometru); // read values from potentiometru
  byte map_pot_value = map(pot_value, 0, 1023, 0, 51); // map the values
  byte map_pot_led = map(map_pot_value, 0, 51, 0, 255); // map the values
  analogWrite(led_vol_indic, map_pot_led);
  if (map_pot_value != last_vol) { // check if there was a change 
    if (last_vol < map_pot_value) { // verify if volume went up or down
      while(!Serial.available()); // wait for serial
      Serial.println("Up"); // send volume up signal
      //print_command("Up");
      if (digitalRead(led_mute_blue) == HIGH) {
        mute++;
        digitalWrite(led_mute_green, HIGH); // Indicate unmuted
        digitalWrite(led_mute_blue, LOW); // Indicate muted
      }
    }
    else {
      while(!Serial.available()); // wait for serial
      Serial.println("Down"); // send volume down signal
      //print_command("Down");
      if (digitalRead(led_mute_blue) == HIGH) {
        mute++;
        digitalWrite(led_mute_green, HIGH); // Indicate unmuted
        digitalWrite(led_mute_blue, LOW); // Indicate muted
      }
    }
    last_vol = map_pot_value; // save last volume val manipulation
  }
  delay(10);
  int16_t joyX_value = analogRead(X_axis); // read values from potentiometru
  byte map_joyX_value = map(joyX_value, 0, 1023, 1, 4); // map the values
  
  if (map_joyX_value != last_X_pos){ // check if value changed
    if (map_joyX_value == 1){ // verify joystick going left 
      while(!Serial.available()); // wait for serial
      Serial.println("prev"); // send previous song signal
      //print_command("Prev");
      blink_led();
    }
    if (map_joyX_value == 3){ // verify joystcik going right
      while(!Serial.available()); // wait for serial
      Serial.println("next"); // send next song signal
      //print_command("Next");
      blink_led();
    }
    last_X_pos = map_joyX_value;
  }

  int16_t joyY_value = analogRead(Y_axis); // read values from potentiometru
  byte map_joyY_value = map(joyY_value, 0, 1023, 1, 4); // map the values
  
  if (map_joyY_value != last_Y_pos){ // check if value changed
    if (map_joyY_value == 1){ // verify joystick going left 
      while(!Serial.available()); // wait for serial
      Serial.println("stop"); // send previous song signal
      //print_command("Stop");
      blink_led();
    }
    if (map_joyY_value == 3){ // verify joystcik going right
      while(!Serial.available()); // wait for serial
      Serial.println("win"); // send next song signal
      //print_command("Win");
      blink_led();
    }
    last_Y_pos = map_joyY_value; 
  }
 delay(100);
}
