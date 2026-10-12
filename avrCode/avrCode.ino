#if 0 // Superseded implementation retained for reference.
#include <UTFTGLUE.h>
#if 0 // Previous interface retained below for reference.

// ============================================================
// TFT
// Same constructor as your WORKING example
// ============================================================

UTFTGLUE myGLCD(0, A2, A1, A3, A4, A0);


// ============================================================
// UART RECEIVE
// ============================================================

char receiveBuffer[80];

byte bufferIndex = 0;

bool receivingPacket = false;


// ============================================================
// SENSOR VALUES
// ============================================================

float moisture = 0.0;
float temperature = 0.0;
float conductivity = 0.0;
float soilPH = 0.0;

int nitrogen = 0;
int phosphorus = 0;
int potassium = 0;


// ============================================================
// COMMUNICATION STATUS
// ============================================================

unsigned long lastDataTime = 0;

bool receivedFirstPacket = false;

bool linkLostDisplayed = false;


// ============================================================
// CLEAR VALUE AREA
// ============================================================

void clearValueArea(
  int x1,
  int y1,
  int x2,
  int y2
)
{
  myGLCD.setColor(
    25,
    25,
    25
  );

  myGLCD.fillRect(
    x1,
    y1,
    x2,
    y2
  );
}


// ============================================================
// DRAW WAITING STATUS
// ============================================================

void drawWaitingStatus()
{
  myGLCD.setColor(
    0,
    0,
    0
  );

  myGLCD.fillRect(
    0,
    290,
    479,
    319
  );


  myGLCD.setBackColor(
    0,
    0,
    0
  );

  myGLCD.setColor(
    255,
    255,
    0
  );

  myGLCD.print(
    "WAITING FOR ESP32 DATA...",
    CENTER,
    300
  );
}


// ============================================================
// DRAW LIVE STATUS
// ============================================================

void drawLiveStatus()
{
  myGLCD.setColor(
    0,
    0,
    0
  );

  myGLCD.fillRect(
    0,
    290,
    479,
    319
  );


  myGLCD.setBackColor(
    0,
    0,
    0
  );

  myGLCD.setColor(
    0,
    255,
    0
  );

  myGLCD.print(
    "SENSOR DATA LIVE",
    CENTER,
    300
  );
}


// ============================================================
// DRAW LOST STATUS
// ============================================================

void drawLostStatus()
{
  myGLCD.setColor(
    0,
    0,
    0
  );

  myGLCD.fillRect(
    0,
    290,
    479,
    319
  );


  myGLCD.setBackColor(
    0,
    0,
    0
  );

  myGLCD.setColor(
    255,
    0,
    0
  );

  myGLCD.print(
    "ESP32 UART LINK LOST",
    CENTER,
    300
  );
}


// ============================================================
// DRAW MAIN INTERFACE
// ============================================================

