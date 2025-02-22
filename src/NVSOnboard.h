/*
 * NVSOnboard.h
 *
 *  Created on: 7 Dec 2023
 *      Author: jondurrant
 */

#ifndef SRC_NVSONBOARD_H_
#define SRC_NVSONBOARD_H_

#include <string>
#include <cstring>
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

//FreeRTOS Kernel Support
#ifdef LIB_FREERTOS_KERNEL
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#endif


#ifndef NVS_MAX_KEY_LEN
//Maximum size of the key in key value pairs
#define NVS_MAX_KEY_LEN 16
#endif

#ifndef NVS_SIZE
//Size of the NVS Segment. Must be a multiple of 4096
#define NVS_SIZE 4096
#endif

#ifndef NVS_CORES
//Set NVS_CORES to 2 to enable multicore support
#define NVS_CORES 1
#endif

#ifndef NVS_WAIT
#define NVS_WAIT 10000   //usecs
#endif


//Write and Read address for the base of the NVS region being used
#define FLASH_WRITE_START (PICO_FLASH_SIZE_BYTES - NVS_SIZE)
#define FLASH_READ_START  (FLASH_WRITE_START + XIP_BASE)

//Type definitions
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
	NVS_TYPE_DOUBLE   = 0x22,  /*!< Type Double */
	NVS_TYPE_BOOL   = 0x23,  /*!< Type Bool */
	NVS_TYPE_PWD   = 0x24,  /*!< Type Password */
    NVS_TYPE_BLOB  = 0x42,  /*!< Type blob */
	NVS_TYPE_ERASE  = 0xFE,
    NVS_TYPE_ANY   = 0xff   /*!< Must be last */
} nvs_type_t;

//Error return types
typedef enum {
    NVS_OK    			= 0x00,
    NVS_FAIL,
	NVS_ERR_NOT_FOUND,
	NVS_ERR_INVALID_NAME,
	NVS_ERR_INVALID_TYPE,
	NVS_ERR_NOT_ENOUGH_MEM,
	NVS_ERR_LOCK_FAILED
} nvs_err_t;

//Entry structure used to build a index table in Flash
typedef struct {
	char 					key[NVS_MAX_KEY_LEN];
	nvs_type_t	 	type;
	size_t 				len;
	void *					value;

} nvs_entry_t;

//Header for the NVS region
typedef struct {
	uint32_t 	count;
	uint32_t   pages;
	uint32_t	hash;
} nvs_header_t;

class NVSOnboard {
public:
	/***
	 * Singleton to return the current instance
	 * @param cleanNVS - Only usable on first call.
	 * If true returns a clean NVS without reading the Flash
	 * Used mainly for rest purposes.
	 * Default is false
	 * @return the NVSOnboard instance
	 */
	static NVSOnboard * getInstance(bool cleanNVS=false);

	/***
	 * Distroy the current singleton instance.
	 * Mainly used for Unit test purposes to confirm code is memory safe
	 */
	static void  delInstance();

	/***
	 * Distructor
	 */
	virtual ~NVSOnboard();

	/***
	 * Set an bool value in NVS
	 * @param key
	 * @param value
	 * @return for return values see @set
	 */
	nvs_err_t set_bool ( const char* key, bool value);

	/***
	 * Set an Int8 value in NVS
	 * @param key
	 * @param value
	 * @return for return values see @set
	 */
	nvs_err_t set_i8 ( const char* key, int8_t value);

	/***
	 * Set an unsigned Int8 value in NVS
	 * @param key
	 * @param value
	 * @return for return values see @set
	 */
	nvs_err_t set_u8 ( const char* key, uint8_t value);

	/***
	 * Set an Int16 value in NVS
	 * @param key
	 * @param value
	 * @return for return values see @set
	 */
	nvs_err_t set_i16 ( const char* key, int16_t value);

	/***
	 * Set an unsigned int 16 value in NVS
	 * @param key
	 * @param value
	 * @return for return values see @set
	 */
	nvs_err_t set_u16 ( const char* key, uint16_t value);

	/***
	 * Set an  int 32 bit value in NVS
	 * @param key
	 * @param value
	 * @return for return values see @set
	 */
	nvs_err_t set_i32 ( const char* key, int32_t value);

	/***
	 * Set an unsigned int 32 bit value in NVS
	 * @param key
	 * @param value
	 * @return for return values see @set
	 */
	nvs_err_t set_u32 ( const char* key, uint32_t value);

