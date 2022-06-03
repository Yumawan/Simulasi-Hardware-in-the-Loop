/*
 * Nama (NIM): 1.Kelvin Angga Yumawan (13219025) 
 *             2.Muhammad Wildan Asyrof (13219067)
 * Deskripsi: Dilakukan simulasi Hardware in the loop menggunakan 2 Arduino yang
 *            berperan sebagai Controller dan Plant (motor) dengan menggunakan 
 *            komunikasi I2C. Controller berperan sebagai Master dan Plant berperan 
 *            sebagai Slave. Code ini merupakan code plant yang mewakili karakteristik
 *            motor asli.
 */
#include<Wire.h>
#define LENGTH 250

//------------Global Variable------------
int8_t x[LENGTH]; // Input Motor
int8_t y[LENGTH]; // Output Motor
int i, cond;
int temp;
byte SlaveSend,SlaveReceived;
//---------------------------------------

int filter() {
  int hasil;
  if (i>0){
    hasil = (0.0667289*x[i]+0.0667289*x[i-1]+0.866542*y[i-1]);
    
  }
  else{
    hasil = 0.0667289*x[i];
  }
  return hasil;
}

void setup() {
  // Inisialisasi Komunikasi Serial dengan PC
  Serial.begin(9600);

  // Alamat slave arduino adalah 0x14
  Wire.begin(0x14);

  // Interrupt untuk penerimaan dan pengiriman
  Wire.onReceive(DataReceive);
  Wire.onRequest(DataRequest);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

// Fungsi Penerimaan data dari Master
void DataReceive(int numBytes)
{
  if(Wire.available()) 
  { 
    SlaveReceived = Wire.read();
    x[i] = SlaveReceived;
  }
  y[i] = filter();
  SlaveSend = int8_t(y[i]);

  //Pengecekan penerimaan data i2c
//  Serial.print("Slave");
//  Serial.print("\t");
//  Serial.print(SlaveReceived);
//  Serial.print("\t");
}

// Fungsi Pengiriman data yang diinginkan Master
void DataRequest()
{
  Wire.write(SlaveSend);
  i++;
  
  //Pengecekan pengiriman data i2c
//  Serial.println(SlaveSend);
}