void drawMainScreen()
{
  // ==========================================================
  // BACKGROUND
  // ==========================================================

  myGLCD.clrScr();


  // ==========================================================
  // HEADER
  // ==========================================================

  myGLCD.setColor(
    0,
    100,
    200
  );

  myGLCD.fillRect(
    0,
    0,
    479,
    45
  );


  myGLCD.setBackColor(
    0,
    100,
    200
  );

  myGLCD.setColor(
    255,
    255,
    255
  );

  myGLCD.print(
    "SOIL NPK MONITORING SYSTEM",
    CENTER,
    15
  );


  // ==========================================================
  // MAIN BACKGROUND
  // ==========================================================

  myGLCD.setColor(
    0,
    0,
    0
  );

  myGLCD.fillRect(
    0,
    46,
    479,
    319
  );


  // ==========================================================
  // MOISTURE PANEL
  // ==========================================================

  myGLCD.setColor(
    25,
    25,
    25
  );

  myGLCD.fillRoundRect(
    8,
    55,
    232,
    125
  );


  myGLCD.setBackColor(
    25,
    25,
    25
  );

  myGLCD.setColor(
    180,
    180,
    180
  );

  myGLCD.print(
    "MOISTURE",
    CENTER,
    63
  );


  // ==========================================================
  // TEMPERATURE PANEL
  // ==========================================================

  myGLCD.setColor(
    25,
    25,
    25
  );

  myGLCD.fillRoundRect(
    247,
    55,
    471,
    125
  );


  myGLCD.setBackColor(
    25,
    25,
    25
  );

  myGLCD.setColor(
    180,
    180,
    180
  );

  myGLCD.print(
    "TEMPERATURE",
    310,
    63
  );


  // ==========================================================
  // PH PANEL
  // ==========================================================

  myGLCD.setColor(
    25,
    25,
    25
  );

  myGLCD.fillRoundRect(
    8,
    135,
    232,
    205
  );


  myGLCD.setBackColor(
    25,
    25,
    25
  );

  myGLCD.setColor(
    180,
    180,
    180
  );

  myGLCD.print(
    "SOIL pH",
    85,
    143
  );


  // ==========================================================
  // CONDUCTIVITY PANEL
  // ==========================================================

  myGLCD.setColor(
    25,
    25,
    25
  );

  myGLCD.fillRoundRect(
    247,
    135,
    471,
    205
  );


  myGLCD.setBackColor(
    25,
    25,
    25
  );

  myGLCD.setColor(
    180,
    180,
    180
  );

  myGLCD.print(
    "CONDUCTIVITY",
    305,
    143
  );


  // ==========================================================
  // NITROGEN PANEL
  // ==========================================================

  myGLCD.setColor(
    25,
    25,
    25
  );

  myGLCD.fillRoundRect(
    8,
    215,
    152,
    282
  );


  myGLCD.setBackColor(
    25,
    25,
    25
  );

  myGLCD.setColor(
    180,
    180,
    180
  );

  myGLCD.print(
    "NITROGEN",
    45,
    222
  );


  // ==========================================================
  // PHOSPHORUS PANEL
  // ==========================================================

  myGLCD.setColor(
    25,
    25,
    25
  );

  myGLCD.fillRoundRect(
    168,
    215,
    312,
    282
  );


  myGLCD.setBackColor(
    25,
    25,
    25
  );

  myGLCD.setColor(
    180,
    180,
    180
  );

  myGLCD.print(
    "PHOSPHORUS",
    195,
    222
  );


  // ==========================================================
  // POTASSIUM PANEL
  // ==========================================================

  myGLCD.setColor(
    25,
    25,
    25
  );

  myGLCD.fillRoundRect(
    328,
    215,
    471,
    282
  );


  myGLCD.setBackColor(
    25,
    25,
    25
  );

  myGLCD.setColor(
    180,
    180,
    180
  );

  myGLCD.print(
    "POTASSIUM",
    365,
    222
  );


  // ==========================================================
  // INITIAL VALUES
  // ==========================================================

  myGLCD.setColor(
    0,
    255,
    255
  );

  myGLCD.print(
    "--",
    105,
    90
  );


  myGLCD.setColor(
    255,
    180,
    0
  );

  myGLCD.print(
    "--",
    345,
    90
  );


  myGLCD.setColor(
    0,
    255,
    0
  );

  myGLCD.print(
    "--",
    105,
    170
  );


  myGLCD.setColor(
    255,
    255,
    0
  );

  myGLCD.print(
    "--",
    345,
    170
  );


  myGLCD.setColor(
    0,
    255,
    0
  );

  myGLCD.print(
    "--",
    70,
    250
  );


  myGLCD.setColor(
    255,
    255,
    0
  );

  myGLCD.print(
    "--",
    230,
    250
  );


  myGLCD.setColor(
    0,
    255,
    255
  );

  myGLCD.print(
    "--",
    390,
    250
  );


  drawWaitingStatus();
}


// ============================================================
// DISPLAY VALUES
// ============================================================

void displayValues()
{
  // ==========================================================
  // CLEAR OLD VALUES
  // ==========================================================

  clearValueArea(
    20,
    82,
    220,
    115
  );

  clearValueArea(
    260,
    82,
    460,
    115
  );

  clearValueArea(
    20,
    162,
    220,
    195
  );

  clearValueArea(
    260,
    162,
    460,
    195
  );

  clearValueArea(
    20,
    242,
    140,
    275
  );

  clearValueArea(
    180,
    242,
    300,
    275
  );

  clearValueArea(
    340,
    242,
    460,
    275
  );


  // ==========================================================
  // MOISTURE
  // ==========================================================

  myGLCD.setBackColor(
    25,
    25,
    25
  );

  myGLCD.setColor(
    0,
    255,
    255
  );


  myGLCD.printNumF(
    moisture,
    1,
    85,
    90
  );


  myGLCD.print(
    "%",
    145,
    90
  );


  // ==========================================================
  // TEMPERATURE
  // ==========================================================

  myGLCD.setColor(
    255,
    180,
    0
  );


  myGLCD.printNumF(
    temperature,
    1,
    320,
    90
  );


  myGLCD.print(
    "C",
    380,
    90
  );


  // ==========================================================
  // SOIL PH
  // ==========================================================

  myGLCD.setColor(
    0,
    255,
    0
  );


  myGLCD.printNumF(
    soilPH,
    1,
    100,
    170
  );


  // ==========================================================
  // CONDUCTIVITY
  // ==========================================================

  myGLCD.setColor(
    255,
    255,
    0
  );


  myGLCD.printNumI(
    (int)conductivity,
    320,
    170
  );


  // ==========================================================
  // NITROGEN
  // ==========================================================

  myGLCD.setColor(
    0,
    255,
    0
  );


  myGLCD.printNumI(
    nitrogen,
    60,
    250
  );


  // ==========================================================
  // PHOSPHORUS
  // ==========================================================

  myGLCD.setColor(
    255,
    255,
    0
  );


  myGLCD.printNumI(
    phosphorus,
    220,
    250
  );


  // ==========================================================
  // POTASSIUM
  // ==========================================================

  myGLCD.setColor(
    0,
    255,
    255
  );


  myGLCD.printNumI(
    potassium,
    380,
    250
  );


  drawLiveStatus();
}


