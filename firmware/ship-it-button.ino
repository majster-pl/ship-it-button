 /*
 * Ship It! Button
 *
 * Hardware: ESP32-S3, 8x WS2812B, USB HID
 * Designed and 3D modeled in Tinkercad
 * STL files and build instructions: see README.md
 * 
 * Libraries used:
 * - FastLED
 * - USBHIDKeyboard (ESP32-S3 compatible)
 * 
 * Developed with help from ChatGPT (OpenAI)
 * 
 * Copyright (C) 2025 Szymon Waliczek <szymon@waliczek.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include <FastLED.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

// === LED Config ===
#define LED_PIN        4
#define NUM_LEDS       8
#define BRIGHTNESS     100
#define LED_TYPE       WS2812B
#define COLOR_ORDER    GRB

CRGB leds[NUM_LEDS];

// === Button Config ===
#define BUTTON_PIN     1

// === USB Keyboard ===
USBHIDKeyboard Keyboard;

// === State ===
bool animationTriggered = false;
bool lastButtonState = HIGH;
bool currentButtonState = LOW;
bool initialAnimationPlayed = false;

void setup() {
  randomSeed(analogRead(A0));

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  turnOffLeds();

  delay(2000); // Initial delay before animations

  Keyboard.begin();
  USB.begin();
}

void loop() {
  // Check for button press
  currentButtonState = digitalRead(BUTTON_PIN);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    animationTriggered = true;
  }
  lastButtonState = currentButtonState;

  if (animationTriggered) {
    animationTriggered = false;
    animationOBC();
    turnOffLeds();
  } else {
    int standbySeconds = getRandom(60, 60 * 20);
    if (!initialAnimationPlayed) {
      breathingAnimation(getRandom(1, 2));
      standByInSeconds(standbySeconds);
    } else {
      standByInSeconds(standbySeconds);
      breathingAnimation(getRandom(1, 2));
    }
    initialAnimationPlayed = true;
  }
}

// === Helper Functions ===

bool checkIfButtonPressed() {
  currentButtonState = digitalRead(BUTTON_PIN);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    animationTriggered = true;
    turnOffLeds();
    return true;
  }
  return false;
}

void standByInSeconds(int seconds) {
  for (int i = 0; i < seconds * 1000; i += 100) {
    if (checkIfButtonPressed()) break;
    delay(100);
  }
}

void turnOffLeds() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

int getRandom(int min, int max) {
  return rand() % (max - min + 1) + min;
}

float getRandomFloat(float min, float max) {
  return min + ((float)rand() / RAND_MAX) * (max - min);
}

void updateChase(uint8_t &pos, unsigned long &lastUpdate, float delayTime) {
  if (millis() - lastUpdate >= delayTime) {
    lastUpdate = millis();
    fadeToBlackBy(leds, NUM_LEDS, 100);
    leds[pos] = CRGB::Purple;
    FastLED.show();
    pos = (pos + 1) % NUM_LEDS;
  }
}

void shipItCommand() {
  Keyboard.print(":ship: ship it! :rocket:");
  delay(200);
  Keyboard.write(KEY_RETURN); 
  delay(10);
  Keyboard.releaseAll();
}

void animationOBC() {
  // === Timing ===
  const unsigned long fadeInTime   = 300;
  const unsigned long accelTime    = 700;
  const unsigned long cruiseTime   = 1500;
  const unsigned long decelTime    = 2000;
  const unsigned long fadeOutTime  = 2000;
  const float startDelay           = 200;
  const float maxSpeedDelay        = 15;

  uint8_t pos = random(0, NUM_LEDS);
  unsigned long lastUpdate = 0;
  float currentDelay = startDelay;

  // --- Phase 0: Fade In ---
  unsigned long phaseStart = millis();
  while (millis() - phaseStart < fadeInTime) {
    float t = (float)(millis() - phaseStart) / fadeInTime;
    FastLED.setBrightness((uint8_t)(t * BRIGHTNESS));
    updateChase(pos, lastUpdate, startDelay);
  }

  // --- Phase 1: Acceleration ---
  phaseStart = millis();
  while (millis() - phaseStart < accelTime) {
    float t = (float)(millis() - phaseStart) / accelTime;
    currentDelay = startDelay - ((1 - cos(t * PI)) / 2.0f) * (startDelay - maxSpeedDelay);
    updateChase(pos, lastUpdate, currentDelay);
  }

  // --- Ship It! ---
  shipItCommand();

  // --- Phase 2: Cruise ---
  phaseStart = millis();
  while (millis() - phaseStart < cruiseTime) {
    updateChase(pos, lastUpdate, maxSpeedDelay);
  }

  // --- Phase 3: Deceleration ---
  phaseStart = millis();
  while (millis() - phaseStart < decelTime) {
    float t = (float)(millis() - phaseStart) / decelTime;
    currentDelay = maxSpeedDelay + ((1 - cos(t * PI)) / 2.0f) * (startDelay - maxSpeedDelay);
    updateChase(pos, lastUpdate, currentDelay);
  }

  delay(800); // Pause before fade out

  // --- Phase 4: Fade Out ---
  phaseStart = millis();
  while (millis() - phaseStart < fadeOutTime) {
    fadeToBlackBy(leds, NUM_LEDS, 20);
    FastLED.show();
    delay(25);
  }
}

void fadeOutLeds(uint8_t startBrightness, unsigned long durationMillis) {
  const unsigned long stepDelay = 5;
  unsigned long steps = durationMillis / stepDelay;
  if (steps == 0) steps = 1;

  for (unsigned long i = 0; i < steps; i++) {
    uint8_t brightness = map(steps - i, 0, steps, 0, startBrightness);
    FastLED.setBrightness(brightness);
    FastLED.show();
    if (checkIfButtonPressed()) break;
    delay(stepDelay);
  }

  FastLED.setBrightness(0);
  turnOffLeds();
}

void breathingAnimation(uint8_t breathCount) {
  const unsigned long breathDuration = 2500;
  const uint8_t breathMaxBrightness = 40;
  const float minBrightnessFactor = 0.15f;
  const float mainBrightnessFactor = getRandomFloat(0.3, 1);
  const unsigned long fadeInTime = 300;

  // Fade in to min brightness
  unsigned long fadeStart = millis();
  uint8_t targetMinBrightness = minBrightnessFactor * breathMaxBrightness;

  while (millis() - fadeStart < fadeInTime) {
    uint8_t brightness = map(millis() - fadeStart, 0, fadeInTime, 0, targetMinBrightness);
    FastLED.setBrightness(brightness * mainBrightnessFactor);
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    if (checkIfButtonPressed()) return;
    delay(20);
  }

  // Main breathing loop
  unsigned long totalDuration = breathCount * breathDuration;
  unsigned long startTime = millis();
  uint8_t currentBrightness = 0;

  while (millis() - startTime < totalDuration) {
    float t = (float)((millis() - startTime) % breathDuration) / breathDuration;
    float brightnessFactor = minBrightnessFactor + ((1 - cos(t * 2 * PI)) / 2.0f) * (1 - minBrightnessFactor);
    currentBrightness = brightnessFactor * breathMaxBrightness * mainBrightnessFactor;

    FastLED.setBrightness(currentBrightness);
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();

    if (checkIfButtonPressed()) return;
  }

  fadeOutLeds(currentBrightness, 800);
  turnOffLeds();
}
