#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_ST7796S_kbv.h"
#include "alligator2.c"  // Image file exported from GIMP, stored as RGB888 data

// === DISPLAY PIN CONFIGURATION ===
// These are your hardware connections to the ST7796 display
#define TFT_CS   4   // Chip Select pin
#define TFT_DC   6   // Data/Command pin
#define TFT_RST  7   // Reset pin

// === INITIALIZE DISPLAY OBJECT ===
// This object gives you access to display methods like begin(), fillScreen(), writeColor(), etc.
Adafruit_ST7796S_kbv tft = Adafruit_ST7796S_kbv(TFT_CS, TFT_DC, TFT_RST);

// === FUNCTION: convertRGB888to565 ===
// Converts a 24-bit RGB color (3 bytes: R, G, B) into 16-bit RGB565 format
// This is required because the ST7796 display expects 16-bit pixel data.
uint16_t convertRGB888to565(uint8_t r, uint8_t g, uint8_t b) {
    // RGB565 format:
    // - Red:   top 5 bits   (r >> 3)
    // - Green: middle 6 bits (g >> 2)
    // - Blue:  bottom 5 bits (b >> 3)
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// === FUNCTION: drawBitmap ===
// Sends pixel data from the alligator2.c image to the ST7796 display
// It draws the image row-by-row to ensure clean, aligned rendering
void drawBitmap() {
    int w = gimp_image.width;                    // Image width (from the struct)
    int h = gimp_image.height;                   // Image height
    const unsigned char* data = gimp_image.pixel_data;  // Raw RGB888 image data

    // Compute offsets to center the image on the screen
    int x_offset = (tft.width()  - w) / 2;
    int y_offset = (tft.height() - h) / 2;

    // Start SPI write transaction (faster than calling drawPixel one at a time)
    tft.startWrite();

    // Loop through each row (Y-axis)
    for (int y = 0; y < h; y++) {
        // Set the active drawing window to just this row
        tft.setAddrWindow(x_offset, y_offset + y, x_offset + w - 1, y_offset + y);

        // Loop through each column (X-axis)
        for (int x = 0; x < w; x++) {
            // Calculate the index of the pixel's R value in the flat byte array
            int idx = (y * w + x) * gimp_image.bytes_per_pixel;

            // Extract R, G, B values from the RGB888 data
            uint8_t r = data[idx];
            uint8_t g = data[idx + 1];
            uint8_t b = data[idx + 2];

            // Convert RGB888 to RGB565 format
            uint16_t color = convertRGB888to565(r, g, b);

            // Send the 16-bit color to the display
            tft.writeColor(color, 1);  // '1' means one pixel
        }
    }

    // End the SPI write transaction
    tft.endWrite();
}

// === SETUP ===
// This function runs once at startup
void setup() {
    Serial.begin(115200);       // Initialize serial for debugging (optional)

    tft.begin();                // Initialize the ST7796 display

    tft.setRotation(3);         // Rotate the display (0–3); 1 = landscape
    tft.fillScreen(0x0000);     // Clear the screen to black

    drawBitmap();               // Call your custom function to draw the image
}

// === LOOP ===
// This runs repeatedly, but we don’t need it here
void loop() {
    // You could animate, redraw, or add input handling here if needed
}