// ============================================================
// PARSE RECEIVED PACKET
// ============================================================

bool parsePacket(char *data)
{
  char *token;


  // ==========================================================
  // MOISTURE
  // ==========================================================

  token = strtok(
    data,
    ","
  );

  if (token == NULL)
  {
    return false;
  }

  float newMoisture =
    atof(token);


  // ==========================================================
  // TEMPERATURE
  // ==========================================================

  token = strtok(
    NULL,
    ","
  );

  if (token == NULL)
  {
    return false;
  }

  float newTemperature =
    atof(token);


  // ==========================================================
  // CONDUCTIVITY
  // ==========================================================

  token = strtok(
    NULL,
    ","
  );

  if (token == NULL)
  {
    return false;
  }

  float newConductivity =
    atof(token);


  // ==========================================================
  // PH
  // ==========================================================

  token = strtok(
    NULL,
    ","
  );

  if (token == NULL)
  {
    return false;
  }

  float newPH =
    atof(token);


  // ==========================================================
  // NITROGEN
  // ==========================================================

  token = strtok(
    NULL,
    ","
  );

  if (token == NULL)
  {
    return false;
  }

  int newNitrogen =
    atoi(token);


  // ==========================================================
  // PHOSPHORUS
  // ==========================================================

  token = strtok(
    NULL,
    ","
  );

  if (token == NULL)
  {
    return false;
  }

  int newPhosphorus =
    atoi(token);


  // ==========================================================
  // POTASSIUM
  // ==========================================================

  token = strtok(
    NULL,
    ","
  );

  if (token == NULL)
  {
    return false;
  }

  int newPotassium =
    atoi(token);


  // ==========================================================
  // VALIDATION
  // ==========================================================

  if (newMoisture < 0.0 ||
      newMoisture > 100.0)
  {
    return false;
  }


  if (newPH < 0.0 ||
      newPH > 14.0)
  {
    return false;
  }


  // ==========================================================
  // SAVE VALUES
  // ==========================================================

  moisture =
    newMoisture;

  temperature =
    newTemperature;

  conductivity =
    newConductivity;

  soilPH =
    newPH;

  nitrogen =
    newNitrogen;

  phosphorus =
    newPhosphorus;

  potassium =
    newPotassium;


  return true;
}


// ============================================================
// READ UART FROM ESP32
// ============================================================

void readESP32()
{
  while (Serial.available() > 0)
  {
    char received =
      Serial.read();


    // ========================================================
    // START CHARACTER
    // ========================================================

    if (received == '<')
    {
      receivingPacket =
        true;

      bufferIndex =
        0;

      continue;
    }


    // ========================================================
    // END CHARACTER
    // ========================================================

    if (received == '>' &&
        receivingPacket)
    {
      receiveBuffer[
        bufferIndex
      ] = '\0';


      receivingPacket =
        false;


      // Parse packet
      if (parsePacket(receiveBuffer))
      {
        lastDataTime =
          millis();

        receivedFirstPacket =
          true;

        linkLostDisplayed =
          false;


        displayValues();
      }


      bufferIndex =
        0;

      continue;
    }


    // ========================================================
    // STORE CHARACTER
    // ========================================================

    if (receivingPacket)
    {
      if (bufferIndex <
          sizeof(receiveBuffer) - 1)
      {
        receiveBuffer[
          bufferIndex
        ] = received;

        bufferIndex++;
      }
      else
      {
        // Buffer overflow
        bufferIndex =
          0;

        receivingPacket =
          false;
      }
    }
  }
}


// ============================================================
// CONNECTION WATCHDOG
// ============================================================

void checkConnection()
{
  if (!receivedFirstPacket)
  {
    return;
  }


  // ESP32 sends every ~3 seconds.
  // Consider connection lost after 10 seconds.

  if ((millis() - lastDataTime) > 10000)
  {
    if (!linkLostDisplayed)
    {
      drawLostStatus();

      linkLostDisplayed =
        true;
    }
  }
}


// ============================================================
// SETUP
// ============================================================

void setup()
{
  // ==========================================================
  // UART
  // ==========================================================

  Serial.begin(9600);


  // ==========================================================
  // TFT
  // ==========================================================

  myGLCD.InitLCD();

  myGLCD.setFont(
    SmallFont
  );


  // ==========================================================
  // DRAW SCREEN
  // ==========================================================

  drawMainScreen();


  // Remove possible garbage from UART
  while (Serial.available())
  {
    Serial.read();
  }
}


// ============================================================
// LOOP
// ============================================================

