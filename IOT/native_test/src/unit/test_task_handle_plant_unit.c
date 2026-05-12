#include "unity.h"
#include "task_handle_plant.h"
#include "mocks/Mockbutton.h"
#include "mocks/Mockdisplay.h"
#include "mocks/Mockserver_connector.h"
#include "mocks/Mockeeprom_wrapper.h"
#include <string.h>
#include <stdbool.h>

void setUp(void)
{
    plant_id = 0;
}

void test_handle_plant_init_ShouldSetDefaultPlantId_WhenEepromIsEmpty(void)
{
    eeprom_wrapper_read_byte_ExpectAndReturn(&savedValue, 0xFF);

    eeprom_wrapper_update_byte_Expect(&savedValue, 0);

    task_handle_plant_init();

    TEST_ASSERT_EQUAL(0, plant_id);
}

void test_handle_plant_init_ShouldLoadExistingPlantId_WhenEepromHasData(void)
{

    eeprom_wrapper_read_byte_ExpectAndReturn(&savedValue, 67);

    task_handle_plant_init();

    TEST_ASSERT_EQUAL(67, plant_id);
}

void test_handle_plant_init_ShouldLoadExistingMaximumPlantId_WhenEepromHasData(void)
{

    eeprom_wrapper_read_byte_ExpectAndReturn(&savedValue, 254); // 254 er max for bruger da 255 er reserveret (tom value)

    task_handle_plant_init();

    TEST_ASSERT_EQUAL(254, plant_id);
}

void test_handle_plant_init_ShouldLoadExistingMinimumPlantId_WhenEepromHasData(void)
{

    eeprom_wrapper_read_byte_ExpectAndReturn(&savedValue, 0);

    task_handle_plant_init();

    TEST_ASSERT_EQUAL(0, plant_id);
}

void test_handle_plant_run_ShouldDoNothing_WhenNoButtonsIsPressed(void)
{
    plant_id = 67;
    button_get_ExpectAndReturn(1, false); // Ikke trykket
    button_get_ExpectAndReturn(2, false); // Ikke trykket
    button_get_ExpectAndReturn(3, false); // Ikke trykket

    task_handle_plant_run();

    TEST_ASSERT_EQUAL(67, plant_id);

    TEST_ASSERT_FALSE(plant_id_changed);
}

void test_handle_plant_run_ShouldStayAtSamePlantId_WhenButton1And2IsPressed(void)
{
    plant_id = 67;
    button_get_ExpectAndReturn(1, true); // Trykket
    button_get_ExpectAndReturn(2, true); // Trykket
    display_int_Expect(67);
    button_get_ExpectAndReturn(3, false); // Ikke trykket

    task_handle_plant_run();

    TEST_ASSERT_EQUAL(67, plant_id);

    TEST_ASSERT_TRUE(plant_id_changed);
}

void test_handle_plant_run_ShouldIncrementPlantId_WhenButton2IsPressed(void)
{
    plant_id = 66;
    button_get_ExpectAndReturn(1, false); // Ikke trykket
    button_get_ExpectAndReturn(2, true);  // Trykket
    display_int_Expect(67);
    button_get_ExpectAndReturn(3, false); // Ikke trykket

    task_handle_plant_run();

    TEST_ASSERT_EQUAL(67, plant_id);

    TEST_ASSERT_TRUE(plant_id_changed);
}

void test_handle_plant_run_ShouldDecrementPlantId_WhenButton1IsPressed(void)
{
    plant_id = 66;
    button_get_ExpectAndReturn(1, true);  // Trykket
    button_get_ExpectAndReturn(2, false); // Ikke trykket
    display_int_Expect(65);
    button_get_ExpectAndReturn(3, false); // Ikke trykket

    task_handle_plant_run();

    TEST_ASSERT_EQUAL(65, plant_id);

    TEST_ASSERT_TRUE(plant_id_changed);
}

void test_handle_plant_run_ShouldDecrementToMinPlantId_WhenButton1IsPressedFrom1(void)
{
    plant_id = 1;
    button_get_ExpectAndReturn(1, true);  // Trykket
    button_get_ExpectAndReturn(2, false); // Ikke trykket
    display_int_Expect(0);
    button_get_ExpectAndReturn(3, false); // Ikke trykket

    task_handle_plant_run();

    TEST_ASSERT_EQUAL(0, plant_id);

    TEST_ASSERT_TRUE(plant_id_changed);
}

