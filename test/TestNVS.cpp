/***
 * Functional failure test
 */

#include "CppUTest/TestHarness.h"
#include "NVSOnboard.h"


TEST_GROUP(NVS){
};

TEST(NVS,  Count){
  NVSOnboard nvs;

   CHECK_EQUAL(0,  nvs.numKeys());
}

TEST(NVS,  AddI8){
  NVSOnboard nvs;
  char key[] = "Testi8";
  int8_t v = 23;

  CHECK_EQUAL(NVS_OK, nvs.set_i8(key,  v));
  int8_t r;
  CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
  CHECK_EQUAL(v, r);
  CHECK_EQUAL(true,  nvs.isDirty());
  nvs.rollback();
}