void loop()
{
  // Receive ESP32 data
  readESP32();


  // Monitor connection
  checkConnection();
}
#endif

#include <stdlib.h>
#include <string.h>

// Active dashboard. The known-working TFT constructor is unchanged.
UTFTGLUE lcd(0, A2, A1, A3, A4, A0);

constexpr uint8_t PACKET_SIZE = 64;
constexpr uint32_t LINK_TIMEOUT_MS = 10000;
char packet[PACKET_SIZE];
uint8_t packetLength = 0;
bool receiving = false, dataReceived = false, lostShown = false;
uint32_t lastPacketTime = 0;

struct Reading {
  float moisture, temperature, ph;
  uint16_t conductivity, nitrogen, phosphorus, potassium;
  int8_t coffeeClass;
  float confidence;
} value;

const uint8_t BG_R = 5, BG_G = 10, BG_B = 12;
const uint8_t PANEL_R = 43, PANEL_G = 101, PANEL_B = 101;
const uint8_t GREEN_R = 105, GREEN_G = 255, GREEN_B = 55;
const uint8_t YELLOW_R = 255, YELLOW_G = 242, YELLOW_B = 60;

void color(uint8_t r, uint8_t g, uint8_t b) { lcd.setColor(r, g, b); }
void back(uint8_t r, uint8_t g, uint8_t b) { lcd.setBackColor(r, g, b); }
void panel(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b) {
  color(r, g, b);
  lcd.fillRoundRect(x1, y1, x2, y2);
}

void drawLeaf() {
  color(GREEN_R, GREEN_G, GREEN_B);
  lcd.fillCircle(23, 22, 11);
  color(BG_R, BG_G, BG_B);
  lcd.drawLine(16, 29, 31, 14);
  lcd.drawLine(18, 25, 25, 25);
}

void drawLink(const char *state, uint8_t r, uint8_t g, uint8_t b) {
  panel(320, 240, 471, 284, r, g, b);
  back(r, g, b);
  color(0, 0, 0);
  lcd.setFont(SmallFont);
  lcd.print("ESP32 LINK", 356, 247);
  lcd.print(state, 356, 265);
}

void drawDashboard() {
  color(BG_R, BG_G, BG_B);
  lcd.fillRect(0, 0, 479, 319);
  drawLeaf();
  back(BG_R, BG_G, BG_B);
  color(255, 255, 255);
  lcd.setFont(BigFont);
  lcd.print("SOIL MONITOR", 47, 13);
  lcd.setFont(SmallFont);
  color(150, 170, 170);
  lcd.print("LIVE NPK SENSOR", 350, 17);

  // Large teal information block like the reference display.
  panel(8, 50, 310, 230, PANEL_R, PANEL_G, PANEL_B);
  back(PANEL_R, PANEL_G, PANEL_B);
  color(255, 255, 255);
  lcd.setFont(BigFont);
  lcd.print("MOISTURE", 20, 65);
  lcd.print("TEMPERATURE", 20, 112);
  lcd.print("SOIL pH", 20, 159);
  lcd.print("EC", 20, 206);

  color(GREEN_R, GREEN_G, GREEN_B); lcd.drawRoundRect(320, 50, 471, 103);
  color(YELLOW_R, YELLOW_G, YELLOW_B); lcd.drawRoundRect(320, 112, 471, 165);
  color(60, 200, 255); lcd.drawRoundRect(320, 174, 471, 227);
  back(BG_R, BG_G, BG_B);
  lcd.setFont(SmallFont);
  color(200, 215, 215);
  lcd.print("NITROGEN (N)", 330, 57);
  lcd.print("PHOSPHORUS (P)", 326, 119);
  lcd.print("POTASSIUM (K)", 330, 181);

  panel(8, 240, 310, 284, 35, 75, 75);
  back(35, 75, 75);
  color(255, 255, 255);
  lcd.setFont(SmallFont);
  lcd.print("COFFEE SUITABILITY", 20, 247);
  color(YELLOW_R, YELLOW_G, YELLOW_B);
  lcd.print("WAITING FOR PREDICTION", 20, 265);
  drawLink("WAITING...", YELLOW_R, YELLOW_G, YELLOW_B);

  back(BG_R, BG_G, BG_B);
  color(130, 150, 150);
  lcd.setFont(SmallFont);
  lcd.print("ESP32 + ATMEGA328P SOIL MONITORING SYSTEM", CENTER, 302);
}

void clearValues() {
  color(PANEL_R, PANEL_G, PANEL_B);
  lcd.fillRect(194, 61, 301, 225);
  color(BG_R, BG_G, BG_B);
  lcd.fillRect(326, 72, 464, 96);
  lcd.fillRect(326, 134, 464, 158);
  lcd.fillRect(326, 196, 464, 220);
}