	/***
	 * Set an  int 64 bit value in NVS
	 * @param key
	 * @param value
	 * @return for return values see @set
	 */
	nvs_err_t set_i64 ( const char* key, int64_t value);

	/***
	 * Set an unsigned int 64 bit value in NVS
	 * @param key
	 * @param value
	 * @return for return values see @set
	 */
	nvs_err_t set_u64 ( const char* key, uint64_t value);

	/***
	 * Set an double value in NVS
	 * @param key
	 * @param value
	 * @return for return values see @set
	 */
	nvs_err_t set_double ( const char* key, double value);

	/***
	 * Set a string value in NVS
	 * @param key
	 * @param value - point to cjar * string
	 * @return for return values see @set
	 */
	nvs_err_t set_str ( const char* key, const char* value);

	/***
	 * Set a string password value in NVS
	 * @param key
	 * @param value - point to cjar * string
	 * @return for return values see @set
	 */
	nvs_err_t set_pwd ( const char* key, const char* value);

	/***
	 * Set a blob value in NVS
	 * @param key
	 * @param value - pointer to blob
	 * @param length - length of the blob
	 * @return for return values see @set
	 */
	nvs_err_t set_blob( const char* key, const void* value, size_t length);

	/***
	 * return the value stored in the key or not found error
	 * @param key  used for storing the value
	 * @param out_value
	 * @return NVS_OK if found. See @get for full list of errors
	 */
	nvs_err_t get_bool ( const char* key, bool* out_value);

	/***
	 * return the value stored in the key or not found error
	 * @param key  used for storing the value
	 * @param out_value
	 * @return NVS_OK if found. See @get for full list of errors
	 */
	nvs_err_t get_i8 ( const char* key, int8_t* out_value);

	/***
	 * return the value stored in the key or not found error
	 * @param key  used for storing the value
	 * @param out_value
	 * @return NVS_OK if found. See @get for full list of errors
	 */
	nvs_err_t get_u8 ( const char* key, uint8_t* out_value);

	/***
	 * return the value stored in the key or not found error
	 * @param key  used for storing the value
	 * @param out_value
	 * @return NVS_OK if found. See @get for full list of errors
	 */
	nvs_err_t get_i16 ( const char* key, int16_t* out_value);

	/***
	 * return the value stored in the key or not found error
	 * @param key  used for storing the value
	 * @param out_value
	 * @return NVS_OK if found. See @get for full list of errors
	 */
	nvs_err_t get_u16 ( const char* key, uint16_t* out_value);

	/***
	 * return the value stored in the key or not found error
	 * @param key  used for storing the value
	 * @param out_value
	 * @return NVS_OK if found. See @get for full list of errors
	 */
	nvs_err_t get_i32 ( const char* key, int32_t* out_value);

	/***
	 * return the value stored in the key or not found error
	 * @param key  used for storing the value
	 * @param out_value
	 * @return NVS_OK if found. See @get for full list of errors
	 */
	nvs_err_t get_u32 ( const char* key, uint32_t* out_value);

	/***
	 * return the value stored in the key or not found error
	 * @param key  used for storing the value
	 * @param out_value
	 * @return NVS_OK if found. See @get for full list of errors
	 */
	nvs_err_t get_i64 ( const char* key, int64_t* out_value);

	/***
	 * return the value stored in the key or not found error
	 * @param key  used for storing the value
	 * @param out_value
	 * @return NVS_OK if found. See @get for full list of errors
	 */
	nvs_err_t get_u64 ( const char* key, uint64_t* out_value);

	/***
	 * return the value stored in the key or not found error
	 * @param key  used for storing the value
	 * @param out_value
	 * @return NVS_OK if found. See @get for full list of errors
	 */
	nvs_err_t get_double( const char* key, double* out_value);

	/***
	 * return the value stored in the key or not found error
	 * @param key  used for storing the value
	 * @param length - Input is Max size of out_value to write to.
	 * Then set with the length actualy returned
	 * @param out_value
	 * @return NVS_OK if found. See @get for full list of errors
	 */
	nvs_err_t get_str ( const char* key, char* out_value, size_t* length);

	/***
	 * return the value stored in the key or not found error
	 * @param key  used for storing the value
	 * @param length - Input is Max size of out_value to write to.
	 * Then set with the length actualy returned
	 * @param out_value
	 * @return NVS_OK if found. See @get for full list of errors
	 */
	nvs_err_t get_pwd ( const char* key, char* out_value, size_t* length);


