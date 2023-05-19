#include "fff.h"
#include "gtest/gtest.h"
#include <stdint.h>



extern "C" 
{
//#include "../src/implementation/co2Impl/mh_z19.h"
#include "../src/implementation/co2Impl/co2.h" 
//#include "../src/implementation/tempImpl/temperaturImpl.h"
//#include "../src/implementation/humidityImpl/humidityImpl.h"
//#include "../drivers/hih8120.h"
}

DEFINE_FFF_GLOBALS	

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

/* 
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
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_measure);
FAKE_VALUE_FUNC(int16_t, hih8120_getTemperature_x10);

class TempImplTest : public testing::Test {
protected:
    void SetUp() override {
        // Resets the fake functions
        RESET_FAKE(hih8120_initialise);
        RESET_FAKE(hih8120_measure);
        RESET_FAKE(hih8120_getTemperature_x10);
    }
    void TearDown() override {

	}
};

TEST_F(TempImplTest, TestGetMeasurement) {
	int temp;
	temp = tempimpl_getMeasurement();
	EXPECT_EQ(0, temp);
}


TEST_F(TempImplTest, TestTempImplInit) {
    tempimpl_init();
    
    EXPECT_EQ(hih8120_initialise_fake, 1);
    //EXPECT_EQ(HIH8120_DRIVER_NOT_INITIALISED, false);
}

TEST_F(TempImplTest, TestTempImplMeasure) {
    hih8120_measure_fake.return_val = HIH8120_OK;
    tempimpl_measure();
    
    EXPECT_EQ(hih8120_measure_fake, 1);
} 











// Define fake functions
//FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_initialise);
FAKE_VALUE_FUNC(hih8120_driverReturnCodes, hih8120_measure);
FAKE_VALUE_FUNC(int16_t, hih8120_getHumidityPercent_x10);

class HumImplTest : public testing::Test {
protected:
    void SetUp() override {
        // Resets the fake functions
        RESET_FAKE(hih8120_measure);
        RESET_FAKE(hih8120_getHumidityPercent_x10);
    }
    void TearDown() override {

	}
};

TEST_F (HumImplTest, TestGetMeasurement) {
	int Hum;
	Hum = humimpl_getMeasurement();
	EXPECT_EQ(0, Hum);
}

TEST_F(HumImplTest, TestHumImplMeasure) {
    hih8120_measure_fake.return_val = HIH8120_OK;
    humimpl_measure();
    
    EXPECT_EQ(hih8120_measure_fake, 1);
}  */