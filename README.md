# Simulasi-Hardware-in-the-Loop
Dilakukan simulasi Hardware in the Loop (HIL) menggunakan dua arduino dimana salah satu arduino berperan sebagai controller PID motor dan arduino lainnya berperan sebagai plant atau motor. Komunikasi yang dilakukan antar dua arduino adalah komunikasi I2C dengan arduino controller sebagai Master dan arduino plant sebagai Slave. 

Untuk melihat hasil pengujian:
1. Pengujian komunikasi data
    - Uncomment baris kode 173-177 pada file controller_i2c.ino
    - Uncomment baris kode 62-65 dan 75 pada file plant_i2c.ino

2. Pengujian Pengendali
    - Uncomment baris kode 168-170 pada file controller_i2c.ino
