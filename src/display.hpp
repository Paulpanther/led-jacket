
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define SCREEN_WIDTH  128  // adjust to your display’s width
#define SCREEN_HEIGHT 160  // adjust to your display’s height
#define NUM_COLUMNS   10   // number of falling "streams"
#define CHAR_WIDTH    20    // approximate width of characters in your font
#define CHAR_HEIGHT   18    // approximate height of characters in your font
#define INNER_W 100
#define INNER_H 30

int innerPadLeft = SCREEN_WIDTH/2 - INNER_W/2; 
int innerPadTop = SCREEN_HEIGHT/2 - INNER_H/2; 

// CS, RS, RES
Adafruit_ST7735 tft = Adafruit_ST7735(5, 0, 4);

struct MatrixColumn {
  int x;
  float y;
  float oldY;    // previous vertical offset
  float speed;
};

MatrixColumn columns[NUM_COLUMNS];
#define TEXT_COUNT 3
const char* texts[TEXT_COUNT] = {
  "hello",
  " uwu ",
  "cyber",
};

int currentText;
unsigned long lastTextUpdate = 0;

void drawInner() {
  if (currentText < TEXT_COUNT) {
  tft.fillRect(innerPadLeft, innerPadTop, INNER_W, INNER_H, ST77XX_BLACK);
  tft.drawRect(innerPadLeft + 4, innerPadTop + 4, INNER_W - 8, INNER_H - 8, ST77XX_WHITE);
  tft.setCursor(innerPadLeft + 22, innerPadTop + 7);
  tft.setTextColor(ST77XX_RED);
  tft.setTextWrap(false);
  tft.setTextSize(2);
  tft.print(texts[currentText]);
  }
}

// -------------------------------------------------------
// Helper Function: Draw a Column
// -------------------------------------------------------
// Draws a vertical stream of random digits starting from
// (column.x, column.y) down the screen. Digits beyond the
// screen bottom won't be visible, but we keep drawing them
// for the “falling” effect.
void drawColumn(const MatrixColumn &col, float startY, uint16_t color)
{
  // We'll draw random digits from startY downward,
  // about 20 lines or so, or until we pass the screen.
  // For a "rain" effect, you can experiment with 
  // how many characters to show.

  // Decide how many characters to print
  int visibleCount = (SCREEN_HEIGHT - (int)startY) / CHAR_HEIGHT + 2;
  if (visibleCount < 1) return;  // no visible portion

  for (int i = 0; i < visibleCount; i++) {
    int digitY = (int)startY + i * CHAR_HEIGHT;
    // Skip if above the top
    if (digitY < 0) continue;
    // Stop if below the bottom
    if (digitY > SCREEN_HEIGHT) break;

    // Generate a random digit [0..9]. 
    // You could also do ASCII letters, symbols, etc.
    char text[2];
    text[0] = '0' + (rand() % 10);
    text[1] = '\0';

    // Position and draw
    tft.setCursor(col.x, digitY);
    tft.setTextColor(color);
    tft.setTextWrap(false);
    tft.setTextSize(2);
    tft.print(text);

    // bool intersects = 
    // (col.x + CHAR_WIDTH  > innerPadLeft)               &&  // digit’s right  > rect’s left
    // (col.x              < innerPadLeft + INNER_W)      &&  // digit’s left   < rect’s right
    // (digitY + CHAR_HEIGHT > innerPadTop)               &&  // digit’s bottom > rect’s top
    // (digitY             < innerPadTop + INNER_H);         // digit’s top    < rect’s bottom


    // if (intersects) {
    //   drawInner();
    // }
  }
}

void initMatrixColumns() {
  srand((unsigned int)millis());  // seed random with something
  int colSpacing = SCREEN_WIDTH / NUM_COLUMNS;
  for (int i = 0; i < NUM_COLUMNS; i++) {
    columns[i].x = i * colSpacing + (colSpacing / 4); // place columns evenly
    columns[i].y = -(rand() % SCREEN_HEIGHT);         // start above the screen
    columns[i].speed = 1.0f + (rand() % 5) * 0.3f;     // random speeds
  }
}

void setupDisplay() {
  tft.initR(INITR_BLACKTAB);
  initMatrixColumns();
}

void updateColumn(MatrixColumn &col)
{
  // 1. Erase old column by drawing it in BLACK
  drawColumn(col, col.oldY, ST77XX_BLACK);

  // 2. Update position
  col.y += col.speed;

  // 3. Draw new column in GREEN
  drawColumn(col, col.y, ST77XX_GREEN);

  // 4. Store current position as oldY for next loop
  col.oldY = col.y;

  // 5. If it's off-screen, reset it slightly above top
  if (col.y > SCREEN_HEIGHT) {
    tft.fillRect(col.x, 0, CHAR_WIDTH, SCREEN_HEIGHT, ST77XX_BLACK);
    // Start above top with some random offset
    col.y = -(CHAR_HEIGHT * (10 + rand() % 20));
    col.oldY = col.y;
    // Possibly randomize speed again
    col.speed = 1.0f + (rand() % 5) * 0.3f;
  }
}

void updateDisplay() {
  // tft.fillScreen(ST77XX_BLACK); // ST77XX_BLACK or your library's black

//   unsigned long currentTime = millis();

  // Check if 100ms have passed since the last update
//   if (currentTime - lastTextUpdate >= 5000) {
//     lastTextUpdate = currentTime;  // Reset the timer
//     currentText = rand() % (TEXT_COUNT * 3);
//   }

  // 2. Update and draw each column
  for (int i = 0; i < NUM_COLUMNS; i++) {
    updateColumn(columns[i]);
//   drawInner();
  }

}

unsigned long lastDisplayUpdate = 0;

void loopDisplay() {
  // Check if 100ms have passed since the last update
  if (millis() - lastDisplayUpdate >= 100) {
    lastDisplayUpdate = millis();  // Reset the timer
    updateDisplay();                  // Call your display update function
  }
}