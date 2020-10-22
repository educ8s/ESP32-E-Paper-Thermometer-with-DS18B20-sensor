    /////////////////////////////////////////////////////////////////
   //       ESP32 E-PAPER THERMOMETER       10/22/2020  v1.01     //
  //       Get the latest version of the code here:              //
 //        http://educ8s.tv/esp32-e-paper-thermometer           //
/////////////////////////////////////////////////////////////////

#include <GxEPD.h>
#include <GxGDEP015OC1/GxGDEP015OC1.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include "BitmapGraphics.h"

#include <Fonts/FreeSansBold24pt7b.h>

#define ONE_WIRE_BUS 15

boolean METRIC = false;    //Set true for metric system; false for imperial
float previousTemperature = -100.0;
float temperature = 0;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

GxIO_Class io(SPI, SS, 22, 21);
GxEPD_Class display(io, 16, 4);

void setup() {
  
  Serial.begin(9600);
  display.init();
  sensors.begin();
  
  display.drawExampleBitmap(gImage_splash, 0, 0, 200, 200, GxEPD_BLACK);
  display.update();
  delay(3000);

  if(METRIC)
  {
    display.drawExampleBitmap(gImage_gui, 0, 0, 200, 200, GxEPD_BLACK);
  }else
  {
    display.drawExampleBitmap(gImage_gui_f, 0, 0, 200, 200, GxEPD_BLACK);
  }
  
  display.update();

  if(METRIC)
  {
    display.drawExampleBitmap(gImage_gui, sizeof(gImage_gui), GxEPD::bm_default | GxEPD::bm_partial_update);
  }else
  {
    display.drawExampleBitmap(gImage_gui_f, sizeof(gImage_gui_f), GxEPD::bm_default | GxEPD::bm_partial_update);
  }
}

void loop() 
{
  previousTemperature = temperature;
  temperature = getTemperatureFromSensor();
  
  if(previousTemperature!=temperature)
  {
    showPartialUpdate(temperature);
  }
  
  printTemperatureToSerial();
  delay(5000);
}

float getTemperatureFromSensor()
{
  float temperature = 0;
  sensors.requestTemperatures();
  if(METRIC)
  {
    temperature = sensors.getTempCByIndex(0);
  }else
  {
    temperature = sensors.getTempFByIndex(0);
  }
  temperature = round(temperature * 10) / 10;
  return temperature;
}

void showPartialUpdate(float temperature)
{
  Serial.println("Updating display ...");
  String temperatureString = String(temperature,1);
  const char* name = "FreeSansBold24pt7b";
  const GFXfont* f = &FreeSansBold24pt7b;
  
  uint16_t box_x = 60;
  uint16_t box_y = 60;
  uint16_t box_w = 90;
  uint16_t box_h = 100;
  uint16_t cursor_y = box_y + 16;

  display.setRotation(45);
  display.setFont(f);
  display.setTextColor(GxEPD_BLACK);

  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.setCursor(box_x, cursor_y+38);
  display.print(temperatureString); 
  display.updateWindow(box_x, box_y, box_w, box_h, true);
}

void printTemperatureToSerial()
{
  Serial.print(temperature);
  Serial.print(" C");
  Serial.print("\n");
}
