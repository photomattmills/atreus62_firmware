//#include "hid-keycode.h"
//#include "Keyboard.h"
#include <SoftwareSerial.h>

typedef struct
{
  uint8_t modifier;   /**< Keyboard modifier keys  */
  uint8_t reserved;   /**< Reserved for OEM use, always set to 0. */
  uint8_t keycode[6]; /**< Key codes of the currently pressed keys. */
} hid_keyboard_report_t;

hid_keyboard_report_t keyReport = { 0, 0, { 0 } };

// Used to re-initalize the report when keys are released/change
hid_keyboard_report_t blankReport = { 0, 0, { 0 } };

int row_0 = 22;
int row_1 = 23;
int row_2 = 5;
int row_3 = 6;
int row_4 = 9;
int row_5 = 10;

int col_0 = 15;
int col_1 = 16;
int col_2 = 17;
int col_3 = 25;  
int col_4 = 24;
int col_5 = 1;
int col_6 = 0;

int macro = 999999999;
bool macro_send = false;
bool state;

int keyreport_index = 0;
// The 'keys' in this keymap have a modifier and a keycode; I tried doing this as a struct but the compiler got mad. :shrug:

const int number_of_rows = 6;
const int number_of_columns = 13;
//
//int layers[number_of_rows][number_of_columns] = {
//    {
//      K_ESCAPE,          K_GRAVE, K_BRACKET_LEFT, K_BRACKET_RIGHT, K_SHIFT_LEFT, K_GUI_LEFT, K_RETURN, K_SPACE, K_ARROW_LEFT, K_ARROW_UP, K_ARROW_RIGHT, K_ARROW_DOWN, K_ALT_LEFT
//    },{
//      K_SHIFT_LEFT,      K_Z,     K_X,            K_C,             K_V,          K_B, K_BACKSPACE,     K_N,      K_M,     K_COMMA,      K_PERIOD,   K_SLASH,     K_SHIFT_LEFT
//    },{
//      K_CONTROL_LEFT,  K_A,     K_S,            K_D,             K_F,          K_G, K_NONE,          K_H,      K_J,     K_K,          K_L,        K_SEMICOLON, K_APOSTROPHE
//    },{
//      K_TAB,             K_Q,     K_W,            K_E,             K_R,          K_T, K_NONE,          K_Y,      K_U,     K_I,          K_O,        K_P,         K_BACKSLASH
//    },{
//      K_EQUAL,           K_1,     K_2,            K_3,             K_4,          K_5, K_NONE,          K_6,      K_7,     K_8,          K_9,        K_0,         K_MINUS
//    },
//};

unsigned long debounce_grid[number_of_rows][number_of_columns][3];
unsigned int debounce_wait = 20;

// I know I could construct these as literal ints, but I like having them named for purposes of easy debugging
int columns[] = {col_0, col_1, col_2, col_3, col_4, col_5, col_6};

int rows[] = {row_0, row_1, row_2, row_3, row_4, row_5};

bool key_state;

void setup() {
   Serial.begin(57600);
  delay(500);
  Serial.println("Hello");
}

void loop() {
  get_keys();
//  send_keys();
}

void get_keys() {
  keyreport_index = 0;
  for (size_t column = 0; column < 7; column++) { // column loop
    for (size_t row = 0; row < 6; row++) {
      state = old_key_pressed(row, column);// || key_pressed_inverse(row, column);
      if (state) {
        Serial.print("column: ");
        Serial.print(column); 
        Serial.print(", row: ");
        Serial.println(row);
      }
      pinMode(rows[row], INPUT); //
      pinMode(columns[column], INPUT); // we turn off the pullup to save power between cycles
    }
  }
}
//
//void send_keys(){
//  set_keys();
//  set_modifiers();
//  Keyboard.send_now();
//  keyReport = blankReport;
//}
//
//void set_keys(){
//  Keyboard.set_key1(keyReport.keycode[0]);
//  Keyboard.set_key2(keyReport.keycode[1]);
//  Keyboard.set_key3(keyReport.keycode[2]);
//  Keyboard.set_key4(keyReport.keycode[3]);
//  Keyboard.set_key5(keyReport.keycode[4]);
//  Keyboard.set_key6(keyReport.keycode[5]);
//}
//
//void set_modifiers(){
//  Keyboard.set_modifier(keyReport.modifier);
//}

bool old_key_pressed(uint8_t row, uint8_t column){
  pinMode(rows[row], INPUT_PULLUP);
  pinMode(columns[column], OUTPUT);
  digitalWrite(columns[column], LOW);
  key_state = digitalRead(rows[row]);
  return !key_state;
}
//
//bool key_pressed(uint8_t row, uint8_t column){
//  // set up the row/column
//  pinMode(rows[row], OUTPUT);
//  pinMode(columns[column], INPUT_PULLUP);
//  digitalWrite(rows[row], LOW);
//  // actually read the key state
//  key_state = !digitalRead(columns[column]);
//
//  if (key_state != debounce_grid[row][column][0]) {
//    // reset the debouncing timer since things have changed
//    debounce_grid[row][column][1] = millis();
//  }
//
//  bool return_val;
//  if ((millis() - debounce_grid[row][column][1]) > debounce_wait){
//    return_val = key_state; // if the key changed and stayed steady for the debounce time, go ahead and return the current state
//    debounce_grid[row][column][2] = key_state; // set the stable value
//  }else{
//    return_val = debounce_grid[row][column][2]; // return the 'stable' value
//  }
//  debounce_grid[row][column][0] = key_state; // store the current state for comparison on the next loop
//  return return_val;
//}

// modifiers are added with boolean logic here
int modifier(int code){
  return keyReport.modifier | code;
}
