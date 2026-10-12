#if 0 // Superseded implementation retained for reference.
#include <HardwareSerial.h> // ESP32 UARTs
#if 0 // Previous implementation retained below for reference.

// =====================================================
// UART OBJECTS
// =====================================================

// NPK sensor UART
HardwareSerial npk(2);

// UART from ESP32 to ATmega328P
HardwareSerial displayUART(1);

// =====================================================
// PIN DEFINITIONS
// =====================================================

// RS485 / NPK
#define RE_AND_DE  4
#define NPK_RX     16
#define NPK_TX     17

// ESP32 -> ATmega328P
#define ATMEGA_TX  25

// =====================================================
// NPK MODBUS REQUEST
// =====================================================

const byte npkRequest[] =
{
  0x01, 0x03, 0x00, 0x00,
  0x00, 0x07, 0x04, 0x08
};

byte buffer[64];

// =====================================================
// SENSOR VALUES
// =====================================================

float moisture     = 0;
float temperature  = 0;
float conductivity = 0;
float ph           = 0;
float nitrogen     = 0;
float phosphorus   = 0;
float potassium    = 0;


// =====================================================
// CONVERT TWO BYTES TO 16-BIT VALUE
// =====================================================

uint16_t wordFromBytes(byte msb, byte lsb)
{
  return ((uint16_t)msb << 8) | lsb;
}


// =====================================================
// READ NPK SENSOR
// =====================================================

bool readNPK()
{
  // Clear old UART data
  while (npk.available())
  {
    npk.read();
  }

  memset(buffer, 0, sizeof(buffer));

  // ---------------------------------------------------
  // RS485 TRANSMIT MODE
  // ---------------------------------------------------

  digitalWrite(RE_AND_DE, HIGH);

  delay(10);

  npk.write(
    npkRequest,
    sizeof(npkRequest)
  );

  npk.flush();

  delay(5);

  // ---------------------------------------------------
  // RS485 RECEIVE MODE
  // ---------------------------------------------------

  digitalWrite(RE_AND_DE, LOW);

  unsigned long startTime = millis();

  int index = 0;

  // Wait for response
  while ((millis() - startTime) < 1500 &&
         index < sizeof(buffer))
  {
    if (npk.available())
    {
      buffer[index++] = npk.read();
    }
  }

  // ---------------------------------------------------
  // NO RESPONSE
  // ---------------------------------------------------

  if (index == 0)
  {
    Serial.println("ERROR: NPK sensor timeout.");
    return false;
  }

  // ---------------------------------------------------
  // FIND VALID MODBUS FRAME
  // ---------------------------------------------------

  int frameStart = -1;

  for (int i = 0; i < index - 2; i++)
  {
    if (buffer[i]     == 0x01 &&
        buffer[i + 1] == 0x03 &&
        buffer[i + 2] == 0x0E)
    {
      frameStart = i;
      break;
    }
  }

  if (frameStart == -1)
  {
    Serial.println("ERROR: Invalid NPK Modbus frame.");
    return false;
  }

  if ((index - frameStart) < 19)
  {
    Serial.println("ERROR: Incomplete NPK frame.");
    return false;
  }

  byte *data = &buffer[frameStart];

  // ---------------------------------------------------
  // EXTRACT SENSOR VALUES
  // ---------------------------------------------------

  moisture =
    wordFromBytes(
      data[3],
      data[4]
    ) / 10.0;

  temperature =
    wordFromBytes(
      data[5],
      data[6]
    ) / 10.0;

  conductivity =
    wordFromBytes(
      data[7],
      data[8]
    );

  ph =
    wordFromBytes(
      data[9],
      data[10]
    ) / 10.0;

  nitrogen =
    wordFromBytes(
      data[11],
      data[12]
    );

  phosphorus =
    wordFromBytes(
      data[13],
      data[14]
    );

  potassium =
    wordFromBytes(
      data[15],
      data[16]
    );

  return true;
}


// =====================================================
// PRINT NPK DATA TO ESP32 SERIAL MONITOR
// =====================================================