void drawPrediction() {
  uint8_t r = GREEN_R, g = GREEN_G, b = GREEN_B;
  const char *label = "ROBUSTA SUITABLE";
  if (value.coffeeClass == 1) {
    r = 60; g = 200; b = 255;
    label = "ARABICA SUITABLE";
  } else if (value.coffeeClass == 2) {
    r = 255; g = 75; b = 45;
    label = "NOT SUITABLE";
  }

  panel(8, 240, 310, 284, r, g, b);
  back(r, g, b);
  color(0, 0, 0);
  lcd.setFont(SmallFont);
  lcd.print("COFFEE SUITABILITY", 20, 247);
  lcd.print(label, 20, 265);
  lcd.printNumF(value.confidence * 100.0f, 1, 238, 265);
  lcd.print("%", 286, 265);
}

void drawValues() {
  clearValues();
  lcd.setFont(BigFont);
  back(PANEL_R, PANEL_G, PANEL_B);
  color(255, 255, 255);
  lcd.printNumF(value.moisture, 1, 198, 65); lcd.print("%", 286, 65);
  lcd.printNumF(value.temperature, 1, 198, 112); lcd.print("C", 286, 112);
  lcd.printNumF(value.ph, 1, 220, 159);
  lcd.printNumI(value.conductivity, 198, 206);

  back(BG_R, BG_G, BG_B);
  color(GREEN_R, GREEN_G, GREEN_B); lcd.printNumI(value.nitrogen, 337, 76);
  color(YELLOW_R, YELLOW_G, YELLOW_B); lcd.printNumI(value.phosphorus, 337, 138);
  color(60, 200, 255); lcd.printNumI(value.potassium, 337, 200);
  lcd.setFont(SmallFont);
  color(205, 215, 215);
  lcd.print("uS/cm", 254, 214);
  lcd.print("mg/kg", 416, 83);
  lcd.print("mg/kg", 416, 145);
  lcd.print("mg/kg", 416, 207);
  drawPrediction();
}

bool unsignedValue(const char *text, uint16_t &result) {
  char *end;
  unsigned long parsed = strtoul(text, &end, 10);
  if (!*text || *end || parsed > 65535UL) return false;
  result = parsed;
  return true;
}

bool parseReading(char *text) {
  char *field[9];
  uint8_t count = 0;
  char *part = strtok(text, ",");
  while (part && count < 9) { field[count++] = part; part = strtok(NULL, ","); }
  if (count != 9 || part) return false;

  Reading next;
  char *end;
  next.moisture = strtod(field[0], &end); if (*end) return false;
  next.temperature = strtod(field[1], &end); if (*end) return false;
  if (!unsignedValue(field[2], next.conductivity)) return false;
  next.ph = strtod(field[3], &end); if (*end) return false;
  if (!unsignedValue(field[4], next.nitrogen) ||
      !unsignedValue(field[5], next.phosphorus) ||
      !unsignedValue(field[6], next.potassium)) return false;
  long classId = strtol(field[7], &end, 10);
  if (*end || classId < 0 || classId > 2) return false;
  next.coffeeClass = static_cast<int8_t>(classId);
  next.confidence = strtod(field[8], &end); if (*end) return false;
  if (next.moisture < 0 || next.moisture > 100 ||
      next.temperature < -40 || next.temperature > 125 ||
      next.ph < 0 || next.ph > 14 ||
      next.confidence < 0 || next.confidence > 1) return false;
  value = next;
  return true;
}

void finishPacket() {
  packet[packetLength] = '\0';
  if (parseReading(packet)) {
    dataReceived = true;
    lostShown = false;
    lastPacketTime = millis();
    drawValues();
    drawLink("DATA LIVE", GREEN_R, GREEN_G, GREEN_B);
  }
}

void readESP32() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '<') { receiving = true; packetLength = 0; }
    else if (c == '>' && receiving) {
      receiving = false;
      finishPacket();
      packetLength = 0;
    } else if (receiving) {
      if (packetLength < PACKET_SIZE - 1) packet[packetLength++] = c;
      else { receiving = false; packetLength = 0; }
    }
  }
}

void setup() {
  Serial.begin(9600);
  lcd.InitLCD();
  lcd.setFont(SmallFont);
  drawDashboard();
  while (Serial.available()) Serial.read();
}

void loop() {
  readESP32();
  if (dataReceived && millis() - lastPacketTime > LINK_TIMEOUT_MS && !lostShown) {
    drawLink("NO DATA", 255, 75, 45);
    lostShown = true;
  }
}
#endif

#include <UTFTGLUE.h>
#include <stdlib.h>
#include <string.h>

// Existing known-working parallel TFT constructor and pins are unchanged.
UTFTGLUE lcd(0, A2, A1, A3, A4, A0);
constexpr uint8_t PACKET_SIZE = 96, HISTORY_SIZE = 5, SCREEN_COUNT = 4;
constexpr uint32_t LINK_TIMEOUT_MS = 10000;
enum Screen : uint8_t { HOME, ANALYSIS, HISTORY, SETTINGS };