	/***
	 * return the value stored in the key or not found error
	 * @param key  used for storing the value
	 * @param length - Input is Max size of out_value to write to.
	 * Then set with the length actualy returned
	 * @param out_value
	 * @return NVS_OK if found. See @get for full list of errors
	 */
	nvs_err_t get_blob( const char* key, void* out_value, size_t* length);

	/***
	 * Erase a Key Value pair. Note does not commit and can be rolled back
	 * @param key to delete
	 * @return NVS_OK if completed.
	 * NVS_ERR_NOT_ENOUGH_MEM if malloc fails to build erase record
	 */
	nvs_err_t erase_key( const char* key);

	/***
	 * Erase all keys value pairs. Note does not commit and can be rolled back
	 * @return  NVS_OK if completed
	 * NVS_ERR_NOT_ENOUGH_MEM if malloc fails to build erase record
	 */
	nvs_err_t erase_all();

	/***
	 * Commit current state of the NVS to Flash. Will turn off interupts
	 * and halt any other running processes while this happens.
	 * This applies to both cores.
	 * @return NVS_OK if completed
	 * NVS_ERR_NOT_ENOUGH_MEM if malloc fails or page size is larger than NVS_SIZE
	 *
	 */
	nvs_err_t commit();

	/***
	 * Rollback any changes which have not been commited
	 * @return NVS_OK always
	 *
	 */
	nvs_err_t rollback();

	/***
	 * Clears NVS to zero entry state. Does not commit to flash
	 * @return NVS_OK always
	 */
	nvs_err_t clear();

	/***
	 * Checks if key is used
	 * @param key
	 * @return true if key is used
	 */
	bool contains(const char *key);

	/***
	 * Returns type for key
	 * @param key
	 * @return type
	 */
	nvs_type_t type(const char *key);

	/***
	 * Returns the number of keys in use
	 * @return number of keys or entries in NVS
	 */
	unsigned int numKeys();

	/***
	 * Does the NVS need to be written to Flash
	 * @return true if dirty and needs to be commited
	 */
	bool isDirty();

	/***
	 * Debug function to print out the keys within NVS
	 * and there status.
	 * Also counts and memory size
	 */
	void printNVS();


	/***
	 * Returns size of the given key in storage
	 * @param key
	 * @return 0 if not found
	 */
	size_t size(const char * key);

protected:
	/***
	 * Constructor
	 * @param cleanNVS - If true don't load from Flash but create clean
	 */
	NVSOnboard(bool cleanNVS=false);

	/***
	 * Load the clean list from the flash
	 */
	void init();

	/***
	 * Set the key value pair within the NVS
	 * @param key - to use to store the data item
	 * @param type of the data item
	 * @param len of the data item
	 * @param value pointer to the value
	 * @return NVS_OK if compelted
	 * NVS_ERR_NOT_ENOUGH_MEM if malloc fails
	 *
	 *
	 */
	nvs_err_t set(const char* key,  nvs_type_t type, size_t len, const void * value);

	/***
	 * Get the value associated with the key.
	 * @param key that value was stored with
	 * @param type of the value to be returned. Will be checked against stored value
	 * @param len of the memory to copy value into
	 * @param out_value pointer to memory to write value to
	 * @return NVS_OK if returned ok
	 * NVS_ERR_NOT_FOUND if no value found
	 * NVS_ERR_INVALID_TYPE if types do not match
	 * NVS_ERR_NOT_ENOUGH_MEM if len is too small to copy over data
	 *
	 */
	nvs_err_t get(const char* key,  nvs_type_t type, size_t * len, void * out_value);




	/***
	 * Validate Key is ok
	 * @param key
	 * @return true if ok
	 */
	nvs_err_t validKey(const char* key);

	/***
	 * Calculate the size needed to write the NVS
	 * Will be a multiple of 256
	 * @return mem size
	 */
	size_t pagesSize();

	/***
	 * Simple has function used to check if flash is corrupt
	 * @param s
	 * @param len
	 * @return has
	 */
	uint32_t oat_hash(const char *s, size_t len);

	map<string, nvs_entry_t *> xDirty;
	map<string, nvs_entry_t *> xClean;

#ifdef LIB_FREERTOS_KERNEL
	SemaphoreHandle_t xWriteSemaphore = NULL;
#endif

private:
	static NVSOnboard * pSingleton;

};

#endif /* SRC_NVSONBOARD_H_ */
