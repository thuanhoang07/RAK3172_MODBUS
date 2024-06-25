#include <Rak3172_Canopus.h>
#include "Canopus_Modbus.h"

// Khai báo đối tượng ModbusMaster
ModbusMaster node;

uint8_t result_analog;
uint8_t result_digital_in;


void setup() {
  Serial.begin(115200);
  Serial.print("\r\n*****************RAK3172_CANOPUS*******************");
  
  // Khởi động Serial cho RS485
  Serial_Canopus.begin(9600, SERIAL_8N1);
  
  // Khởi tạo I/O và kích hoạt RS485
  init_io();
  enable_Vrs485();
}

void loop() {
  // Thiết lập ID slave cho node Modbus
  node.begin(1, Serial_Canopus); // ID slave là 1
  
  Serial.printf("\r\n\n\nExample read modbus RTU for RAK3172_Canopus board");
  // Đọc dữ liệu từ thanh ghi 40000 đến 40005 (analog inputs)
  result_analog = node.readHoldingRegisters(0, 6); // Đọc từ 40000 đến 40005
  delay(10);
  if (result_analog == node.ku8MBSuccess) { // Nếu đọc thành công
    for (uint8_t i = 0; i < 6; i++) {
      uint16_t rawValue = node.getResponseBuffer(i);
      float current = (rawValue * 20.0) / 4095.0;     //value of current!
      Serial.printf("\r\nValue 4000%d: %d -> Current: %.2f mA", i, rawValue, current);
    }
  } else {
    Serial.printf("\nRead Fail node 1"); // Nếu đọc thất bại
  }

  
  result_digital_in = node.readDiscreteInputs(0x0000, 4); // Read input status from address 0x0000
  delay(10);
  if (result_digital_in == node.ku8MBSuccess) {
    uint16_t inputStatus = node.getResponseBuffer(0);
    Serial.printf("\nReceived Data: 0x%04X", inputStatus);
    for (uint8_t i = 0; i < 4; i++) {
      bool state = (inputStatus >> i) & 0x01;
      Serial.printf("\nDI_%02d: %s", i + 1, state ? "ON" : "OFF"); // active low
    }
  } else {
    Serial.printf("\nRead Fail, Error: %02X", result_digital_in); // If reading fails
  }
  
  delay(500);
}
