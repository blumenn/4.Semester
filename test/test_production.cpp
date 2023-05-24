#include "fff.h"
#include "gtest/gtest.h"

extern "C" {
#include "mh_z19.h"
#include "../src/implementation/co2Impl/co2.h" 

}
DEFINE_FFF_GLOBALS	

// Define fake functions
FAKE_VOID_FUNC(mh_z19_initialise, serial_comPort_t);
typedef void (*mh_z19_callback)(uint16_t);
FAKE_VOID_FUNC(mh_z19_injectCallBack, mh_z19_callback);
FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_takeMeassuring);
FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_getCo2Ppm, uint16_t*);

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
