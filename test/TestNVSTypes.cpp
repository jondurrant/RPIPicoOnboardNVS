/***
 * Functional failure test
 */

#include "CppUTest/TestHarness.h"
#include "NVSOnboard.h"


TEST_GROUP(NVSTypes){
	 void teardown(){
		 NVSOnboard::delInstance();
	 }
};


TEST(NVSTypes,  Dirty){
  NVSOnboard * nvs = NVSOnboard::getInstance(true);
  char 		i8k[] = "i8";
  int8_t 		i8v = -23;
  int8_t 		i8c;
  char 		u8k[] = "u8";
  uint8_t 	u8v = 12;
  uint8_t 	u8c;
  char 		i16k[] = "i16";
  int16_t 		i16v = -23;
  int16_t 		i16c;
  char 		u16k[] = "u16";
  uint16_t 	u16v = 12;
  uint16_t 	u16c;
  char 		i32k[] = "i32";
  int32_t 		i32v = -23;
  int32_t 		i32c;
  char 		u32k[] = "u32";
  uint32_t 	u32v = 12;
  uint32_t 	u32c;
  char 		i64k[] = "i64";
  int64_t 		i64v = -23;
  int64_t 		i64c;
  char 		u64k[] = "u64";
  uint64_t 	u64v = 12;
  uint64_t 	u64c;
  char strk[]="str";
  char strv[]="TEST STRING";
  char strc[20];
  char blobk[]="blob";
  size_t len=20;

  //Store key
  CHECK_EQUAL(NVS_OK, nvs->set_i8(i8k,  i8v));
  CHECK_EQUAL(NVS_OK, nvs->set_u8(u8k,  u8v));
  CHECK_EQUAL(NVS_OK, nvs->set_i16(i16k,  i16v));
  CHECK_EQUAL(NVS_OK, nvs->set_u16(u16k,  u16v));
  CHECK_EQUAL(NVS_OK, nvs->set_i32(i32k,  i32v));
  CHECK_EQUAL(NVS_OK, nvs->set_u32(u32k,  u32v));
  CHECK_EQUAL(NVS_OK, nvs->set_i64(i64k,  i64v));
  CHECK_EQUAL(NVS_OK, nvs->set_u64(u64k,  u64v));

  CHECK_EQUAL(NVS_OK, nvs->set_str(strk,  strv));
  CHECK_EQUAL(NVS_OK, nvs->set_blob(blobk,  strv, strlen(strv)+1));

  //Check Count
  CHECK_EQUAL(true,  nvs->isDirty());
  CHECK_EQUAL(10, nvs->numKeys());

  //Check values
  CHECK_EQUAL(NVS_OK,  nvs->get_i8(i8k, &i8c));
  CHECK_EQUAL(i8v, i8c);
  CHECK_EQUAL(NVS_OK,  nvs->get_u8(u8k, &u8c));
  CHECK_EQUAL(u8v, u8c);
  CHECK_EQUAL(NVS_OK,  nvs->get_i16(i16k, &i16c));
  CHECK_EQUAL(i16v, i16c);
  CHECK_EQUAL(NVS_OK,  nvs->get_u16(u16k, &u16c));
  CHECK_EQUAL(u16v, u16c);
  CHECK_EQUAL(NVS_OK,  nvs->get_i32(i32k, &i32c));
  CHECK_EQUAL(i32v, i32c);
  CHECK_EQUAL(NVS_OK,  nvs->get_u32(u32k, &u32c));
  CHECK_EQUAL(u32v, u32c);
  CHECK_EQUAL(NVS_OK,  nvs->get_i64(i64k, &i64c));
  CHECK_EQUAL(i64v, i64c);
  CHECK_EQUAL(NVS_OK,  nvs->get_u64(u64k, &u64c));
  CHECK_EQUAL(u64v, u64c);

  len=20;
  CHECK_EQUAL(NVS_OK,  nvs->get_str(strk,  strc,  &len));
  CHECK(strcmp(strv, strc) == 0);
  CHECK_EQUAL(len, strlen(strv) + 1);

  len=20;
 CHECK_EQUAL(NVS_OK,  nvs->get_blob(blobk,  strc,  &len));
 CHECK(strcmp(strv, strc) == 0);
 CHECK_EQUAL(len, strlen(strv) + 1);

}


