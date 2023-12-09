/*
 * NVSOnboard.h
 *
 *  Created on: 7 Dec 2023
 *      Author: jondurrant
 */

#ifndef SRC_NVSONBOARD_H_
#define SRC_NVSONBOARD_H_

#include "pico/stdlib.h"
#include "hardware/flash.h"
#if CPPUTEST_USE_NEW_MACROS
   #undef new
#endif
#include <map>
using namespace std;
#if CPPUTEST_USE_NEW_MACROS
   #include "CppUTest/MemoryLeakDetectorNewMacros.h"
#endif

#define NVS_MAX_KEY_LEN 16

#define NVS_SIZE 4096
#define FLASH_WRITE_START (PICO_FLASH_SIZE_BYTES - NVS_SIZE)
#define FLASH_READ_START  (FLASH_WRITE_START + XIP_BASE)
typedef enum {
    NVS_TYPE_U8    = 0x01,  /*!< Type uint8_t */
    NVS_TYPE_I8    = 0x11,  /*!< Type int8_t */
    NVS_TYPE_U16   = 0x02,  /*!< Type uint16_t */
    NVS_TYPE_I16   = 0x12,  /*!< Type int16_t */
    NVS_TYPE_U32   = 0x04,  /*!< Type uint32_t */
    NVS_TYPE_I32   = 0x14,  /*!< Type int32_t */
    NVS_TYPE_U64   = 0x08,  /*!< Type uint64_t */
    NVS_TYPE_I64   = 0x18,  /*!< Type int64_t */
    NVS_TYPE_STR   = 0x21,  /*!< Type string */
    NVS_TYPE_BLOB  = 0x42,  /*!< Type blob */
	NVS_TYPE_ERASE  = 0xFE,
    NVS_TYPE_ANY   = 0xff   /*!< Must be last */
} nvs_type_t;

typedef enum {
    NVS_OK    			= 0x00,
    NVS_FAIL,
	NVS_ERR_NOT_FOUND,
	NVS_ERR_INVALID_NAME,
	NVS_ERR_INVALID_TYPE,
	NVS_ERR_NOT_ENOUGH_MEM
} nvs_err_t;

typedef struct {
	char 					key[NVS_MAX_KEY_LEN];
	nvs_type_t	 	type;
	size_t 				len;
	void *					value;

} nvs_entry_t;

typedef struct {
	uint32_t 	count;
	uint32_t   pages;
	uint32_t	hash;
} nvs_header_t;

class NVSOnboard {
public:
	NVSOnboard(bool cleanNVS=false);
	virtual ~NVSOnboard();

	nvs_err_t set_i8 ( const char* key, int8_t value);
	nvs_err_t set_u8 ( const char* key, uint8_t value);
	nvs_err_t set_i16 ( const char* key, int16_t value);
	nvs_err_t set_u16 ( const char* key, uint16_t value);
	nvs_err_t set_i32 ( const char* key, int32_t value);
	nvs_err_t set_u32 ( const char* key, uint32_t value);
	nvs_err_t set_i64 ( const char* key, int64_t value);
	nvs_err_t set_u64 ( const char* key, uint64_t value);
	nvs_err_t set_str ( const char* key, const char* value);
	nvs_err_t set_blob( const char* key, const void* value, size_t length);

	nvs_err_t get_i8 ( const char* key, int8_t* out_value);
	nvs_err_t get_u8 ( const char* key, uint8_t* out_value);
	nvs_err_t get_i16 ( const char* key, int16_t* out_value);
	nvs_err_t get_u16 ( const char* key, uint16_t* out_value);
	nvs_err_t get_i32 ( const char* key, int32_t* out_value);
	nvs_err_t get_u32 ( const char* key, uint32_t* out_value);
	nvs_err_t get_i64 ( const char* key, int64_t* out_value);
	nvs_err_t get_u64 ( const char* key, uint64_t* out_value);
	nvs_err_t get_str ( const char* key, char* out_value, size_t* length);
	nvs_err_t get_blob( const char* key, void* out_value, size_t* length);


	nvs_err_t erase_key( const char* key);
	nvs_err_t erase_all();
	nvs_err_t commit();
	nvs_err_t rollback();

	bool contains(const char *key);
	unsigned int numKeys();
	bool isDirty();

	void printNVS();

private:

	void init();
	nvs_err_t validKey(const char* key);
	size_t pagesSize();
	uint32_t oat_hash(const char *s, size_t len);

	map<string, nvs_entry_t *> xDirty;
	map<string, nvs_entry_t *> xClean;

};

#endif /* SRC_NVSONBOARD_H_ */
