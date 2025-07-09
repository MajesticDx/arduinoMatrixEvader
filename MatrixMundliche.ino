#include <Esprit.h>
#include <WiFi.h>
#include <FreematicsOLED.h>
#include <WebServer.h>
#include <SPI.h>
#include <Bluepad32.h>

#define CS  5

WebServer server (80);
ControllerPtr myControllers[BP32_MAX_GAMEPADS];
hw_timer_t *timer = NULL;
OLED_SH1106 OLED;

String ssid     = YOUR WIFI NAME HERE
String password = YOUR WIFI PASSWORD HERE

volatile bool sprint = false;
volatile bool links = false;
volatile bool rechts = false;
volatile bool oben = false;
volatile bool unten = false;
volatile bool controllerUpdateFlag = false;

volatile bool toggle = false;
String loseInd = "Ingame";
volatile int highscore = 0;

unsigned char snake[] = {0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
unsigned char blocks[] = {0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
unsigned char finished_canvas[] = {0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};

unsigned char smallT_RtoL[][8] = { ///ID 0
  {0xE0,0,0,0,0,0,0,0},
  {0x40,0xE0,0,0,0,0,0,0},
  {0,0x40,0xE0,0,0,0,0,0},
  {0,0,0x40,0xE0,0,0,0,0},
  {0,0,0,0x40,0xE0,0,0,0},
  {0,0,0,0,0x40,0xE0,0,0},
  {0,0,0,0,0,0x40,0xE0,0},
  {0,0,0,0,0,0,0x40,0xE0},
  {0,0,0,0,0,0,0,0x40}        
};

unsigned char Rec_TtoB[][8] = { ///ID 1
  {0,0,0x80,0x80,0,0,0,0},
  {0,0,0xC0,0xC0,0,0,0,0},  
  {0,0,0xE0,0xE0,0,0,0,0},  
  {0,0,0x70,0x70,0,0,0,0},  
  {0,0,0x38,0x38,0,0,0,0},  
  {0,0,0x1C,0x1C,0,0,0,0},  
  {0,0,0x0E,0x0E,0,0,0,0},  
  {0,0,0x07,0x07,0,0,0,0},       
  {0,0,0x03,0x03,0,0,0,0},
  {0,0,0x01,0x01,0,0,0,0}
};

unsigned char smallL_LtoR[][8] = { ///ID 2
  {0,0,0,0,0,0,0,0x01},
  {0,0,0,0,0,0,0x01,0x07},  
  {0,0,0,0,0,0x01,0x07,0}, 
  {0,0,0,0,0x01,0x07,0,0}, 
  {0,0,0,0x01,0x07,0,0,0},
  {0,0,0x01,0x07,0,0,0,0},  
  {0,0x01,0x07,0,0,0,0,0}, 
  {0x01,0x07,0,0,0,0,0,0},
  {0x07,0,0,0,0,0,0,0} 
};

unsigned char vp1_BtoT[][8] = { ///ID 3 vierplus1 
  {0,0,0,0,0x01,0x01,0x01,0},
  {0,0,0,0,0x02,0x03,0x03,0},
  {0,0,0,0,0x04,0x06,0x06,0},
  {0,0,0,0,0x08,0x0C,0x0C,0},
  {0,0,0,0,0x10,0x18,0x18,0},
  {0,0,0,0,0x20,0x30,0x30,0},
  {0,0,0,0,0x40,0x60,0x60,0},
  {0,0,0,0,0x80,0xC0,0xC0,0},
  {0,0,0,0,0,0x80,0x80,0}
};

unsigned char cross_LtoR[][8] = { ///ID 4
  {0,0,0,0,0,0,0,0x10},
  {0,0,0,0,0,0,0x10,0x38},
  {0,0,0,0,0,0x10,0x38,0x10},
  {0,0,0,0,0x10,0x38,0x10,0},
  {0,0,0,0x10,0x38,0x10,0,0},
  {0,0,0x10,0x38,0x10,0,0,0},
  {0,0x10,0x38,0x10,0,0,0,0},
  {0x10,0x38,0x10,0,0,0,0,0},
  {0x38,0x10,0,0,0,0,0,0},
  {0x10,0,0,0,0,0,0,0}
};

unsigned char trcht_BtoT[][8] = { //ID 5
  {0,0x01,0,0x01,0,0,0,0},
  {0,0x03,0x01,0x03,0,0,0,0},
  {0,0x06,0x03,0x06,0,0,0,0},
  {0,0x0C,0x06,0x0C,0,0,0,0},
  {0,0x18,0x0C,0x18,0,0,0,0},
  {0,0x30,0x18,0x30,0,0,0,0},
  {0,0x60,0x30,0x60,0,0,0,0},
  {0,0xC0,0x60,0xC0,0,0,0,0},
  {0,0x80,0xC0,0x80,0,0,0,0},
  {0,0,0x80,0,0,0,0,0}
};

unsigned char LL_RtoL[][8] = { //ID 6
  {0x06,0,0,0,0,0,0,0},
  {0x04,0x06,0,0,0,0,0,0},
  {0x0C,0x04,0x06,0,0,0,0,0},
  {0,0x0C,0x04,0x06,0,0,0,0},
  {0,0,0x0C,0x04,0x06,0,0,0},
  {0,0,0,0x0C,0x04,0x06,0,0},
  {0,0,0,0,0x0C,0x04,0x06,0},
  {0,0,0,0,0,0x0C,0x04,0x06},
  {0,0,0,0,0,0,0x0C,0x04},
  {0,0,0,0,0,0,0,0x0C}

};

unsigned char hllwV_TtoB[][8] = { //ID 7
  {0,0,0,0,0x80,0x80,0x80,0},
  {0,0,0,0,0xC0,0x40,0xC0,0},
  {0,0,0,0,0xE0,0xA0,0xE0,0},
  {0,0,0,0,0x70,0x50,0x70,0},
  {0,0,0,0,0x38,0x28,0x38,0},
  {0,0,0,0,0x1C,0x14,0x1C,0},
  {0,0,0,0,0x0E,0x0A,0x0E,0},
  {0,0,0,0,0x07,0x05,0x07,0},
  {0,0,0,0,0x03,0x02,0x03,0},
  {0,0,0,0,0x01,0x01,0x01,0}
};

volatile int activeBase1 = 10;
volatile int activeBase2 = 60;
volatile int activeBase3 = 110;
volatile int activeBase4 = 160;
volatile int activeBase5 = 30;
volatile int activeBase6 = 130;
volatile int activeBase7 = 80;
volatile int activeBase8 = 170;

unsigned char skull[] = {0x19, 0x3A, 0x6F, 0xFE, 0xFE, 0x6F, 0x3A, 0x19};
unsigned char victory[] = {0x78, 0xFC, 0xFE, 0x7F, 0x7F, 0xFE, 0xFC, 0x78};

volatile int stateX = 0;
volatile int SCNDstateX = 1;
volatile int LASTstateX = 7;

volatile bool lose = false;
volatile bool win = false;

volatile int count_till_move = 0;
volatile int activation_counter = 0;
unsigned char active_blocks[] = {false, false, false, false, false, false, false, false};
unsigned char moving_blocks_state[] = {0, 0, 0, 0, 0, 0, 0, 0};

///////////// PS4 FUNKTIONEN
void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}
void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }
    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
} 
void dumpGamepad(ControllerPtr ctl) {
    Serial.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
        "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
        ctl->index(),        // Controller Index
        ctl->dpad(),         // D-pad
        ctl->buttons(),      // bitmask of pressed buttons
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->brake(),        // (0 - 1023): brake button
        ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
        ctl->miscButtons(),  // bitmask of pressed "misc" buttons
        ctl->gyroX(),        // Gyro X
        ctl->gyroY(),        // Gyro Y
        ctl->gyroZ(),        // Gyro Z
        ctl->accelX(),       // Accelerometer X
        ctl->accelY(),       // Accelerometer Y
        ctl->accelZ()        // Accelerometer Z
    );
}
void processGamepad(ControllerPtr ctl) { //update
  
    if (ctl->buttons() == 0x0001) {
      Serial.println("RESET");
      snake[0] = 0x01;
      snake[1] = 0x00;
      snake[2] = 0x00;
      snake[3] = 0x00;
      snake[4] = 0x00;
      snake[5] = 0x00;
      snake[6] = 0x00;
      snake[7] = 0x00;
      for(int adr=0; adr<8; adr++) {
        blocks[adr] = 0;
        finished_canvas[adr] = 0;
      }
      stateX = 0;
      SCNDstateX = 1;
      LASTstateX = 7;
      lose = false;
      win = false;
      count_till_move = 0;
      activation_counter = 0;
      for (int i = 0; i < 8; i++) {
        active_blocks[i] = false;
        moving_blocks_state[i] = 0;
      } 
      activeBase1 = 10;
      activeBase2 = 60;
      activeBase3 = 110;
      activeBase4 = 160;
      activeBase5 = 30;
      activeBase6 = 130;
      activeBase7 = 80;
      activeBase8 = 170;
    }

    if (ctl->buttons() == 0x0080) {
        Serial.println("SPRINT");
        ctl->playDualRumble(0 /* delayedStartMs */, 250 /* durationMs */, 0x80 /* weakMagnitude */,0x40 /* strongMagnitude */);
        ctl->setColorLED(255, 0, 0);
        sprint = true;
    } else {
      ctl->setColorLED(0, 0, 255);
      sprint = false;
    }

    if (ctl->axisX() <= -300) {
      Serial.println("LINKS");
      links = true;
    } else {
      links = false;
    }
    if (ctl->axisX() >= 300) {
      Serial.println("RECHTS");
      rechts = true;
    } else {
      rechts = false;
    }
    if (ctl->axisY() <= -300) {
      Serial.println("OBEN");
      oben = true;
    } else {
      oben = false;
    }
    if (ctl->axisY() >= 300) {
      Serial.println("UNTEN");
      unten = true;
    } else {
      unten = false;
    }
}
void processControllers() {
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData()) {
            if (myController->isGamepad()) {
                processGamepad(myController);
            }
        }
    }
}
///////////// PS4 FUNKTIONEN ENDE