TEST(NVSTypes,  Commit){
  NVSOnboard * nvs = NVSOnboard::getInstance(true);
  char 		i8k[] = "i8";
  int8_t 		i8v = -23;
  int8_t 		i8c;
  char 		u8k[] = "u8";
  uint8_t 	u8v = 12;
  uint8_t 	u8c;
  char 		i16k[] = "i16";
  int16_t 		i16v = -23;
  int16_t 		i16c;
  char 		u16k[] = "u16";
  uint16_t 	u16v = 12;
  uint16_t 	u16c;
  char 		i32k[] = "i32";
  int32_t 		i32v = -23;
  int32_t 		i32c;
  char 		u32k[] = "u32";
  uint32_t 	u32v = 12;
  uint32_t 	u32c;
  char 		i64k[] = "i64";
  int64_t 		i64v = -23;
  int64_t 		i64c;
  char 		u64k[] = "u64";
  uint64_t 	u64v = 12;
  uint64_t 	u64c;
  char strk[]="str";
  char strv[]="TEST STRING";
  char strc[20];
  char blobk[]="blob";
  size_t len=20;

  //Store key
  CHECK_EQUAL(NVS_OK, nvs->set_i8(i8k,  i8v));
  CHECK_EQUAL(NVS_OK, nvs->set_u8(u8k,  u8v));
  CHECK_EQUAL(NVS_OK, nvs->set_i16(i16k,  i16v));
  CHECK_EQUAL(NVS_OK, nvs->set_u16(u16k,  u16v));
  CHECK_EQUAL(NVS_OK, nvs->set_i32(i32k,  i32v));
  CHECK_EQUAL(NVS_OK, nvs->set_u32(u32k,  u32v));
  CHECK_EQUAL(NVS_OK, nvs->set_i64(i64k,  i64v));
  CHECK_EQUAL(NVS_OK, nvs->set_u64(u64k,  u64v));

  CHECK_EQUAL(NVS_OK, nvs->set_str(strk,  strv));
  CHECK_EQUAL(NVS_OK, nvs->set_blob(blobk,  strv, strlen(strv)+1));

  //Check Count
  CHECK_EQUAL(true,  nvs->isDirty());
  CHECK_EQUAL(10, nvs->numKeys());

  //nvs->printNVS();

  //Commit
  CHECK_EQUAL(NVS_OK, nvs->commit());
  CHECK_EQUAL(false,  nvs->isDirty());
  CHECK_EQUAL(10, nvs->numKeys());

  //nvs->printNVS();

  //Check
  CHECK_EQUAL(NVS_OK,  nvs->get_i8(i8k, &i8c));
  CHECK_EQUAL(i8v, i8c);
  CHECK_EQUAL(NVS_OK,  nvs->get_u8(u8k, &u8c));
  CHECK_EQUAL(u8v, u8c);
  CHECK_EQUAL(NVS_OK,  nvs->get_i16(i16k, &i16c));
  CHECK_EQUAL(i16v, i16c);
  CHECK_EQUAL(NVS_OK,  nvs->get_u16(u16k, &u16c));
  CHECK_EQUAL(u16v, u16c);
  CHECK_EQUAL(NVS_OK,  nvs->get_i32(i32k, &i32c));
  CHECK_EQUAL(i32v, i32c);
  CHECK_EQUAL(NVS_OK,  nvs->get_u32(u32k, &u32c));
  CHECK_EQUAL(u32v, u32c);
  CHECK_EQUAL(NVS_OK,  nvs->get_i64(i64k, &i64c));
  CHECK_EQUAL(i64v, i64c);
  CHECK_EQUAL(NVS_OK,  nvs->get_u64(u64k, &u64c));
  CHECK_EQUAL(u64v, u64c);

  len=20;
  CHECK_EQUAL(NVS_OK,  nvs->get_str(strk,  strc,  &len));
  CHECK(strcmp(strv, strc) == 0);
  CHECK_EQUAL(len, strlen(strv) + 1);

  len=20;
 CHECK_EQUAL(NVS_OK,  nvs->get_blob(blobk,  strc,  &len));
 CHECK(strcmp(strv, strc) == 0);
 CHECK_EQUAL(len, strlen(strv) + 1);

}





