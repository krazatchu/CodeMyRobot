#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox =  Adafruit_VL53L0X();


void setupLidar(void){

	if(!lox.begin()){
		Serial.println(F("Failed to boot VL53L0X"));
		while(1);
	}
}

/**
 * Gets the measurement from the lidar sensor. 
 * @param none
 * @return integer value of the measurement in millimeters (if out of range return -1.0)
 */
uint16_t getMeasurement(void){
  VL53L0X_RangingMeasurementData_t measure;
	lox.rangingTest(&measure, false);	

	if(measure.RangeStatus != 4){
		//Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
		return measure.RangeMilliMeter;
	}else{	
		Serial.println("Out of range!");
		return -1.0;
	}

}
