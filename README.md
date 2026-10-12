# TinyML-Based Coffee Soil Suitability Prediction System

An ESP32-based smart agriculture prototype that uses soil measurements and an embedded machine-learning model to classify coffee-growing conditions into three categories:

- **Suitable for Robusta**
- **Suitable for Arabica**
- **Not Suitable**

> **Research prototype:** The current model uses provisional, rule-derived labels and synthetic training records anchored to published Ugandan regional soil measurements. Predictions are experimental and are **not** a substitute for laboratory soil testing or professional agronomic advice.

## Overview

The system combines an **ESP32 microcontroller**, an **NPK soil sensor**, soil pH information, a **TinyML classifier**, and a **TFT display**. It is intended to demonstrate how local soil measurements can be processed directly on a microcontroller to support preliminary coffee-growing assessments in Uganda.

The classifier uses four soil features in its current version: **pH, nitrogen (N), phosphorus (P), and potassium (K)**. Reliable differentiation between Arabica and Robusta also requires climatic and geographic variables such as temperature and altitude; these are planned improvements.

## Objectives

- Measure soil N, P, and K using an electronic soil sensor.
- Prepare and analyze coffee-soil data informed by Ugandan research.
- Train and evaluate a three-class machine-learning model.
- Convert the trained model into a microcontroller-compatible TensorFlow Lite format.
- Run inference locally on the ESP32 and present results on a TFT display.

## Hardware

| Component | Purpose |
| --- | --- |
| ESP32 development board | Sensor acquisition and on-device inference |
| NPK soil sensor (typically RS485/Modbus) | Soil nitrogen, phosphorus, and potassium measurements |
| RS485-to-UART interface | Communication between compatible NPK sensors and ESP32 |
| Soil pH sensor | Soil acidity measurement |
| TFT display | Sensor values, prediction, and system status |
| Suitable power supply and wiring | Stable operation |

**Note:** Confirm your specific sensor's voltage, protocol, electrical interface, calibration requirements, and measurement units before wiring it to the ESP32. Some RS485 modules require 5 V power and proper 3.3 V logic interfacing.

## System Workflow

```text

## System Architecture

The following block diagram illustrates the operation of the
TinyML-Based Coffee Soil Suitability Prediction System.

![Coffee Soil Suitability System Block Diagram](Coffee Soil Suitability Analyzer Diagram.png)
  
```

## Dataset and NARO Research

The project draws on Ugandan coffee-soil research published through the **National Agricultural Research Organisation (NARO)**, particularly the study **“Characterisation and Mapping of Soils in Major Coffee Growing Regions of Uganda.”** The study reports measurements and regional summaries from **717 soil samples across 45 coffee-growing districts**, including soil pH, N, P, K, and other properties.

