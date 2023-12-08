/***
 * Functional failure test
 */

#include "CppUTest/TestHarness.h"
#include "NVSOnboard.h"


TEST_GROUP(NVS){
};


TEST(NVS,  AddI8){
  NVSOnboard nvs;
  char key[] = "Testi8";
  int8_t v = 23;

  printf("NVS Count %u\n", nvs.numKeys());

  CHECK_EQUAL(NVS_OK, nvs.set_i8(key,  v));
  int8_t r;
  CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
  CHECK_EQUAL(v, r);
  CHECK_EQUAL(true,  nvs.isDirty());
  printf("NVS Count %u\n", nvs.numKeys());
  CHECK_EQUAL(NVS_OK,  nvs.commit());
  CHECK_EQUAL(false,  nvs.isDirty());
  printf("NVS Count %u\n", nvs.numKeys());
  CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
  CHECK_EQUAL(v, r);
}

TEST(NVS,  AddI8Commit){
	NVSOnboard nvs;
	char key[] = "Testi8B";
	int8_t v = 101;

	printf("NVS Count %u\n", nvs.numKeys());

	CHECK_EQUAL(NVS_OK, nvs.set_i8(key,  v));
	int8_t r;
	CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
	CHECK_EQUAL(v, r);
	CHECK_EQUAL(NVS_OK,  nvs.commit());
	CHECK_EQUAL(false,  nvs.isDirty());
	printf("NVS Count %u\n", nvs.numKeys());
	CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
	CHECK_EQUAL(v, r);

}

TEST(NVS,  EraseDirty){
	NVSOnboard nvs;
	char key[] = "Dirty";
	int8_t v = 101;
	int8_t r;

	CHECK_EQUAL(NVS_OK, nvs.set_i8(key,  v));
	CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
	CHECK_EQUAL(v, r);
	CHECK_EQUAL(NVS_OK,  nvs.erase_key(key));

	CHECK_EQUAL(NVS_ERR_NOT_FOUND,  nvs.get_i8(key, &r));
}

/*
TEST(NVS,  EraseCommit){
	NVSOnboard nvs;
	char key[] = "Com";
	int8_t v = 101;
	int8_t r;

	CHECK_EQUAL(NVS_OK, nvs.set_i8(key,  v));
	CHECK_EQUAL(NVS_OK,  nvs.commit());

	CHECK_EQUAL(NVS_OK,  nvs.erase_key(key));
	CHECK_EQUAL(NVS_ERR_NOT_FOUND,  nvs.get_i8(key, &r));

	CHECK_EQUAL(NVS_OK,  nvs.commit());
	CHECK_EQUAL(NVS_ERR_NOT_FOUND,  nvs.get_i8(key, &r));
}
*/