void printNPK()
{
  Serial.println();
  Serial.println("================================");
  Serial.println("        NPK SENSOR DATA");
  Serial.println("================================");

  Serial.print("Moisture      : ");
  Serial.print(moisture, 1);
  Serial.println(" %");

  Serial.print("Temperature   : ");
  Serial.print(temperature, 1);
  Serial.println(" C");

  Serial.print("Conductivity  : ");
  Serial.print(conductivity, 0);
  Serial.println(" uS/cm");

  Serial.print("pH            : ");
  Serial.println(ph, 1);

  Serial.print("Nitrogen      : ");
  Serial.print(nitrogen, 0);
  Serial.println(" mg/kg");

  Serial.print("Phosphorus    : ");
  Serial.print(phosphorus, 0);
  Serial.println(" mg/kg");

  Serial.print("Potassium     : ");
  Serial.print(potassium, 0);
  Serial.println(" mg/kg");

  Serial.println("================================");
}


// =====================================================
// SEND NPK DATA TO ATmega328P
// =====================================================

void sendToATmega()
{
  /*
     Packet format:

     <M,T,EC,PH,N,P,K>

     Example:

     <35.2,25.6,450,6.8,30,25,40>
  */

  displayUART.print("<");

  displayUART.print(moisture, 1);
  displayUART.print(",");

  displayUART.print(temperature, 1);
  displayUART.print(",");

  displayUART.print(conductivity, 0);
  displayUART.print(",");

  displayUART.print(ph, 1);
  displayUART.print(",");

  displayUART.print(nitrogen, 0);
  displayUART.print(",");

  displayUART.print(phosphorus, 0);
  displayUART.print(",");

  displayUART.print(potassium, 0);

  displayUART.println(">");

  // ---------------------------------------------------
  // DEBUG
  // ---------------------------------------------------

  Serial.print("Sent to TFT: <");

  Serial.print(moisture, 1);
  Serial.print(",");

  Serial.print(temperature, 1);
  Serial.print(",");

  Serial.print(conductivity, 0);
  Serial.print(",");

  Serial.print(ph, 1);
  Serial.print(",");

  Serial.print(nitrogen, 0);
  Serial.print(",");

  Serial.print(phosphorus, 0);
  Serial.print(",");

  Serial.print(potassium, 0);

  Serial.println(">");
}


// =====================================================
// SETUP
// =====================================================

void setup()
{
  // ESP32 USB Serial Monitor
  Serial.begin(115200);

  delay(500);

  // ---------------------------------------------------
  // NPK SENSOR UART
  // ---------------------------------------------------

  npk.begin(
    4800,
    SERIAL_8N1,
    NPK_RX,
    NPK_TX
  );

  // ---------------------------------------------------
  // UART TO ATmega328P
  //
  // RX = -1 because communication is one-way.
  // ---------------------------------------------------

  displayUART.begin(
    9600,
    SERIAL_8N1,
    -1,
    ATMEGA_TX
  );

  // ---------------------------------------------------
  // RS485 CONTROL
  // ---------------------------------------------------

  pinMode(
    RE_AND_DE,
    OUTPUT
  );

  digitalWrite(
    RE_AND_DE,
    LOW
  );

  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println(" ESP32 NPK SOIL MONITOR");
  Serial.println(" UART -> ATmega328P");
  Serial.println("================================");
}


// =====================================================
// LOOP
// =====================================================

void loop()
{
  Serial.println();
  Serial.println("Reading NPK sensor...");

  if (readNPK())
  {
    printNPK();

    sendToATmega();
  }
  else
  {
    Serial.println(
      "Failed to read NPK sensor."
    );
  }

  // Update every 3 seconds
  delay(3000);
}
#endif

// Active implementation. Existing working pins and baud rates are unchanged.
#include <HardwareSerial.h>
#include <CoffeeSoilESP32.h>

constexpr uint8_t RS485_RE_DE = 4;
constexpr int8_t NPK_RX_PIN = 16;
constexpr int8_t NPK_TX_PIN = 17;
constexpr int8_t ATMEGA_TX_PIN = 25;
constexpr uint32_t SAMPLE_MS = 3000;

