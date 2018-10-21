#include "src/TimerOne/TimerOne.h"
#include "characters.h"

#define ROW_ENABLE HIGH
#define ROW_DISABLE LOW
#define COLUMN_ON LOW
#define COLUMN_OFF HIGH

#define TEXT "Schmo-Fu"
#define TEXT_SHIFT_SPEED_MS 50

//                     Arduino   AVR    LED Matrix 
const uint8_t LED_X1 = 2;     // PD2    Pin 13
const uint8_t LED_X2 = 3;     // PD3    Pin  3
const uint8_t LED_X3 = 4;     // PD4    Pin  4
const uint8_t LED_X4 = 5;     // PD5    Pin 10
const uint8_t LED_X5 = 6;     // PD6    Pin  6
const uint8_t LED_X6 = 7;     // PD7    Pin 11
const uint8_t LED_X7 = 8;     // PB0    Pin 15
const uint8_t LED_X8 = 9;     // PB1    Pin 16

const uint8_t LED_Y8 = 14;    // PC0    Pin  9
const uint8_t LED_Y7 = 15;    // PC1    Pin 14
const uint8_t LED_Y6 = 16;    // PC2    Pin  8
const uint8_t LED_Y5 = 17;    // PC3    Pin 12
const uint8_t LED_Y4 = 18;    // PC4    Pin  1  
const uint8_t LED_Y3 = 19;    // PC5    Pin  7
const uint8_t LED_Y2 = 12;    // PB4    Pin  2
const uint8_t LED_Y1 = 13;    // PB5    Pin  5

const uint8_t LED_X[8] = {LED_X1, LED_X2, LED_X3, LED_X4, LED_X5, LED_X6, LED_X7, LED_X8};
const uint8_t LED_Y[8] = {LED_Y1, LED_Y2, LED_Y3, LED_Y4, LED_Y5, LED_Y6, LED_Y7, LED_Y8};

const int button_1_Pin = 10;  // PB2        push button SW1
const int button_2_Pin = 11;  // PB3 (MOSI) push button SW2

typedef enum{
  BUTTON_PRESSED,
  BUTTON_HELD,
  BUTTON_RELEASED,
  BUTTON_INACTIVE,
}button_state;

volatile button_state button_1_state = BUTTON_INACTIVE;
volatile button_state button_2_state = BUTTON_INACTIVE;

