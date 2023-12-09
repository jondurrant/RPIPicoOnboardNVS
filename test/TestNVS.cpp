/***
 * Functional failure test
 */

#include "CppUTest/TestHarness.h"
#include "NVSOnboard.h"


TEST_GROUP(NVS){
};


TEST(NVS,  AddI8){
  NVSOnboard nvs(true);
  char key[] = "Testi8";
  int8_t v = 23;
  int8_t r;

  //Store key
  CHECK_EQUAL(NVS_OK, nvs.set_i8(key,  v));

  //Check count and value stored
  CHECK_EQUAL(1, nvs.numKeys());
  CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
  CHECK_EQUAL(v, r);
  CHECK_EQUAL(true,  nvs.isDirty());

  //Commit
  CHECK_EQUAL(NVS_OK,  nvs.commit());

  //Check key stored
  CHECK_EQUAL(1, nvs.numKeys());
  CHECK_EQUAL(false,  nvs.isDirty());
  CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
  CHECK_EQUAL(v, r);
}

TEST(NVS,  ChangeI8){
	NVSOnboard nvs(true);
	char key[] = "Testi8";
	int8_t v   = 101;
	int8_t nv =77;
	int8_t r;

	//Add Key
	CHECK_EQUAL(NVS_OK, nvs.set_i8(key,  v));
	CHECK_EQUAL(1, nvs.numKeys());
	CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
	CHECK_EQUAL(v, r);

	//Commit
	CHECK_EQUAL(NVS_OK,  nvs.commit());
	CHECK_EQUAL(1, nvs.numKeys());
	CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
	CHECK_EQUAL(v, r);


	// Change
	CHECK_EQUAL(NVS_OK, nvs.set_i8(key,  nv));
	CHECK_EQUAL(1, nvs.numKeys());
	CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
	CHECK_EQUAL(nv, r);

	//Commit
	CHECK_EQUAL(NVS_OK,  nvs.commit());
	CHECK_EQUAL(1, nvs.numKeys());
	CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
	CHECK_EQUAL(nv, r);

}

TEST(NVS,  EraseDirty){
	NVSOnboard nvs(true);
	char key[] = "Dirty";
	int8_t v = 101;
	int8_t r;

	//Add
	CHECK_EQUAL(NVS_OK, nvs.set_i8(key,  v));
	CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
	CHECK_EQUAL(v, r);
	CHECK_EQUAL(1, nvs.numKeys());

	//Erase
	CHECK_EQUAL(NVS_OK,  nvs.erase_key(key));
	CHECK_EQUAL(0, nvs.numKeys());
	CHECK_EQUAL(NVS_ERR_NOT_FOUND,  nvs.get_i8(key, &r));
}


TEST(NVS,  EraseCommit){
	NVSOnboard nvs(true);
	char key[] = "Com";
	int8_t v = 101;
	int8_t r;

	CHECK_EQUAL(NVS_OK, nvs.set_i8(key,  v));
	CHECK_EQUAL(1, nvs.numKeys());
	//printf("Added %s\n", key);
	//nvs.printNVS();
	CHECK_EQUAL(NVS_OK,  nvs.commit());
	CHECK_EQUAL(1, nvs.numKeys());
	//printf("Commit\n");
	//nvs.printNVS();

	CHECK_EQUAL(NVS_OK,  nvs.erase_key(key));
	//printf("Erase %s\n", key);
	//nvs.printNVS();
	CHECK_EQUAL(NVS_ERR_NOT_FOUND,  nvs.get_i8(key, &r));
	CHECK_EQUAL(0, nvs.numKeys());

	CHECK_EQUAL(NVS_OK,  nvs.commit());
	//printf("Commit %s\n", key);
	//nvs.printNVS();
	CHECK_EQUAL(NVS_ERR_NOT_FOUND,  nvs.get_i8(key, &r));
	CHECK_EQUAL(0, nvs.numKeys());

}

TEST(NVS,  Rollback){
	NVSOnboard nvs(true);
	char key[] = "Com";
	int8_t v = 101;
	int8_t nv=55;
	int8_t r;

	//1 New entry and roll back without commit
	CHECK_EQUAL(NVS_OK, nvs.set_i8(key,  v));
	CHECK_EQUAL(1, nvs.numKeys());
	CHECK_EQUAL(NVS_OK, nvs.rollback());
	CHECK_EQUAL(0, nvs.numKeys());

	//Roll back after commit
	CHECK_EQUAL(NVS_OK, nvs.set_i8(key,  v));
	CHECK_EQUAL(1, nvs.numKeys());
	CHECK_EQUAL(NVS_OK, nvs.commit());
	CHECK_EQUAL(NVS_OK, nvs.rollback());
	CHECK_EQUAL(1, nvs.numKeys());

	//Roll back on change
	CHECK_EQUAL(NVS_OK, nvs.set_i8(key,  nv));
	CHECK_EQUAL(1, nvs.numKeys());
	CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
	CHECK_EQUAL(nv, r);
	CHECK_EQUAL(NVS_OK, nvs.rollback());
	CHECK_EQUAL(1, nvs.numKeys());
	CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
	CHECK_EQUAL(v, r);

	//Roll back erase
	CHECK_EQUAL(NVS_OK,  nvs.erase_key(key));
	CHECK_EQUAL(0, nvs.numKeys());
	CHECK_EQUAL(NVS_OK, nvs.rollback());
	CHECK_EQUAL(1, nvs.numKeys());
	CHECK_EQUAL(NVS_OK,  nvs.get_i8(key, &r));
	CHECK_EQUAL(v, r);

}



