#include <Adafruit_GFX.h>
#include <Adafruit_ST7796S_kbv.h>

// Updated SPI pin assignments
#define TFT_CS   4
#define TFT_DC   6
#define TFT_RST  7  // Use -1 if tied to ESP32 reset

Adafruit_ST7796S_kbv tft(TFT_CS, TFT_DC, TFT_RST);
int speed;

void displayMPH(int num);
void displayBatteryLife(int num);

// Circle parameters
int screenWidth, screenHeight;
int centerX, centerY;
int radius = 80;

void setup() {
  // Initialize display
  tft.begin();
  tft.setRotation(3);  // Adjust 0–3 for screen orientation
  tft.fillScreen(0xFFFF);  // White background

  speed = 20;

  screenWidth  = tft.width();     // Typically 320px
  screenHeight = tft.height();    // Typically 480px
  centerX = screenWidth / 2;
  centerY = screenHeight / 2;

  // Draw 3-pixel-thick horizontal and vertical black lines (cross)
  int y = centerY - 1;
  int x = centerX - 1;
  tft.fillRect(0, y, screenWidth, 3, 0x0000);
  tft.fillRect(x, 0, 3, screenHeight, 0x0000);

  // Draw center circle
  tft.fillCircle(centerX, centerY, radius, 0xFFFF);   // White fill
  tft.drawCircle(centerX, centerY, radius, 0x0000);   // Black border
  displayMPH(speed);
  displayBatteryLife(69);
}

void loop() {
  /*
  // Clear center circle before updating speed
  tft.fillCircle(centerX, centerY, radius, 0xFFFF);   // Clear with white
  tft.drawCircle(centerX, centerY, radius, 0x0000);   // Redraw border

  displayMPH(speed);

  speed++;
  if (speed > 29) {
    speed = 20;
  }
  delay(500);
  */
 
}

void displayMPH(int num) {
  char buffer[3];
  sprintf(buffer, "%d", num); // Convert int to string

  // Display MPH number centered in the circle
  tft.setCursor(centerX - 44, centerY - 40);  // Adjust for centering visually
  tft.setTextColor(0x0000);
  tft.setTextSize(8);
  tft.print(buffer);

  // Display "MPH" text below the number
  tft.setCursor(centerX - 27, centerY + 30);  // Adjust for centering
  tft.setTextColor(0x0000);
  tft.setTextSize(3);
  tft.print("MPH");
}

void displayBatteryLife(int num) {
  char buffer[6];
  sprintf(buffer, "%d%%", num); // Convert int to string

  // Display Batt Percent
  tft.setCursor(60,30);  // Adjust for centering visually
  tft.setTextColor(0x0000);
  tft.setTextSize(5);
  tft.print(buffer);

  // Display "MPH" text below the number
  tft.setCursor(38,70);  // Adjust for centering
  tft.setTextColor(0x0000);
  tft.setTextSize(3);
  tft.print("Battery");
}