struct Reading {
  float moisture, temperature, ph, confidence;
  uint16_t conductivity, nitrogen, phosphorus, potassium;
  int8_t coffeeClass;
} reading;
struct HistoryItem {
  uint16_t n, p, k; uint8_t ph10; int8_t coffeeClass;
} history[HISTORY_SIZE];

char packet[PACKET_SIZE];
uint8_t packetLength = 0, historyCount = 0, historyHead = 0;
uint8_t selectedScreen = HOME, activeScreen = HOME;
bool receiving = false, hasData = false, linkLost = false;
uint32_t lastPacketAt = 0;

const uint8_t BG_R=13, BG_G=25, BG_B=38, PANEL_R=39, PANEL_G=56, PANEL_B=76;
const uint8_t GREEN_R=100, GREEN_G=205, GREEN_B=120, MUTED_R=155, MUTED_G=170, MUTED_B=180;

void color(uint8_t r,uint8_t g,uint8_t b){lcd.setColor(r,g,b);}
void back(uint8_t r,uint8_t g,uint8_t b){lcd.setBackColor(r,g,b);}
void fill(int x1,int y1,int x2,int y2,uint8_t r,uint8_t g,uint8_t b){color(r,g,b);lcd.fillRect(x1,y1,x2,y2);}
void panel(int x1,int y1,int x2,int y2){color(PANEL_R,PANEL_G,PANEL_B);lcd.fillRoundRect(x1,y1,x2,y2);}
const char* titleFor(uint8_t page){
  static const char* titles[]={"COFFEE SOIL ANALYZER","SOIL ANALYSIS","RECENT MEASUREMENTS","SYSTEM INFORMATION"};
  return titles[page<SCREEN_COUNT?page:0];
}
const char* resultName(int8_t id){return id==0?"SUITABLE FOR ROBUSTA":id==1?"SUITABLE FOR ARABICA":"NOT SUITABLE";}

void drawHeader(){
  fill(0,0,479,43,BG_R,BG_G,BG_B); color(GREEN_R,GREEN_G,GREEN_B); lcd.fillCircle(20,20,8);
  lcd.drawLine(19,28,19,34); back(BG_R,BG_G,BG_B); color(245,245,245);
  lcd.setFont(BigFont); lcd.print(titleFor(activeScreen),38,10); lcd.setFont(SmallFont);
  if(hasData&&!linkLost) color(GREEN_R,GREEN_G,GREEN_B); else color(245,85,65);
  lcd.drawCircle(414,18,3);lcd.drawCircle(414,18,7);lcd.fillCircle(414,18,1);
  color(205,215,220);lcd.print("ESP32",430,14);
}

void drawNavIcon(uint8_t icon,int cx,uint8_t r,uint8_t g,uint8_t b){
  color(r,g,b);
  if(icon==HOME){lcd.drawLine(cx-8,285,cx,278);lcd.drawLine(cx,278,cx+8,285);lcd.drawRect(cx-6,285,cx+6,294);}
  else if(icon==ANALYSIS){lcd.fillRect(cx-8,286,cx-5,294);lcd.fillRect(cx-2,281,cx+1,294);lcd.fillRect(cx+4,284,cx+7,294);}
  else if(icon==HISTORY){lcd.drawCircle(cx,286,8);lcd.drawLine(cx,286,cx,280);lcd.drawLine(cx,286,cx+5,289);}
  else{lcd.drawCircle(cx,286,7);lcd.drawCircle(cx,286,2);}
}
void drawNavigation(){
  static const char* labels[]={"HOME","ANALYSIS","HISTORY","SETTINGS"};
  for(uint8_t i=0;i<SCREEN_COUNT;++i){
    int x=3+i*119;
    if(i==selectedScreen){fill(x,270,x+115,318,GREEN_R,GREEN_G,GREEN_B);back(GREEN_R,GREEN_G,GREEN_B);drawNavIcon(i,x+57,10,25,25);color(10,25,25);}
    else{fill(x,270,x+115,318,PANEL_R,PANEL_G,PANEL_B);back(PANEL_R,PANEL_G,PANEL_B);drawNavIcon(i,x+57,210,215,220);color(210,215,220);}
    lcd.setFont(SmallFont);lcd.print(labels[i],x+28,303);
  }
}

void metric(int x,const char* label,uint16_t value,const char* unit){
  panel(x,52,x+108,119);back(PANEL_R,PANEL_G,PANEL_B);color(MUTED_R,MUTED_G,MUTED_B);
  lcd.setFont(SmallFont);lcd.print(label,x+7,59);color(245,245,245);lcd.setFont(BigFont);
  if(hasData)lcd.printNumI(value,x+9,79);else lcd.print("--",x+9,79);
  lcd.setFont(SmallFont);color(MUTED_R,MUTED_G,MUTED_B);lcd.print(unit,x+62,99);
}
void phMetric(int x){
  panel(x,52,x+108,119);back(PANEL_R,PANEL_G,PANEL_B);color(MUTED_R,MUTED_G,MUTED_B);
  lcd.setFont(SmallFont);lcd.print("pH",x+46,59);color(245,245,245);lcd.setFont(BigFont);
  if(hasData)lcd.printNumF(reading.ph,1,x+30,79);else lcd.print("--",x+30,79);
  lcd.setFont(SmallFont);color(MUTED_R,MUTED_G,MUTED_B);lcd.print("pH",x+46,99);
}

