#include <Wire.h>
#include <Zumo32U4.h>

#define XBee Serial1 // Hier wordt de Serial1 verbinding verklaard naar Xbee

class XBeeVerbinding { // Klasse aangemaakt voor de Xbee comminucatie
public:
  void begin(int baudRate) { // Hier wordt de Baudrate van de Xbee gedeclareerd
    XBee.begin(baudRate);
  }

  void println(const char* message) { // Methode om een bericht naar XBee te verzenden
    XBee.println(message);
  }
};

class IMU { // Klasse aamgemaakt voor de IMU
public:
  void init() { // Initialisatie van de IMU
    Wire.begin(); // Start de comminucatie
    if (!imu.init()) { // Doe dit als het opstarten van de IMU niet gelukt is
      while(1) {
        xbeeVerbinding.println("Failed to initialize IMU sensors."); // Verzend foutmelding naar XBee
        delay(100);
      }
    }
    imu.enableDefault(); // Zet de standaardinstellingen voor IMU
  }

  void read() { // Lees IMU-sensoren
    imu.read();
  }

  int getMagX() { // Haal de X-richting magnetometerwaarde op
    return imu.m.x;
  }

  int getMagY() { // Haal de Y-richting magnetometerwaarde op
    return imu.m.y;
  }

  int getGyroX() { // Haal de X-richting gyroscoopwaarde op
    return round((float)(imu.a.x / 320) * 2 ); // Pas de berekening toe om waardes te krijgen van -100 tot 100
  }

  int getGyroY() { // Haal de Y-richting gyroscoopwaarde op
    return round((float)(imu.a.y / 320) * 2 ); // Pas de berekening toe om waardes te krijgen van -100 tot 100
  }

  void sendIMUReport() { // Methode om het rapport met IMU-gegevens te verzenden
    read(); // Lees de IMU-sensoren

    // Formatteer het rapport met IMU-gegevens
    snprintf(report, sizeof(report), "Gyroscope: X:%d Y:%d, Magnetometer: X:%d Y:%d",
             getGyroX(), getGyroY(), getMagX(), getMagY());

    XBee.println(report); // Verzend het rapport naar de XBee-module

    delay(100); // Wacht 100 milliseconden voordat de lus opnieuw wordt uitgevoerd
  }

private:
  Zumo32U4IMU imu; // Instantie van Zumo32U4IMU
  XBeeVerbinding xbeeVerbinding; // Instantie van XBeeVerbinding voor communicatie
  char report[120]; // Array voor het opslaan van een rapport
};

IMU imu; // Declaratie van een IMU-object

void setup() {
  imu.init(); // Initialiseer de IMU-sensoren

  XBeeVerbinding xbeeVerbinding; // Maak een XBeeVerbinding-object aan
  xbeeVerbinding.begin(4800); // Start de XBee-communicatie met een baudrate van 4800
}

void loop() {
  imu.sendIMUReport(); // Roep de methode aan om het IMU-rapport te verzenden
}