HardwareSerial sensorPort(2);
HardwareSerial tftPort(1);
CoffeeSoilESP32 coffeeModel;
bool modelReady = false;

const uint8_t requestFrame[] = {
  0x01, 0x03, 0x00, 0x00, 0x00, 0x07, 0x04, 0x08
};

struct Reading {
  float moisture, temperature, ph;
  uint16_t conductivity, nitrogen, phosphorus, potassium;
} value;

CoffeePrediction prediction;

uint16_t makeWord(uint8_t hi, uint8_t lo) {
  return (static_cast<uint16_t>(hi) << 8) | lo;
}

uint16_t modbusCRC(const uint8_t *data, size_t length) {
  uint16_t crc = 0xFFFF;
  while (length--) {
    crc ^= *data++;
    for (uint8_t bit = 0; bit < 8; ++bit)
      crc = (crc & 1) ? ((crc >> 1) ^ 0xA001) : (crc >> 1);
  }
  return crc;
}

bool readSensor() {
  uint8_t reply[32];
  size_t count = 0;
  while (sensorPort.available()) sensorPort.read();

  digitalWrite(RS485_RE_DE, HIGH);
  delay(10);
  sensorPort.write(requestFrame, sizeof(requestFrame));
  sensorPort.flush();
  delay(5);
  digitalWrite(RS485_RE_DE, LOW);

  uint32_t started = millis(), lastByte = started;
  while (millis() - started < 1500 && count < sizeof(reply)) {
    if (sensorPort.available()) {
      reply[count++] = sensorPort.read();
      lastByte = millis();
    } else if (count >= 19 && millis() - lastByte > 20) break;
  }

  int8_t start = -1;
  for (size_t i = 0; i + 18 < count; ++i)
    if (reply[i] == 1 && reply[i + 1] == 3 && reply[i + 2] == 14) {
      start = i;
      break;
    }
  if (start < 0 || count - start < 19) return false;

  const uint8_t *f = reply + start;
  if (modbusCRC(f, 17) != makeWord(f[18], f[17])) return false;

  Reading next;
  next.moisture = makeWord(f[3], f[4]) / 10.0f;
  next.temperature = static_cast<int16_t>(makeWord(f[5], f[6])) / 10.0f;
  next.conductivity = makeWord(f[7], f[8]);
  next.ph = makeWord(f[9], f[10]) / 10.0f;
  next.nitrogen = makeWord(f[11], f[12]);
  next.phosphorus = makeWord(f[13], f[14]);
  next.potassium = makeWord(f[15], f[16]);
  if (next.moisture < 0 || next.moisture > 100 ||
      next.temperature < -40 || next.temperature > 125 ||
      next.ph < 0 || next.ph > 14) return false;
  value = next;
  return true;
}

void makePrediction() {
  // Sensor: N/P/K = mg/kg. Model: N = %, P = mg/kg, K = meq/100 g.
  const float nitrogenPercent = value.nitrogen / 10000.0f;
  const float potassiumMeq100g = value.potassium / 391.0f;
  prediction = coffeeModel.predict(value.ph, nitrogenPercent,
                                   value.phosphorus, potassiumMeq100g);
}

void sendReading() {
  // Packet: <moisture,temp,EC,pH,N,P,K,classId,confidence>
  tftPort.printf("<%.1f,%.1f,%u,%.1f,%u,%u,%u,%d,%.3f>\n", value.moisture,
                 value.temperature, value.conductivity, value.ph,
                 value.nitrogen, value.phosphorus, value.potassium,
                 static_cast<int>(prediction.classId), prediction.confidence);
  Serial.printf("TFT <- <%.1f,%.1f,%u,%.1f,%u,%u,%u,%d,%.3f>\n", value.moisture,
                value.temperature, value.conductivity, value.ph,
                value.nitrogen, value.phosphorus, value.potassium,
                static_cast<int>(prediction.classId), prediction.confidence);
  Serial.printf("Coffee prediction: %s (%.1f%%)\n",
                prediction.label, prediction.confidence * 100.0f);
}

