#include <Wire.h
#define ACMadd 0x68
#define MAGadd 0x0C
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
  Wire.requestFrom(Address, Nbytes);
  uint8_t index = 0;
  while (Wire.available())
    Data[index++] = Wire.read();
}
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}
void setup()
{
  Wire.begin();         //0x00 0x08 0x10 0x18
  Wire.beginTransmission(MPUadd);
  Wire.write(28);
  
  I2CwriteByte(MPUadd,28,0x18);// 2g 4g 8g 16g
  I2CwriteByte(MPUadd,27,0x18);// 250dps 500dps 1000dps 2000dps
  //I2CwriteByte(MPUadd,0x37,0x02);//
  I2CwriteByte(MAGadd,0x0A,0x01);//Medicion de una sola ejecución 
}
void loop()
{
  uint8_t Buf[14];
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);//reg de aceleraciones
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6); // Solicita 2 bytes por cada eje
  int16_t aceleracionX = Wire.read() << 8 | Wire.read();
  int16_t aceleracionY = Wire.read() << 8 | Wire.read();
  int16_t aceleracionZ = Wire.read() << 8 | Wire.read();
  uint8_t ST1;
  do
  {
    I2Cread(MAGadd, 0x02, 1, &ST1);
  } while (!(ST1 & 0x01));//comprueba el estado del magnetómetro
  uint8_t Mag[7];
  I2Cread(MAGadd, 0x03, 7, Mag);
  int16_t mx = -(Mag[3] << 8 | Mag[2]);
  int16_t my = -(Mag[1] << 8 | Mag[0]);
  int16_t mz = -(Mag[5] << 8 | Mag[4]);
  delay(10);    
}
