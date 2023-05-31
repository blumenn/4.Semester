    #include "gtest/gtest.h"
    #include "fff.h"
    #include "stdint.h"
    #include "FreeRTOS_FFF_MocksDeclaration.h"

    extern "C" {
    #include "../src/DataCollection/dataCollection.h"
    #include "../../implementation/servo/servoImpl.h"
    #include "servoHandler.h"
    #include "lora_driver.h"
    #include "rc_servo.h"
    }

    FAKE_VOID_FUNC(servo_init);
    FAKE_VOID_FUNC(servoOpenWindow);
    FAKE_VOID_FUNC(servoCloseWindow);
    FAKE_VOID_FUNC(run_servo_handler_task, void*);
    FAKE_VALUE_FUNC(latestData, get_latestData);

    class ServoHandlerTest : public testing::Test {
    protected:
        void SetUp() override {
            RESET_FAKE(servo_init);
            RESET_FAKE(servoOpenWindow);
            RESET_FAKE(servoCloseWindow);
            RESET_FAKE(run_servo_handler_task);
            RESET_FAKE(xTaskCreate);
            RESET_FAKE(xSemaphoreCreateMutex);
            RESET_FAKE(xSemaphoreTake);
            RESET_FAKE(xSemaphoreGive);
            RESET_FAKE(get_latestData);
            FFF_RESET_HISTORY();
            xSemaphoreTake_fake.return_val = pdTRUE;
        }
        void TearDown() override {
        }
    };

    TEST_F(ServoHandlerTest, ServoHandlerInitTEST) {
        servo_handler_init();
        
        EXPECT_EQ(servo_init_fake.call_count, 1);
        EXPECT_EQ(xSemaphoreCreateMutex_fake.call_count, 1);
        EXPECT_EQ(xTaskCreate_fake.call_count, 1);
    }

TEST_F(ServoHandlerTest, ServoSetConfigTEST) {
    uint16_t maxHumSetting = 60;
    uint16_t minHumSetting = 30;
    uint16_t maxTempSetting = 25;
    uint16_t minTempsetting = 15;
    uint16_t maxCo2Setting = 1500;
    uint16_t minCo2Setting = 1000;
    
    servo_set_config(maxHumSetting, minHumSetting, maxTempSetting, minTempsetting, maxCo2Setting, minCo2Setting);

    EXPECT_EQ(xSemaphoreTake_fake.call_count, 1);
    EXPECT_EQ(xSemaphoreGive_fake.call_count, 1);
}

TEST_F(ServoHandlerTest, ServoMeasuring_OpenWindowTEST) {
    latestData aboveThresholdData;
    aboveThresholdData.temp.data = 30;
    aboveThresholdData.hum.data = 70;
    aboveThresholdData.co2.data = 1600;

    get_latestData_fake.return_val = aboveThresholdData;

    servo_measuring();

    EXPECT_EQ(xSemaphoreTake_fake.call_count, 1);
    EXPECT_EQ(servoOpenWindow_fake.call_count, 1);
    EXPECT_EQ(xSemaphoreGive_fake.call_count, 1);
}

TEST_F(ServoHandlerTest, ServoMeasuring_CloseWindowTest) {
    latestData belowThresholdData;
    belowThresholdData.temp.data = 10;
    belowThresholdData.hum.data = 20;
    belowThresholdData.co2.data = 900;

    get_latestData_fake.return_val = belowThresholdData;

    servo_measuring();

    EXPECT_EQ(xSemaphoreTake_fake.call_count, 1);
    EXPECT_EQ(servoCloseWindow_fake.call_count, 1);
    EXPECT_EQ(xSemaphoreGive_fake.call_count, 1);
}

TEST_F(ServoHandlerTest, ServoMeasureing_forOneValueAboveConfigAndOneBelowTEST){
    latestData oneAboveandOneBelowTreshold;
    oneAboveandOneBelowTreshold.temp.data = 10;
    oneAboveandOneBelowTreshold.hum.data = 45;
    oneAboveandOneBelowTreshold.co2.data = 1700;

    get_latestData_fake.return_val = oneAboveandOneBelowTreshold;

    servo_measuring();

    EXPECT_EQ(xSemaphoreTake_fake.call_count, 1);
    EXPECT_EQ(servoOpenWindow_fake.call_count,1);
    EXPECT_EQ(xSemaphoreGive_fake.call_count, 1);
}

TEST_F(ServoHandlerTest, servo_measuring_toValuesOkOneValueBelowThresholdTEST){
        latestData toValuesOkOneValueBelowThresholdTEST;
    toValuesOkOneValueBelowThresholdTEST.temp.data = 10;
    toValuesOkOneValueBelowThresholdTEST.hum.data = 45;
    toValuesOkOneValueBelowThresholdTEST.co2.data = 1700;

    get_latestData_fake.return_val = toValuesOkOneValueBelowThresholdTEST;

    servo_measuring();

    EXPECT_EQ(xSemaphoreTake_fake.call_count, 1);
    EXPECT_EQ(servoOpenWindow_fake.call_count,1);
    EXPECT_EQ(xSemaphoreGive_fake.call_count, 1);
}