Source: [NARO research repository](https://researchspace.naro.go.ug/items/7ede5a00-5c62-4414-b39c-0099b432767e)

**Data provenance:** The study's published regional summaries are not the same as the complete sample-level laboratory dataset. The prototype ML CSV contains **2,100 derived/synthetic rows** based on regional soil statistics and heuristic suitability rules. The three-class Arabica/Robusta labels are **temporary proxies**, not verified field labels. Do not describe these 2,100 rows as original NARO observations.

### Model Inputs

| Input | Training unit |
| --- | --- |
| Soil pH | pH scale |
| Nitrogen | % |
| Phosphorus | mg/kg |
| Potassium | meq/100 g |

**Important:** Many commercial NPK sensors report N, P, and K in mg/kg. Raw sensor outputs must not be passed directly to the model unless units, measurement basis, and calibration are compatible. A numerical unit conversion alone does not guarantee that sensor readings and laboratory measurements are equivalent.

## Machine-Learning Pipeline

1. Import and validate the CSV dataset.
2. Perform exploratory data analysis and check class balance.
3. Select deployable features and separate the three-class target.
4. Split data into training, validation, and test subsets.
5. Fit a standard scaler **using training data only**.
6. Train a compact neural network and evaluate precision, recall, F1-score, accuracy, and confusion matrix.
7. Convert the model to **TensorFlow Lite** and compare its predictions with the original model.
8. Package the model, scaler parameters, labels, and inference code into an Arduino library.

The initial neural network uses **4 inputs → 16 hidden units → 8 hidden units → 3 softmax outputs**.

> **Evaluation caution:** Random splits of synthetic, rule-labeled data measure how well the model reproduces the synthetic labeling scheme; they do not establish accuracy on real Ugandan farms. Independent field and laboratory validation is required.

## Software and Tools

- Google Colab / Python
- Pandas, NumPy, scikit-learn, Matplotlib
- TensorFlow / TensorFlow Lite
- Arduino IDE and ESP32 board support
- ArduTFLite and its compatible TensorFlow Lite Micro runtime

## Training and Deployment

1. Open the project training notebook in **Google Colab**.
2. Upload `uganda_coffee_model_ready_3class.csv`.
3. Run the notebook cells to train, evaluate, and convert the model.
4. Download the generated `CoffeeSoilESP32.zip` library.
5. Install the required Arduino inference runtime libraries using **Arduino IDE → Library Manager**.
6. Install the generated ZIP via **Sketch → Include Library → Add .ZIP Library**.
7. Open the included `CoffeeSoilPrediction` example, select the ESP32 board, and compile.
8. Validate sensor inputs and units before replacing the example values with live readings.

### Arduino Inference Example

```cpp
#include <CoffeeSoilESP32.h>

CoffeeSoilESP32 coffee;

void setup() {
  Serial.begin(115200);

  if (!coffee.begin()) {
    Serial.println("Model initialization failed");
    while (true) delay(1000);
  }

  // Example only: values must match training units.
  CoffeePrediction result = coffee.predict(
    5.8,   // pH
    0.18,  // Nitrogen (%)
    35.0,  // Phosphorus (mg/kg)
    0.80   // Potassium (meq/100 g)
  );

  Serial.print("Prediction: ");
  Serial.println(result.label);
  Serial.print("Model confidence: ");
  Serial.println(result.confidence, 3);
}

void loop() {}
```

The example illustrates the **generated library API**. Compilation depends on a compatible Arduino ESP32 core and TensorFlow Lite runtime; verify those versions on your hardware.

## TFT Dashboard (Planned / Prototype Features)

- Live NPK and pH readings
- Three-class coffee suitability prediction
- Per-class model scores
- Sensor connection and error status
- Nutrient status indicators
- Measurement history and export (future enhancement)

Model confidence is **not** the same as agronomic certainty.

## Limitations

- Training labels are derived rather than verified against individual farm outcomes.
- The present four-input model omits altitude, temperature, rainfall, drainage, and soil texture.
- Low-cost NPK sensors require independent calibration and may differ from laboratory tests.
- Regional soil averages do not describe every farm within a region.
- The model requires validation using independent real-world soil samples before agricultural deployment.

## Future Improvements

- Collect labeled soil samples from Ugandan Arabica and Robusta farms.
- Add altitude, temperature, rainfall, drainage, and soil texture.
- Validate sensor readings against laboratory soil tests.
- Compare model predictions against expert agronomic assessments.
- Improve TFT navigation, data logging, and export capabilities.
- Assess prediction reliability across different Ugandan coffee-growing regions.

## Reference

National Agricultural Research Organisation (NARO), *Characterisation and Mapping of Soils in Major Coffee Growing Regions of Uganda*. [Research repository record](https://researchspace.naro.go.ug/items/7ede5a00-5c62-4414-b39c-0099b432767e).

## Project Status

**Prototype / ongoing development.** Model development and ESP32 integration require further field validation.

## License

No license has been selected for this repository. Add a `LICENSE` file if you wish to define reuse permissions.
