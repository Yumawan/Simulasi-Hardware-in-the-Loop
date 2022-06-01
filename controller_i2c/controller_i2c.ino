/*
 * Nama (NIM): 1.Kelvin Angga Yumawan (13219025) 
 *             2.Muhammad Wildan Asyrof (13219067)
 * Deskripsi: Dilakukan simulasi Hardware in the loop menggunakan 2 Arduino yang
 *            berperan sebagai Controller dan Plant (motor) dengan menggunakan 
 *            komunikasi I2C. Controller berperan sebagai Master dan Plant berperan 
 *            sebagai Slave. Code ini merupakan code controller yang melakukan
 *            pengendalian PID.
 */
#include<Wire.h>

//------------Global Variable------------
float err, sum_err, diff_err, prev_err;
float PID;
float kp = 0.351,
      ki = 0.044,
      kd = 0;
int input;
int state, dont;
int8_t terima, kirim;
byte MasterSend;
byte MasterReceived;
//---------------------------------------

int computePID(int setpoint, int feedback){
  //Error
  err = setpoint-feedback;
  //Integral error
  sum_err += err;
  //Selisih error
  diff_err = err - prev_err;

  //Perhitungan PID
  PID = err * kp + sum_err * ki + diff_err * kd;
  //Simpan nilai error
  prev_err = err;

  //Pembatasan Nilai PID
  if (PID >= 127){
    PID = 127;
  }
  else if (PID <= -128){
    PID = -128;
  }      
  return PID;
}

void init_timer()
{
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
  
  // set compare match register to desired timer count:
  OCR1A = 1562;   // interrupt timer setiap 100ms
  
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  
  // enable global interrupts:
  sei();
}

void setup() {
  // Inisialisasi Timer1
  init_timer();

  // Deklarasi pin A0 sebagai Input dari Potensiometer
  pinMode(A0,INPUT);

  // Inisialisasi Komunikasi Serial dengan PC
  Serial.begin(9600);

  // Komunikasi antar Arduino menggunakan komunikasi I2C
  Wire.begin();
}

void loop() {
  // Pembacaan dan Scaling hasil pembacaan potensio
  input = analogRead(A0);
  input = map(input, 0, 1023, -128, 127);

  // Proses pengiriman dan penerimaan data i2c
  if(!dont){ //variable dont menandakan aktivitas komunikasi tidak dilakukan
    Serial.print(input);
    Serial.print("\t");

    // Pengiriman Data ke Slave
    if(!state){
      Wire.beginTransmission(0x14);
      Wire.write(MasterSend);
      Wire.endTransmission();
    }

    // Penerimaan Data dari Slave
    else{
      Wire.requestFrom(0x14, 1);
      if(Wire.available()){
        MasterReceived = Wire.read();
      }
      terima = MasterReceived;

      //Perhitungan PID
      kirim = computePID(input,terima);
      MasterSend = kirim;
    }
    Serial.println(terima);

    //Pengecekan pengiriman dan penerimaan data i2c
//    Serial.print(MasterReceived);
//    Serial.print("\t");
//    Serial.print("\t");
//    Serial.print(kirim);
//    Serial.print("\t");
//    Serial.println(MasterSend);

    state = !state;
    dont = 1; // dilakukan agar pengiriman atau penerimaan dilakukan setiap 100ms
  }
}

ISR(TIMER1_COMPA_vect){
  dont= 0;
}
