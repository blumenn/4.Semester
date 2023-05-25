#include "fff.h"
#include "gtest/gtest.h"
#include <stdint.h>
#include "FreeRTOS_FFF_MocksDeclaration.h"



extern "C" 
{
#include "mh_z19.h"
#include "../src/implementation/co2Impl/co2.h" 
#include "../src/implementation/tempImpl/temperaturImpl.h"
#include "../src/implementation/humidityImpl/humidityImpl.h"
#include "../src/implementation/tempImpl/delay_hal/delay_hal.h"
#include "hih8120.h"
SemaphoreHandle_t xTestSemaphore;
}



// Define fake functions
FAKE_VOID_FUNC(mh_z19_initialise, serial_comPort_t);
typedef void (*mh_z19_callback)(uint16_t);
FAKE_VOID_FUNC(mh_z19_injectCallBack, mh_z19_callback);
FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_takeMeassuring);
FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_getCo2Ppm, uint16_t*);
FAKE_VALUE_FUNC(uint16_t ,hih8120_getHumidityPercent_x10);
class Co2ImplTest : public testing::Test {
protected:
    void SetUp() override {
        // Resets the fake functions
        RESET_FAKE(mh_z19_initialise);
        RESET_FAKE(mh_z19_injectCallBack);
        RESET_FAKE(mh_z19_takeMeassuring);
    }
    void TearDown() override {

	}
};

TEST_F(Co2ImplTest, TestGetMeasurement) {
	int co2ppm;
	co2ppm = co2impl_getMeasurement();
	EXPECT_EQ(0, co2ppm);
}

 
TEST_F(Co2ImplTest, TestCo2ImplInit) {
    co2impl_init();
    
    EXPECT_EQ(mh_z19_initialise_fake.call_count, 1);
    EXPECT_EQ(mh_z19_injectCallBack_fake.call_count, 1);
}

TEST_F(Co2ImplTest, TestCo2ImplMeasure) {
    mh_z19_takeMeassuring_fake.return_val = MHZ19_OK;
    co2impl_measure();
    
    EXPECT_EQ(mh_z19_takeMeassuring_fake.call_count, 1);
}


// Define fake functions
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_initialise);
FAKE_VOID_FUNC(delay_hal, uint16_t);
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_wakeup);
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_measure);
FAKE_VALUE_FUNC(int16_t, hih8120_getTemperature_x10);

class TempImplTest : public testing::Test {
protected:
    void SetUp() override {
        // Resets the fake functions
        RESET_FAKE(hih8120_initialise);
        RESET_FAKE(hih8120_measure);
        RESET_FAKE(hih8120_getTemperature_x10);
        xTestSemaphore = xSemaphoreCreateMutex();
        xSemaphoreGive(xTestSemaphore);
    }
    void TearDown() override {
         vSemaphoreDelete(xTestSemaphore);

	}
};


TEST_F(TempImplTest, TestGetMeasurement) {
   uint16_t temp = 0; 
   uint16_t result = tempimpl_getMeasurement();
   EXPECT_EQ(temp, result);
}


TEST_F(TempImplTest, TestTempImplInit) {
     tempimpl_init();
    
    EXPECT_EQ(hih8120_initialise_fake.call_count, 1);
    EXPECT_EQ(HIH8120_DRIVER_NOT_INITIALISED, 2); //her er det 2, fordi DRIVER_NOT_INITIALISED er 2. "værdi" i return hih8120_driverReturnCode_t enum. 
}

TEST_F(TempImplTest, TestTempImplMeasure) {
   
    hih8120_measure_fake.return_val = HIH8120_OK;
    xSemaphoreTake_fake.return_val = pdTRUE;  
    hih8120_wakeup_fake.return_val = HIH8120_OK;
    tempimpl_measure();
    
    EXPECT_EQ(hih8120_measure_fake.call_count, 1);
    EXPECT_EQ(xSemaphoreTake_fake.call_count, 1);
    EXPECT_EQ(hih8120_wakeup_fake.call_count, 1);
    EXPECT_EQ(xSemaphoreGive_fake.call_count,2);
    
}







class HumImplTest : public testing::Test {
protected:
    void SetUp() override {
        // Resets the fake functions
        RESET_FAKE(hih8120_measure);
        RESET_FAKE(hih8120_getHumidityPercent_x10);
        xTestSemaphore = xSemaphoreCreateMutex();
        xSemaphoreGive(xTestSemaphore);
    }
    void TearDown() override {
         vSemaphoreDelete(xTestSemaphore);

	}
};



TEST_F (HumImplTest, TestGetMeasurement) {
	hih8120_getHumidityPercent_x10_fake.return_val = 0;
    humimpl_measure();
    uint16_t hum;
    hum = humimpl_getMeasurement();
    EXPECT_EQ(0, hum);
}

TEST_F(HumImplTest, TestHumImplMeasure) {
    hih8120_measure_fake.return_val = HIH8120_OK;
    humimpl_measure();
    
    EXPECT_EQ(hih8120_measure_fake.call_count, 0);
}  