void predictionBar(const char* label,uint8_t percent,int y,bool winner){
  back(PANEL_R,PANEL_G,PANEL_B);color(220,225,230);lcd.setFont(SmallFont);lcd.print(label,20,y);
  color(67,83,101);lcd.fillRoundRect(114,y+2,415,y+12);
  if(percent){color(winner?GREEN_R:130,winner?GREEN_G:155,winner?GREEN_B:180);lcd.fillRoundRect(114,y+2,114+(percent*301L)/100,y+12);}
  back(PANEL_R,PANEL_G,PANEL_B);color(240,245,245);lcd.printNumI(percent,426,y);lcd.print("%",454,y);
}
void drawHomeData(){
  metric(8,"N",reading.nitrogen,"mg/kg");metric(126,"P",reading.phosphorus,"mg/kg");
  metric(244,"K",reading.potassium,"mg/kg");phMetric(362);
  panel(8,130,471,264);back(PANEL_R,PANEL_G,PANEL_B);color(205,215,220);lcd.setFont(SmallFont);lcd.print("ML PREDICTION",20,141);
  if(hasData&&reading.coffeeClass!=2)color(GREEN_R,GREEN_G,GREEN_B);else color(245,85,65);
  lcd.setFont(BigFont);lcd.print(hasData?resultName(reading.coffeeClass):"WAITING FOR DATA",20,161);
  uint8_t scores[3]={0,0,0};
  if(hasData){uint8_t win=uint8_t(reading.confidence*100+0.5f);scores[reading.coffeeClass]=win;uint8_t rest=(100-win)/2;
    for(uint8_t i=0;i<3;++i)if(i!=reading.coffeeClass)scores[i]=rest;}
  predictionBar("Robusta",scores[0],195,hasData&&reading.coffeeClass==0);
  predictionBar("Arabica",scores[1],216,hasData&&reading.coffeeClass==1);
  predictionBar("Not Suitable",scores[2],237,hasData&&reading.coffeeClass==2);
}
void drawHome(){
  fill(0,0,479,319,BG_R,BG_G,BG_B);drawHeader();drawHomeData();drawNavigation();
}

const char* level(uint16_t v,uint16_t lo,uint16_t hi){return v<lo?"LOW":v>hi?"HIGH":"OK";}
void drawAnalysisData(){
  panel(8,51,232,132);panel(247,51,471,132);
  panel(8,143,232,264);panel(247,143,471,264);back(PANEL_R,PANEL_G,PANEL_B);lcd.setFont(SmallFont);
  color(MUTED_R,MUTED_G,MUTED_B);lcd.print("MOISTURE",20,61);lcd.print("TEMPERATURE",259,61);
  lcd.print("SOIL CHEMISTRY",20,153);lcd.print("NUTRIENT FLAGS",259,153);color(245,245,245);lcd.setFont(BigFont);
  if(hasData){lcd.printNumF(reading.moisture,1,20,88);lcd.print("%",110,88);lcd.printNumF(reading.temperature,1,259,88);lcd.print("C",349,88);
    lcd.setFont(SmallFont);lcd.print("pH",20,181);lcd.printNumF(reading.ph,1,85,181);lcd.print("EC",20,211);
    lcd.printNumI(reading.conductivity,85,211);lcd.print("uS/cm",142,211);
    lcd.print("N",259,181);lcd.print(level(reading.nitrogen,20,200),329,181);
    lcd.print("P",259,211);lcd.print(level(reading.phosphorus,10,100),329,211);
    lcd.print("K",259,241);lcd.print(level(reading.potassium,40,250),329,241);
  }else{lcd.print("--",20,88);lcd.print("--",259,88);}
}
void drawAnalysis(){
  fill(0,0,479,319,BG_R,BG_G,BG_B);drawHeader();drawAnalysisData();drawNavigation();
}