void updateSnake() {
  if (toggle || sprint) {
    if (links) {
      snake[SCNDstateX] = snake[stateX];
      snake[stateX] = 0;

      stateX++;
      if (stateX > 7) stateX = 0;

      SCNDstateX = stateX + 1;
      if (SCNDstateX > 7) SCNDstateX = 0;

      LASTstateX = stateX - 1;
      if (LASTstateX < 0) LASTstateX = 7;
    }
    
    if (rechts) {
      snake[LASTstateX] = snake[stateX];
      snake[stateX] = 0;

      stateX--;
      if (stateX < 0) stateX = 7;

      SCNDstateX = stateX + 1;
      if (SCNDstateX > 7) SCNDstateX = 0;

      LASTstateX = stateX - 1;
      if (LASTstateX < 0) LASTstateX = 7;

    }
    
    if (oben) {
      snake[stateX] <<= 1;
      if (snake[stateX] == 0x00) snake[stateX] = 0x01;

    }
    
    if (unten) {
      snake[stateX] >>= 1;
      if (snake[stateX] == 0x00) snake[stateX] = 0x80;
    }
  }
}

void handel_activations() { //update
  if (activation_counter == activeBase1) {
    active_blocks[0] = true;
    activeBase1 += 110;
  }
  if (activation_counter == activeBase2) {
    active_blocks[1] = true;
    activeBase2 += 130;
  }
  if (activation_counter == activeBase3) {
    active_blocks[2] = true;
    activeBase3 += 120;
  }
  if (activation_counter == activeBase4) {
    active_blocks[3] = true;
    activeBase4 += 140;
  }
  if (activation_counter == activeBase5) {
    active_blocks[4] = true;
    activeBase5 += 150;
  }
  if (activation_counter == activeBase6) {
    active_blocks[5] = true;
    activeBase6 += 160;
  }
  if (activation_counter == activeBase7) {
    active_blocks[6] = true;
    activeBase7 += 170;
  }
  if (activation_counter == activeBase8) {
    active_blocks[7] = true;
    activeBase8 += 180;
  }
}
void update_active() { //update
  if (count_till_move == 9) {
    for(int i=0; i<8; i++) {
      if (active_blocks[i] == true) {
        moving_blocks_state[i]++;
      }
      switch(i) {
        case 0:
        case 2:
        case 3:
          if (moving_blocks_state[i] >= 9) {
            active_blocks[i] = false;
            moving_blocks_state[i] = 0;
          }
          break;
        case 1:
        case 4:
        case 5:
        case 6:
        case 7:
          if (moving_blocks_state[i] >= 10) {
            active_blocks[i] = false;
            moving_blocks_state[i] = 0;
          }
          break;
      }
    }
  }
}

