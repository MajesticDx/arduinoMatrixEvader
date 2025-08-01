/// MAX7219 initialisation
void Init_MAX7219(void)
{
    send_data(0x09, 0x00);         // Decoding off (0xAddr Data)
    send_data(0x0A, 0x08);         // Brightness to intermediate
    send_data(0x0B, 0x07);         // Scan limit = 7
    send_data(0x0C, 0x01);         // Normal operation mode
    send_data(0x0F, 0x0F);       // Enable display test
    delay(500); // 500 ms delay
    send_data(0x0F,0x00);         // Disable display test
    send_data(0x01,0x00);         // Clear row 0.
    send_data(0x02,0x00);         // Clear row 1.
    send_data(0x03,0x00);         // Clear row 2.
    send_data(0x04,0x00);         // Clear row 3.
    send_data(0x05,0x00);         // Clear row 4.
    send_data(0x06,0x00);         // Clear row 5.
    send_data(0x07,0x00);         // Clear row 6.
    send_data(0x08,0x00);         // Clear row 7.

    delay(500); // 500 ms delay
}


void send_data (unsigned char MSB, unsigned char LSB)
{
  digitalWrite(CS, LOW);
  SPI.transfer(MSB);
  SPI.transfer(LSB);
  digitalWrite(CS, HIGH);
}

void for_loop (unsigned char array[]) {
  for(int adr=1; adr<9; adr++) {
    send_data(adr, array[adr - 1]);
  }
}

void overlapcheck() {
  for(int adr=0; adr<8; adr++) {
    if (snake[adr] != 0x00) {
      if (int(blocks[adr] | snake[adr]) != int(snake[adr] + blocks[adr])) {
        lose = true;
      }
    }
  }
}

void create_canvas() {
  for(int adr=0; adr<8; adr++) {
    finished_canvas[adr] = blocks[adr] | snake[adr];
  }
}