void saveHistory(){
  HistoryItem &h=history[historyHead];h.n=reading.nitrogen;h.p=reading.phosphorus;h.k=reading.potassium;
  h.ph10=uint8_t(reading.ph*10+0.5f);h.coffeeClass=reading.coffeeClass;
  historyHead=(historyHead+1)%HISTORY_SIZE;if(historyCount<HISTORY_SIZE)++historyCount;
}
void drawHistory(){
  fill(0,0,479,319,BG_R,BG_G,BG_B);drawHeader();back(BG_R,BG_G,BG_B);color(MUTED_R,MUTED_G,MUTED_B);lcd.setFont(SmallFont);
  lcd.print("#",14,53);lcd.print("N",55,53);lcd.print("P",112,53);lcd.print("K",169,53);lcd.print("pH",226,53);lcd.print("RESULT",286,53);
  for(uint8_t row=0;row<historyCount;++row){uint8_t i=(historyHead+HISTORY_SIZE-1-row)%HISTORY_SIZE;int y=76+row*37;
    fill(8,y-7,471,y+23,PANEL_R,PANEL_G,PANEL_B);back(PANEL_R,PANEL_G,PANEL_B);color(240,245,245);
    lcd.printNumI(row+1,14,y);lcd.printNumI(history[i].n,55,y);lcd.printNumI(history[i].p,112,y);lcd.printNumI(history[i].k,169,y);
    lcd.printNumF(history[i].ph10/10.0f,1,226,y);lcd.print(history[i].coffeeClass==0?"ROBUSTA":history[i].coffeeClass==1?"ARABICA":"UNSUITABLE",286,y);}
  if(!historyCount)lcd.print("NO MEASUREMENTS RECEIVED YET",CENTER,145);drawNavigation();
}

void drawSettings(){
  fill(0,0,479,319,BG_R,BG_G,BG_B);drawHeader();panel(8,51,471,131);panel(8,142,471,264);
  back(PANEL_R,PANEL_G,PANEL_B);lcd.setFont(SmallFont);color(MUTED_R,MUTED_G,MUTED_B);lcd.print("SERIAL LINK",20,61);color(245,245,245);
  lcd.print("TX25 -> AVR RX",20,84);lcd.print("RX26 <- AVR TX",20,105);
  color(MUTED_R,MUTED_G,MUTED_B);lcd.print("CONTROLS",20,153);color(245,245,245);
  lcd.print("CLK 18   DT 19   SW 23",20,184);lcd.print("Sample 3s   UART 9600",20,218);drawNavigation();
}
void drawScreen(){if(activeScreen==HOME)drawHome();else if(activeScreen==ANALYSIS)drawAnalysis();else if(activeScreen==HISTORY)drawHistory();else drawSettings();}

bool parseUInt(const char* s,uint16_t &v){char* e;unsigned long n=strtoul(s,&e,10);if(!*s||*e||n>65535)return false;v=n;return true;}
bool parseData(){
  char* f[9];uint8_t n=0;char* p=strtok(NULL,",");while(p&&n<9){f[n++]=p;p=strtok(NULL,",");}if(n!=9||p)return false;
  Reading next;char* e;next.moisture=strtod(f[0],&e);if(*e)return false;next.temperature=strtod(f[1],&e);if(*e)return false;
  if(!parseUInt(f[2],next.conductivity))return false;next.ph=strtod(f[3],&e);if(*e)return false;
  if(!parseUInt(f[4],next.nitrogen)||!parseUInt(f[5],next.phosphorus)||!parseUInt(f[6],next.potassium))return false;
  long c=strtol(f[7],&e,10);if(*e||c<0||c>2)return false;next.coffeeClass=c;next.confidence=strtod(f[8],&e);
  if(*e||next.moisture<0||next.moisture>100||next.temperature< -40||next.temperature>125||next.ph<0||next.ph>14||next.confidence<0||next.confidence>1)return false;
  reading=next;return true;
}
void handlePacket(){
  packet[packetLength]=0;char* type=strtok(packet,",");if(!type)return;
  if(!strcmp(type,"DATA")){if(!parseData())return;
    bool first=!hasData,wasLost=linkLost;hasData=true;linkLost=false;lastPacketAt=millis();saveHistory();
    if(first)drawScreen();else{if(wasLost)drawHeader();if(activeScreen==HOME)drawHomeData();else if(activeScreen==ANALYSIS)drawAnalysisData();}}
  else if(!strcmp(type,"NAV")){char* s=strtok(NULL,",");char* a=strtok(NULL,",");if(!s||!a)return;uint8_t ns=atoi(s),na=atoi(a);
    if(ns>=SCREEN_COUNT||na>=SCREEN_COUNT)return;bool changed=na!=activeScreen;selectedScreen=ns;activeScreen=na;if(changed)drawScreen();else drawNavigation();}
}
void readESP32(){while(Serial.available()){char c=Serial.read();if(c=='<'){receiving=true;packetLength=0;}
  else if(c=='>'&&receiving){receiving=false;handlePacket();packetLength=0;}
  else if(receiving){if(packetLength<PACKET_SIZE-1)packet[packetLength++]=c;else{receiving=false;packetLength=0;}}}}

void setup(){Serial.begin(9600);lcd.InitLCD();lcd.setFont(SmallFont);drawScreen();while(Serial.available())Serial.read();}
void loop(){
  readESP32();
  if(hasData&&millis()-lastPacketAt>LINK_TIMEOUT_MS&&!linkLost){linkLost=true;drawHeader();}
}