void process_active() { //update
  for (int i = 0; i < 8; i++) {
    blocks[i] = 0x00;
  } 

  for(int i=0; i<8; i++) {
    if (active_blocks[i] == true) {
      switch(i) {
        case 0:
          for(int adr=0; adr<8; adr++) {
            int id = moving_blocks_state[i];
            blocks[adr] = smallT_RtoL[id][adr] | blocks[adr];
          }
          break;
        case 1:
          for(int adr=0; adr<8; adr++) {
            int id = moving_blocks_state[i];
            blocks[adr] = Rec_TtoB[id][adr] | blocks[adr];
          }
          break;
        case 2:
          for(int adr=0; adr<8; adr++) {
            int id = moving_blocks_state[i];
            blocks[adr] = smallL_LtoR[id][adr] | blocks[adr];
          }
          break;
        case 3:
          for(int adr=0; adr<8; adr++) {
            int id = moving_blocks_state[i];
            blocks[adr] = vp1_BtoT[id][adr] | blocks[adr];
          }
          break;
        case 4:
          for(int adr=0; adr<8; adr++) {
            int id = moving_blocks_state[i];
            blocks[adr] = cross_LtoR[id][adr] | blocks[adr];
          }
          break;
        case 5:
          for(int adr=0; adr<8; adr++) {
            int id = moving_blocks_state[i];
            blocks[adr] = trcht_BtoT[id][adr] | blocks[adr];
          }
          break;
        case 6:
          for(int adr=0; adr<8; adr++) {
            int id = moving_blocks_state[i];
            blocks[adr] = LL_RtoL[id][adr] | blocks[adr];
          }
          break;
        case 7:
          for(int adr=0; adr<8; adr++) {
            int id = moving_blocks_state[i];
            blocks[adr] = hllwV_TtoB[id][adr] | blocks[adr];
          }
          break;
      }
    }
  }
}