void setup() {
  Serial.begin(115200);
  sensorPort.begin(4800, SERIAL_8N1, NPK_RX_PIN, NPK_TX_PIN);
  tftPort.begin(9600, SERIAL_8N1, -1, ATMEGA_TX_PIN);
  pinMode(RS485_RE_DE, OUTPUT);
  digitalWrite(RS485_RE_DE, LOW);
  delay(1000);
  modelReady = coffeeModel.begin();
  if (!modelReady) {
    Serial.println("Coffee model initialization failed");
  }
  Serial.println("ESP32 soil monitor ready");
}

void loop() {
  static uint32_t previous = 0;
  if (previous == 0 || millis() - previous >= SAMPLE_MS) {
    previous = millis();
    if (readSensor() && modelReady) {
      makePrediction();
      sendReading();
    }
    else if (!modelReady) Serial.println("Prediction unavailable: model not initialized");
    else Serial.println("NPK read failed: check sensor, RS485 wiring, or CRC");
  }
}
#endif

// Encoder-driven TFT controller link.
#include <HardwareSerial.h>
#include <CoffeeSoilESP32.h>

constexpr uint8_t RS485_RE_DE = 4;
constexpr int8_t NPK_RX_PIN = 16, NPK_TX_PIN = 17;
constexpr int8_t ATMEGA_RX_PIN = 26, ATMEGA_TX_PIN = 25;
constexpr uint8_t ENCODER_CLK_PIN = 18, ENCODER_DT_PIN = 19, ENCODER_SW_PIN = 23;
constexpr uint32_t SAMPLE_MS = 3000, DEBOUNCE_MS = 35, ENCODER_STEP_MS = 120;
constexpr uint8_t SCREEN_COUNT = 4;

HardwareSerial sensorPort(2), tftPort(1);
CoffeeSoilESP32 coffeeModel;
const uint8_t requestFrame[] = {0x01, 0x03, 0, 0, 0, 7, 0x04, 0x08};

struct Reading {
  float moisture, temperature, ph;
  uint16_t conductivity, nitrogen, phosphorus, potassium;
} reading;
CoffeePrediction prediction;
bool modelReady = false;
uint8_t selectedScreen = 0, activeScreen = 0, lastEncoderState = 0;
int8_t encoderAccumulator = 0;
bool lastButton = HIGH, stableButton = HIGH;
uint32_t buttonChangedAt = 0, lastEncoderStepAt = 0;

uint16_t word16(uint8_t hi, uint8_t lo) { return (uint16_t(hi) << 8) | lo; }
uint16_t crc16(const uint8_t *p, size_t n) {
  uint16_t crc = 0xFFFF;
  while (n--) {
    crc ^= *p++;
    for (uint8_t i = 0; i < 8; ++i) crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : crc >> 1;
  }
  return crc;
}

bool readSoilSensor() {
  uint8_t reply[32]; size_t count = 0;
  while (sensorPort.available()) sensorPort.read();
  digitalWrite(RS485_RE_DE, HIGH); delay(10);
  sensorPort.write(requestFrame, sizeof(requestFrame)); sensorPort.flush(); delay(5);
  digitalWrite(RS485_RE_DE, LOW);
  uint32_t started = millis(), lastByte = started;
  while (millis() - started < 1500 && count < sizeof(reply)) {
    if (sensorPort.available()) { reply[count++] = sensorPort.read(); lastByte = millis(); }
    else if (count >= 19 && millis() - lastByte > 20) break;
  }
  int8_t start = -1;
  for (size_t i = 0; i + 18 < count; ++i)
    if (reply[i] == 1 && reply[i + 1] == 3 && reply[i + 2] == 14) { start = i; break; }
  if (start < 0 || count - start < 19) return false;
  const uint8_t *f = reply + start;
  if (crc16(f, 17) != word16(f[18], f[17])) return false;
  Reading next;
  next.moisture = word16(f[3], f[4]) / 10.0f;
  next.temperature = int16_t(word16(f[5], f[6])) / 10.0f;
  next.conductivity = word16(f[7], f[8]); next.ph = word16(f[9], f[10]) / 10.0f;
  next.nitrogen = word16(f[11], f[12]); next.phosphorus = word16(f[13], f[14]);
  next.potassium = word16(f[15], f[16]);
  if (next.moisture < 0 || next.moisture > 100 || next.temperature < -40 ||
      next.temperature > 125 || next.ph < 0 || next.ph > 14) return false;
  reading = next; return true;
}

