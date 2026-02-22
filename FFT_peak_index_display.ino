/*I tested with 1 khz and 2 khz tones and expected to see 23, 46 on the max value indexes.
 at first, the FFT system appeared to not work as the indexes of the max value were detected as 0,
 which turned out to be caused by DC offset dominating the signal. 
 The issue was solved by adding a DC blocker filter before the FFT.
After doing these tests, the max values indexes appeared as 23 for 1 khz and 46 for 2 khz .*/


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>

// ===== Audio Objects =====
AudioInputI2S        micInput;
AudioFilterBiquad    dcBlocker;     // DC removal filter
AudioAnalyzeFFT1024  fft;

AudioConnection patchCord1(micInput, 0, dcBlocker, 0);
AudioConnection patchCord2(dcBlocker, 0, fft, 0);

AudioControlSGTL5000 audioShield;

void setup() {
  Serial.begin(250000);
  AudioMemory(40);

  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.micGain(50);

  // ---- DC Blocker Setup ----
  // High-pass filter at 20 Hz (removes DC only)
  dcBlocker.setHighpass(0, 20, 0.707);
}

void loop() {

  if (fft.available()) {

    float maxMag = 0.0;
    int maxIndex = 0;

    for (int i = 0; i < 512; i++) {

      float mag = fft.read(i);

      if (mag > maxMag) {
        maxMag = mag;
        maxIndex = i;
      }
    }

    Serial.println(maxIndex);
  }
}
