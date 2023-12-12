/***
 * Functional failure test
 */

#include "CppUTest/TestHarness.h"
#include "NVSOnboard.h"
#include <math.h>
#include <stdio.h>
#include "pico/multicore.h"


bool timerCore1CB (repeating_timer_t *rt){
	int r = 2 * 7;
	return true;
}

void core1Entry(){
	 multicore_lockout_victim_init();
	for(;;){
		for (int i=1; i < 0xFFFFFF; i++ ){
			double sq = (double)i;
			double r = sqrt(sq);
			//printf("Sqrt(%d) = %.2f\n", i, r);
		}
	}
}



TEST_GROUP(NVSMulticore){
	repeating_timer_t xTimer;
    void setup(){
    	add_repeating_timer_us	(
    			1000,
				timerCore1CB,
				NULL,
				&xTimer
    	);
    	 multicore_launch_core1(core1Entry);
	}
	 void teardown(){
		 multicore_reset_core1();
		 cancel_repeating_timer (&xTimer);
		 NVSOnboard::delInstance();
	 }
};


TEST(NVSMulticore,  AddI8){
  NVSOnboard * nvs = NVSOnboard::getInstance(true);
  char key[] = "Testi8";
  int8_t v = 23;
  int8_t r;

  //Store key
  CHECK_EQUAL(NVS_OK, nvs->set_i8(key,  v));

  //Check count and value stored
  CHECK_EQUAL(1, nvs->numKeys());
  CHECK_EQUAL(NVS_OK,  nvs->get_i8(key, &r));
  CHECK_EQUAL(v, r);
  CHECK_EQUAL(true,  nvs->isDirty());

  //Commit
  CHECK_EQUAL(NVS_OK,  nvs->commit());

  //Check key stored
  CHECK_EQUAL(1, nvs->numKeys());
  CHECK_EQUAL(false,  nvs->isDirty());
  CHECK_EQUAL(NVS_OK,  nvs->get_i8(key, &r));
  CHECK_EQUAL(v, r);
}

TEST(NVSMulticore,  ChangeI8){
   NVSOnboard * nvs = NVSOnboard::getInstance(true);
	char key[] = "Testi8";
	int8_t v   = 101;
	int8_t nv =77;
	int8_t r;

	//Add Key
	CHECK_EQUAL(NVS_OK, nvs->set_i8(key,  v));
	CHECK_EQUAL(1, nvs->numKeys());
	CHECK_EQUAL(NVS_OK,  nvs->get_i8(key, &r));
	CHECK_EQUAL(v, r);

	//Commit
	CHECK_EQUAL(NVS_OK,  nvs->commit());
	CHECK_EQUAL(1, nvs->numKeys());
	CHECK_EQUAL(NVS_OK,  nvs->get_i8(key, &r));
	CHECK_EQUAL(v, r);


	// Change
	CHECK_EQUAL(NVS_OK, nvs->set_i8(key,  nv));
	CHECK_EQUAL(1, nvs->numKeys());
	CHECK_EQUAL(NVS_OK,  nvs->get_i8(key, &r));
	CHECK_EQUAL(nv, r);

	//Commit
	CHECK_EQUAL(NVS_OK,  nvs->commit());
	CHECK_EQUAL(1, nvs->numKeys());
	CHECK_EQUAL(NVS_OK,  nvs->get_i8(key, &r));
	CHECK_EQUAL(nv, r);

}

TEST(NVSMulticore,  Clear){
	 NVSOnboard * nvs = NVSOnboard::getInstance();
	 CHECK_EQUAL(NVS_OK, nvs->clear());
	 CHECK_EQUAL(0, nvs->numKeys());
	 CHECK_EQUAL(NVS_OK,  nvs->commit());
	 CHECK_EQUAL(0, nvs->numKeys());
}


TEST(NVSMulticore,  EraseDirty){
   NVSOnboard * nvs = NVSOnboard::getInstance(true);
	char key[] = "Dirty";
	int8_t v = 101;
	int8_t r;

	//Add
	CHECK_EQUAL(NVS_OK, nvs->set_i8(key,  v));
	CHECK_EQUAL(NVS_OK,  nvs->get_i8(key, &r));
	CHECK_EQUAL(v, r);
	CHECK_EQUAL(1, nvs->numKeys());

	//Erase
	CHECK_EQUAL(NVS_OK,  nvs->erase_key(key));
	CHECK_EQUAL(0, nvs->numKeys());
	CHECK_EQUAL(NVS_ERR_NOT_FOUND,  nvs->get_i8(key, &r));
}


TEST(NVSMulticore,  EraseCommit){
	NVSOnboard * nvs = NVSOnboard::getInstance(true);
	char key[] = "Com";
	int8_t v = 101;
	int8_t r;

	CHECK_EQUAL(NVS_OK, nvs->set_i8(key,  v));
	CHECK_EQUAL(1, nvs->numKeys());
	//printf("Added %s\n", key);
	//nvs->printNVS();
	CHECK_EQUAL(NVS_OK,  nvs->commit());
	CHECK_EQUAL(1, nvs->numKeys());
	//printf("Commit\n");
	//nvs->printNVS();

	CHECK_EQUAL(NVS_OK,  nvs->erase_key(key));
	//printf("Erase %s\n", key);
	//nvs->printNVS();
	CHECK_EQUAL(NVS_ERR_NOT_FOUND,  nvs->get_i8(key, &r));
	CHECK_EQUAL(0, nvs->numKeys());

	CHECK_EQUAL(NVS_OK,  nvs->commit());
	//printf("Commit %s\n", key);
	//nvs->printNVS();
	CHECK_EQUAL(NVS_ERR_NOT_FOUND,  nvs->get_i8(key, &r));
	CHECK_EQUAL(0, nvs->numKeys());

}

TEST(NVSMulticore,  Rollback){
   NVSOnboard * nvs = NVSOnboard::getInstance(true);
   char key[] ="Roll";
	int8_t v = 101;
	int8_t nv=55;
	int8_t r;

	//1 New entry and roll back without commit
	CHECK_EQUAL(NVS_OK, nvs->set_i8(key,  v));
	CHECK_EQUAL(1, nvs->numKeys());
	CHECK_EQUAL(NVS_OK, nvs->rollback());
	CHECK_EQUAL(0, nvs->numKeys());

	//Roll back after commit
	CHECK_EQUAL(NVS_OK, nvs->set_i8(key,  v));
	CHECK_EQUAL(1, nvs->numKeys());
	CHECK_EQUAL(NVS_OK, nvs->commit());
	CHECK_EQUAL(NVS_OK, nvs->rollback());
	CHECK_EQUAL(1, nvs->numKeys());

	//Roll back on change
	CHECK_EQUAL(NVS_OK, nvs->set_i8(key,  nv));
	CHECK_EQUAL(1, nvs->numKeys());
	CHECK_EQUAL(NVS_OK,  nvs->get_i8(key, &r));
	CHECK_EQUAL(nv, r);
	CHECK_EQUAL(NVS_OK, nvs->rollback());
	CHECK_EQUAL(1, nvs->numKeys());
	CHECK_EQUAL(NVS_OK,  nvs->get_i8(key, &r));
	CHECK_EQUAL(v, r);

	//Roll back erase
	CHECK_EQUAL(NVS_OK,  nvs->erase_key(key));
	CHECK_EQUAL(0, nvs->numKeys());
	CHECK_EQUAL(NVS_OK, nvs->rollback());
	CHECK_EQUAL(1, nvs->numKeys());
	CHECK_EQUAL(NVS_OK,  nvs->get_i8(key, &r));
	CHECK_EQUAL(v, r);

}