void sendReadingToTFT() {
  tftPort.printf("<DATA,%.1f,%.1f,%u,%.1f,%u,%u,%u,%d,%.3f>\n",
    reading.moisture, reading.temperature, reading.conductivity, reading.ph,
    reading.nitrogen, reading.phosphorus, reading.potassium,
    int(prediction.classId), prediction.confidence);
}
void sendNavigation() { tftPort.printf("<NAV,%u,%u>\n", selectedScreen, activeScreen); }

void updateEncoder() {
  // Decode a complete quadrature detent instead of reacting to one noisy edge.
  static const int8_t transition[] = {
     0, -1,  1,  0,
     1,  0,  0, -1,
    -1,  0,  0,  1,
     0,  1, -1,  0
  };
  uint8_t state = (digitalRead(ENCODER_CLK_PIN) << 1) | digitalRead(ENCODER_DT_PIN);
  if (state != lastEncoderState) {
    encoderAccumulator += transition[(lastEncoderState << 2) | state];
    lastEncoderState = state;
  }
  if ((encoderAccumulator >= 4 || encoderAccumulator <= -4) &&
      millis() - lastEncoderStepAt >= ENCODER_STEP_MS) {
    if (encoderAccumulator > 0) selectedScreen = (selectedScreen + 1) % SCREEN_COUNT;
    else selectedScreen = (selectedScreen + SCREEN_COUNT - 1) % SCREEN_COUNT;
    encoderAccumulator = 0;
    lastEncoderStepAt = millis();
    sendNavigation();
  } else if (encoderAccumulator > 4 || encoderAccumulator < -4) {
    encoderAccumulator = 0;
  }
  bool button = digitalRead(ENCODER_SW_PIN);
  if (button != lastButton) buttonChangedAt = millis();
  if (millis() - buttonChangedAt >= DEBOUNCE_MS && button != stableButton) {
    stableButton = button;
    if (stableButton == LOW) { activeScreen = selectedScreen; sendNavigation(); }
  }
  lastButton = button;
}

void readTFTStatus() {
  static char line[40]; static uint8_t length = 0;
  while (tftPort.available()) {
    char c = tftPort.read();
    if (c == '\n' || c == '\r') {
      if (length) { line[length] = 0; Serial.print("TFT -> "); Serial.println(line); length = 0; }
    } else if (length < sizeof(line) - 1) line[length++] = c;
    else length = 0;
  }
}

void setup() {
  Serial.begin(115200);
  sensorPort.begin(4800, SERIAL_8N1, NPK_RX_PIN, NPK_TX_PIN);
  tftPort.begin(9600, SERIAL_8N1, ATMEGA_RX_PIN, ATMEGA_TX_PIN);
  pinMode(RS485_RE_DE, OUTPUT); digitalWrite(RS485_RE_DE, LOW);
  pinMode(ENCODER_CLK_PIN, INPUT_PULLUP); pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
  pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
  lastEncoderState = (digitalRead(ENCODER_CLK_PIN) << 1) | digitalRead(ENCODER_DT_PIN);
  delay(1000); modelReady = coffeeModel.begin(); sendNavigation();
  Serial.println(modelReady ? "ESP32 soil monitor ready" : "Coffee model failed");
}

void loop() {
  updateEncoder(); readTFTStatus();
  static uint32_t previous = 0;
  if (!previous || millis() - previous >= SAMPLE_MS) {
    previous = millis();
    if (readSoilSensor() && modelReady) {
      prediction = coffeeModel.predict(reading.ph, reading.nitrogen / 10000.0f,
                                       reading.phosphorus, reading.potassium / 391.0f);
      sendReadingToTFT();
    } else Serial.println(modelReady ? "NPK read failed" : "Model unavailable");
  }
}