void test_handle_plant_run_ShouldWrapToMaxValuePlantId_WhenButton1IsPressedFromMinValue(void)
{
    plant_id = 0;                         // MinValue
    button_get_ExpectAndReturn(1, true);  // Trykket
    button_get_ExpectAndReturn(2, false); // Ikke trykket
    display_int_Expect(254);              // MaxValue
    button_get_ExpectAndReturn(3, false); // Ikke trykket

    task_handle_plant_run();

    TEST_ASSERT_EQUAL(254, plant_id);

    TEST_ASSERT_TRUE(plant_id_changed);
}

void test_handle_plant_run_ShouldWrapTo0PlantId_WhenButton2IsPressedFrom254(void)
{
    plant_id = 254;
    button_get_ExpectAndReturn(1, false); // Ikke trykket
    button_get_ExpectAndReturn(2, true);  // Trykket
    display_int_Expect(0);
    button_get_ExpectAndReturn(3, false); // Ikke trykket

    task_handle_plant_run();

    TEST_ASSERT_EQUAL(0, plant_id);

    TEST_ASSERT_TRUE(plant_id_changed);
}

void test_handle_plant_run_ShouldIncrementToMaxPlantId_WhenButton2IsPressedFrom253(void)
{
    plant_id = 253;
    button_get_ExpectAndReturn(1, false); // Ikke trykket
    button_get_ExpectAndReturn(2, true);  // Trykket
    display_int_Expect(254);
    button_get_ExpectAndReturn(3, false); // Ikke trykket

    task_handle_plant_run();

    TEST_ASSERT_EQUAL(254, plant_id);

    TEST_ASSERT_TRUE(plant_id_changed);
}

void test_handle_plant_run_ShouldSaveAndSendPlantIdAndDisplay1000_WhenButton3IsPressed(void)
{
    plant_id = 67;
    button_get_ExpectAndReturn(1, false); // Ikke trykket
    button_get_ExpectAndReturn(2, false); // Ikke trykket
    button_get_ExpectAndReturn(3, true);  // Trykket

    eeprom_wrapper_update_byte_Expect(&savedValue, 67);
    server_connector_send_plant_id_ExpectAndReturn(67, WIFI_OK);

    display_int_Expect(1000);

    task_handle_plant_run();

    TEST_ASSERT_EQUAL(67, plant_id);

    TEST_ASSERT_FALSE(plant_id_changed);
}

void test_handle_plant_run_ShouldStillDisplay1000_EvenIfServerFails(void)
{
    plant_id = 67;
    button_get_ExpectAndReturn(1, false);
    button_get_ExpectAndReturn(2, false);
    button_get_ExpectAndReturn(3, true);

    eeprom_wrapper_update_byte_Expect(&savedValue, 67);
    server_connector_send_plant_id_ExpectAndReturn(67, WIFI_FAIL);
    display_int_Expect(1000);

    task_handle_plant_run();
}

void test_handle_plant_run_IncrementDecrementAFewTimesAndSave(void)
{
    plant_id = 65;
    TEST_ASSERT_FALSE(plant_id_changed);

    button_get_ExpectAndReturn(1, false);
    button_get_ExpectAndReturn(2, true); // plant_id++
    display_int_Expect(66);
    button_get_ExpectAndReturn(3, false);
    task_handle_plant_run();
    TEST_ASSERT_TRUE(plant_id_changed);

    button_get_ExpectAndReturn(1, true);
    button_get_ExpectAndReturn(2, false); // plant_id--
    display_int_Expect(65);
    button_get_ExpectAndReturn(3, false);
    task_handle_plant_run();
    TEST_ASSERT_TRUE(plant_id_changed);

    button_get_ExpectAndReturn(1, false);
    button_get_ExpectAndReturn(2, true); // plant_id++
    display_int_Expect(66);
    button_get_ExpectAndReturn(3, false);
    task_handle_plant_run();
    TEST_ASSERT_TRUE(plant_id_changed);

    button_get_ExpectAndReturn(1, false);
    button_get_ExpectAndReturn(2, true); // plant_id++
    display_int_Expect(67);
    button_get_ExpectAndReturn(3, false);
    task_handle_plant_run();
    TEST_ASSERT_TRUE(plant_id_changed);

    button_get_ExpectAndReturn(1, false);
    button_get_ExpectAndReturn(2, false);
    button_get_ExpectAndReturn(3, true); // Gem
    eeprom_wrapper_update_byte_Expect(&savedValue, 67);
    server_connector_send_plant_id_ExpectAndReturn(67, WIFI_OK);
    display_int_Expect(1000);
    task_handle_plant_run();

    TEST_ASSERT_EQUAL(67, plant_id);
    TEST_ASSERT_FALSE(plant_id_changed);
}