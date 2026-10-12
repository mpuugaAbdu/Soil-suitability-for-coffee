#include <CoffeeSoilESP32.h>
CoffeeSoilESP32 coffee;

void setup() {
  Serial.begin(115200);
  delay(1000);

  if(!coffee.begin()) {
    Serial.println("Model initialization failed.");
    while(true) delay(1000);
  }

  // Replace with real sensor values in the SAME units used during training.
  CoffeePrediction r = coffee.predict(5.8, 0.18, 35.0, 0.80);

  Serial.print("Prediction: "); Serial.println(r.label);
  Serial.print("Confidence: "); Serial.println(r.confidence, 4);
  Serial.print("Robusta: "); Serial.println(r.probabilities[0], 4);
  Serial.print("Arabica: "); Serial.println(r.probabilities[1], 4);
  Serial.print("Not suitable: "); Serial.println(r.probabilities[2], 4);
}
void loop() {}