void setup() {
  pinMode(CS, OUTPUT);

  digitalWrite(CS, HIGH);
  SPI.begin(18, -1, 23, CS);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  SPI.setDataMode(SPI_MODE0);
  delay(10);
  Init_MAX7219(); 

  timer = timerBegin(0, 80000, true);
  timerAlarmWrite(timer, 750, true);
  timerAttachInterrupt(timer, &isr_Timer2, true);

  ///PS4
  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);
  ///PS4 ENDE
  Serial.println("Ende Setup");

  WiFi.begin(ssid.c_str(), password.c_str());
  OLED_LCD("wifi...");
  //connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print("connection...");
    OLED_LCD("connection...");
  }
  OLED_LCD("connected"); 

  server.on ("/", handleRoot );  // Hier wird die Anfrage "gaheandelt"
  server.begin();                // Starte Web-Server
}

void isr_Timer2 (void) {
  toggle = !toggle;
  controllerUpdateFlag = true;
  if (!lose) {
      count_till_move++;
      activation_counter++;
  }
}

void loop() {
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print("connection...");
    OLED_LCD("connection...");
    delay(500);
  }

  if (controllerUpdateFlag) {
    controllerUpdateFlag = false;

    handel_activations();
    update_active();
    bool dataUpdated = BP32.update();
    if (dataUpdated)
      processControllers();
    updateSnake();
  }

  if (count_till_move >= 10) {
    count_till_move = 0;
  }

  if (win) {
    for_loop(victory);
  } else if (!lose) {
    process_active();

    overlapcheck();
    create_canvas();
    for_loop(finished_canvas);

  } else {
    for_loop(skull);
    if (activation_counter > highscore) {
      highscore = activation_counter;
    }

  }

  server.handleClient();  //Wartet auf Client Anfragen zum Verarbeiten => request / response
}