uint8_t matrix[8] {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

// Add here a define for your output mode and increase OUTPUT_MODE_MAX accordingly
#define FILL_MATRIX_SLOW        0
#define FILL_MATRIX_FAST        1
#define FILL_MATRIX_SPIRAL      2
#define FILL_MATRIX_RANDOM      3
#define OUTPUT_TEXT             4

#define OUTPUT_MODE_MAX         5

volatile uint8_t reqModeSwitch = 0;
volatile uint16_t countdown = 0;

uint8_t x = 0;
uint8_t y = 0;

#define TIMER_PERIOD_IN_MS   2

#define TIME_3_S              (3000/TIMER_PERIOD_IN_MS)
#define TIME_200_MS           (200/TIMER_PERIOD_IN_MS)
#define TIME_50_MS            (50/TIMER_PERIOD_IN_MS)
#define TIME_20_MS            (20/TIMER_PERIOD_IN_MS)
#define TIME_10_MS            (10/TIMER_PERIOD_IN_MS)

void setup() {
  pinMode(LED_X1, OUTPUT);
  pinMode(LED_X2, OUTPUT);
  pinMode(LED_X3, OUTPUT);
  pinMode(LED_X4, OUTPUT);
  pinMode(LED_X5, OUTPUT);
  pinMode(LED_X6, OUTPUT);
  pinMode(LED_X7, OUTPUT);
  pinMode(LED_X8, OUTPUT);

  pinMode(LED_Y1, OUTPUT);
  pinMode(LED_Y2, OUTPUT);
  pinMode(LED_Y3, OUTPUT);
  pinMode(LED_Y4, OUTPUT);
  pinMode(LED_Y5, OUTPUT);
  pinMode(LED_Y6, OUTPUT);
  pinMode(LED_Y7, OUTPUT);
  pinMode(LED_Y8, OUTPUT);

  digitalWrite(LED_X1, ROW_DISABLE);
  digitalWrite(LED_X2, ROW_DISABLE);
  digitalWrite(LED_X3, ROW_DISABLE);
  digitalWrite(LED_X4, ROW_DISABLE);
  digitalWrite(LED_X5, ROW_DISABLE);
  digitalWrite(LED_X6, ROW_DISABLE);
  digitalWrite(LED_X7, ROW_DISABLE);
  digitalWrite(LED_X8, ROW_DISABLE);

  digitalWrite(LED_Y1, COLUMN_OFF);
  digitalWrite(LED_Y2, COLUMN_OFF);
  digitalWrite(LED_Y3, COLUMN_OFF);
  digitalWrite(LED_Y4, COLUMN_OFF);
  digitalWrite(LED_Y5, COLUMN_OFF);
  digitalWrite(LED_Y6, COLUMN_OFF);
  digitalWrite(LED_Y7, COLUMN_OFF);
  digitalWrite(LED_Y8, COLUMN_OFF);

  pinMode(button_1_Pin, INPUT_PULLUP);
  pinMode(button_2_Pin, INPUT_PULLUP);

  Timer1.initialize();
  // The display function gets called every 2 ms
  Timer1.attachInterrupt(display, TIMER_PERIOD_IN_MS*1000);
}


void loop() {
  //outputShiftString("123abc");
  //output_fill_matrix_slow();
  static uint8_t outputMode = 0;

  if (reqModeSwitch){
    // both buttons were pressed for 3 seconds
    reqModeSwitch = 0;

    // create a defined state and turn off all leds
    clear_matrix_immediatly();

    // switch output mode
    outputMode++;
    if (outputMode == OUTPUT_MODE_MAX) outputMode = 0;

    // Do initializations for a new output mode here if necessary
    switch (outputMode){
      case FILL_MATRIX_SLOW:
        break;
      case FILL_MATRIX_FAST:
        break;
      default:
        break;
    }
  }

  // Call your output mode in this switch
  switch (outputMode){
    case FILL_MATRIX_SLOW:
      snake();
      //outputShiftString(TEXT);
      //outputString(TEXT);
      //displayCharacterOffset(LETTERS[0],3,0);
      //output_fill_matrix_slow();
      //output_fill_matrix_random();
      break;
    case FILL_MATRIX_FAST:
      output_fill_matrix_fast();
      break;
    case FILL_MATRIX_SPIRAL:
      output_fill_matrix_spiral();
      break;
    case FILL_MATRIX_RANDOM:
      output_fill_matrix_random();
      break;
    case OUTPUT_TEXT:
      outputShiftString(TEXT);
      break;
    default:
      break;
  }
}

// output mode functions
void output_fill_matrix_slow(){
  if (countdown == 0){
    // set pixel
    //clear_matrix_immediatly_without_reset();
    matrixSetPixel(x,y, true);
    x++;
    if (x > 7){
      x = 0;
      y++;
      if (y > 7){
        clear_matrix_immediatly();
      }
    }
    countdown = TIME_200_MS;
  }
}

void output_fill_matrix_fast(){
  if (countdown == 0){
    // set pixel
    matrixSetPixel(x,y, true);
    x++;
    if (x > 7){
      x = 0;
      y++;
      if (y > 7){
        clear_matrix_immediatly();
      }
    }
    countdown = TIME_20_MS;
  }
}

void output_fill_matrix_spiral(){
  static uint8_t round = 0;
  static uint8_t mode = 1;  // 1 = fill, 0 = clear
  static uint8_t direction = 0;

  if (countdown == 0){
    if (mode == 1) {
      // fill
      if (x == round && y == round + 1) {
        direction = 0;
        round++;
      } else if (x == 7 - round && y == round) {
        direction = 1;
      } else if (x == 7 -round && y == 7 - round) {
        direction = 2;
      } else if (x == round && y == 7 - round) {
        direction = 3;
      }

    } else {
      // clear
      if (x == round && y == round) {
        direction = 0;
      } else if (x == 7 - round && y == round) {
        direction = 1;
      } else if (x == 7 - round && y == 7 - round) {
        direction = 2;
      } else if (x == round - 1 && y == 7 - round) {
        direction = 3;
        round--;
      }
    }

    switch(direction) {
      case 0: x++; break;  // to the right
      case 1: y++; break;  // upwards
      case 2: x--; break;  // to the left
      case 3: y--; break;  // downwards
    }

    matrixSetPixel(x, y, mode);

    if (mode == 1 && x == 3 && y == 4) {  // last pixel activ - switch to turn off
      mode = 0;
      direction = 0;
      x--;
      y--;
    }

    if (mode == 0 && x == 0 && y == 7) {  // last pixel clear - refill again
      mode = 1;
      direction = 0;
      x = -1;
      y = 0;
    }
    countdown = TIME_20_MS;
  }
}

unsigned int rng() {
  static unsigned int r = 0;
  r += micros(); // seeded with changing number
  r ^= r << 2;
  r ^= r >> 7;
  r ^= r << 7;
  return (r);
}

void output_fill_matrix_random() {
    static uint8_t i = 0;
    x = rng() / 256;

    matrix[i] = x;
    i++;
    if (i >7){
      i=0;
    }
}

// Shows sing characters of passed string one after the other
void outputString(char * text){
  char * t;
  int offsetASCII = 0;
  int xOffset=0;
  int yOffset=0;
  
  for (t = text; *t != '\0'; t++){
    displayCharacter(ASCII[(int)*t]);
    delay(500);
  }
}

//Shifts string through matrix
void outputShiftString(char * text){
  char *t;
  static int i = 0;
  static int xOffset = -1;
  static int yOffset = 0;
  static bool fistrun = true;

  /* iterate over text characters
   * loop takes car of two characters a the same time
  */

  if (countdown == 0){
    //remember last drawn char
    t = text;
    t = t + i;

    // if first character start from left side
    if (fistrun){
      xOffset=7;
      fistrun=false;
    }
    // draw char
    displayCharacterOffset(ASCII[(int)*t],xOffset,yOffset);
    
    // *********second charcter part***********
    if(xOffset < -1 && *(t+1) != '\0'){
      displayCharacterOffset(ASCII[(int)*(t+1)],xOffset+7,yOffset);
    }
    
    countdown = TEXT_SHIFT_SPEED_MS;
    
    //reset if end of string
    if(*t == '\0'){
      i = 0;     
      //xOffset=0;   
      fistrun = true;  
    }
    // if charachter shift completed
    if(xOffset <= -8){
      xOffset=-1;
      i++;
    }
    //set X offset for next cycle
    xOffset--;
  }
}


// crazy snake !!1ELF!
// mmmixed up and down by schmo-fu
  
struct segment {
    int8_t pos;
    segment *next;
  };

void snake(){
  static uint8_t speed = 100;
  static bool debounced = true;
  #define TOP    0
  #define RIGHT  1
  #define BOTTOM 2
  #define LEFT   3
  static uint8_t snake_dir = RIGHT;
  static uint8_t snake_len = 1;
  static uint8_t snake_age = 0;
  static uint8_t mouse_pos = 10;
  static segment *tail = new segment {.pos = 32, .next = NULL};

  // check buttons
  if (debounced) {
    if (button_2_state == BUTTON_HELD) { // turn to the left
      if (snake_dir == TOP) { snake_dir = LEFT; }
      else { snake_dir--; } 
      debounced = false;
    }
    if (button_1_state == BUTTON_HELD) {
      if (snake_dir == LEFT) { snake_dir = TOP; }
      else { snake_dir++; } 
      debounced = false;
    }
  } else if ((button_2_state == BUTTON_INACTIVE) && (button_1_state == BUTTON_INACTIVE)) {
    debounced = true;
  }
  
  if (countdown == 0) {  // showtime!
    // snake ist getting older
    snake_age += 1;
    
    // move the snake
    segment *snake = new segment {.pos = tail->pos, .next = tail};
    switch (snake_dir) {
      case TOP: snake->pos -= 8; if (snake->pos < 0) {snake->pos += 64;} break;
      case RIGHT: snake->pos += 1; if (snake->pos % 8 == 0) {snake->pos -= 8;} break;
      case BOTTOM: snake->pos += 8; if (snake->pos > 63) {snake->pos -= 64;} break;
      case LEFT: snake->pos -= 1; if ((snake->pos % 8 == 7) || (snake->pos == -1)) {snake->pos += 8;} break;
    }
  
    // check for mouse contact
    if (snake->pos == mouse_pos) {
      snake_len += 1;
      if(snake_age >= (snake_len * 2)) { // eating makes the snake younger again
        snake_age -= snake_len * 2;
      } else {
        snake_age = 0;
      }
      // create new mouse at some later time
      mouse_pos = 64 + 3; // wait for three turns
    } else {
      // snake_len stays the same. As we added a new head we must remove the last tail segment
      segment *p = snake;
      while (p->next->next != NULL) { p = p->next; }
      delete p->next;
      p->next = NULL; 
    }
    
    // no collision detection, but old snakes eat themselves
    if (snake_age >= 200) {
      if (snake_len >= 2) {  
        segment *p = snake;
        while (p->next->next != NULL) { p = p->next; }
        delete p->next;
        p->next = NULL; 
        snake_len -= 1;
        snake_age -= 30;
      } else {
        snake_age = 0; // zooom! starved and reborn.
      }
    }
    
    // reset last frame
    clear_matrix_immediatly_without_reset();
    // update and draw mouse
    if (mouse_pos < 64) {
      matrixSetPixel(mouse_pos%8, mouse_pos/8, true);
    } else if (mouse_pos == 64) {
      // new mouse position
      mouse_pos = (snake->pos + 28)%64;
    } else {
      mouse_pos -= 1; // wait another turn
    }
    // draw snake
    tail = snake;
    while (snake != NULL) {
      matrixSetPixel(snake->pos%8, snake->pos/8, true);
      snake = snake->next;
    }
    countdown = speed - (snake_len-1) * 5; // more speed with longer snakes, it's crazy!
  }
}

