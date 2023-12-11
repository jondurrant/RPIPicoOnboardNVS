/*
 * NVSOnboard.cpp
 *
 *  Created on: 7 Dec 2023
 *      Author: jondurrant
 */

#include "NVSOnboard.h"

#include "hardware/sync.h"

#if NVS_CORES == 2
#include "pico/multicore.h"
#include "hardware/structs/sio.h"
#endif

NVSOnboard * NVSOnboard::pSingleton = NULL;

NVSOnboard * NVSOnboard::getInstance(bool cleanNVS){
	if (NVSOnboard::pSingleton == NULL) {
		NVSOnboard::pSingleton = new NVSOnboard(cleanNVS);
	}
	return NVSOnboard::pSingleton;
}

void  NVSOnboard::delInstance(){
	if (NVSOnboard::pSingleton != NULL) {
		delete(NVSOnboard::pSingleton);
		NVSOnboard::pSingleton = NULL;
	}
}


NVSOnboard::NVSOnboard(bool cleanNVS) {
	if (! cleanNVS){
		init();
	}

}

NVSOnboard::~NVSOnboard() {
	rollback();
}




nvs_err_t NVSOnboard::set(
		const char* key,
		nvs_type_t type,
		size_t len,
		const void * value){
	nvs_err_t res;

	//Check key valid
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	//Create new entry
	nvs_entry_t * entry = (nvs_entry_t *)malloc(sizeof(nvs_entry_t));
	if (entry == NULL){
		return NVS_ERR_NOT_ENOUGH_MEM;
	}
	strcpy(entry->key, key);
	entry->type = type;
	entry->len = len;
	entry->value = malloc(entry->len);
	memcpy(entry->value,  value, entry->len);

	// *** START THREAD SEMAPHORE
	//If existing dirty value then clean it up
	if (xDirty.count(key) > 0){
		nvs_entry_t * old = xDirty[key];
		free(old->value);
		free(old);
	}

	//Store new entry
	xDirty[key]=entry;

	// *** END THREAD SEMAPHORE

	return NVS_OK;
}

nvs_err_t NVSOnboard::get(
		const char* key,
		nvs_type_t type,
		size_t * len,
		void * out_value){

	//Check key is valid
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	// Check if on dirty list
	if (xDirty.count(key) > 0){
		nvs_entry_t * old = xDirty[key];
		if (old->type == NVS_TYPE_ERASE){
			return NVS_ERR_NOT_FOUND;
		}
		if (old->type == type){
			if (old->len <= *len) {
				memcpy(out_value, old->value, old->len);
				*len = old->len;
				return NVS_OK;
			} else {
				return NVS_ERR_NOT_ENOUGH_MEM;
			}
		} else {
			return NVS_ERR_INVALID_TYPE;
		}
	}

	if (xClean.count(key) > 0){
		nvs_entry_t * old = xClean[key];
		if (old->type == type){
			if (old->len <= *len) {
				memcpy(out_value, old->value, old->len);
				*len = old->len;
				return NVS_OK;
			} else {
				return NVS_ERR_NOT_ENOUGH_MEM;
			}
		} else {
			return NVS_ERR_INVALID_TYPE;
		}
	}

	return NVS_ERR_NOT_FOUND;
}



nvs_err_t NVSOnboard::set_i8 ( const char* key, int8_t value){
	return set(key, NVS_TYPE_I8, 1, &value);
}

nvs_err_t NVSOnboard::set_u8 ( const char* key, uint8_t value){
	return set(key, NVS_TYPE_U8, 1, &value);
}

nvs_err_t NVSOnboard::set_i16 ( const char* key, int16_t value){
	return set(key, NVS_TYPE_I16, 2, &value);
}

nvs_err_t NVSOnboard::set_u16 ( const char* key, uint16_t value){
	return set(key, NVS_TYPE_U16, 2, &value);
}

nvs_err_t NVSOnboard::set_i32 ( const char* key, int32_t value){
	return set(key, NVS_TYPE_I32, 4, &value);
}

nvs_err_t NVSOnboard::set_u32 ( const char* key, uint32_t value){
	return set(key, NVS_TYPE_U32, 4, &value);
}

nvs_err_t NVSOnboard::set_i64 ( const char* key, int64_t value){
	return set(key, NVS_TYPE_I64, 8, &value);
}

nvs_err_t NVSOnboard::set_u64 ( const char* key, uint64_t value){
	return set(key, NVS_TYPE_U64, 8, &value);
}

nvs_err_t NVSOnboard::set_str ( const char* key, const char* value){
	return set(key, NVS_TYPE_STR, strlen(value)+1, value);
}

nvs_err_t NVSOnboard::set_blob(
		const char* key,
		const void* value,
		size_t length){
	return set(key, NVS_TYPE_BLOB, length, value);
}


nvs_err_t NVSOnboard::get_i8 ( const char* key, int8_t* out_value){
	size_t len = 1;
	return get(key, NVS_TYPE_I8, &len, out_value);
}

nvs_err_t NVSOnboard::get_u8 ( const char* key, uint8_t* out_value){
	size_t len = 1;
	return get(key, NVS_TYPE_U8, &len, out_value);
}

nvs_err_t NVSOnboard::get_i16 ( const char* key, int16_t* out_value){
	size_t len = 2;
	return get(key, NVS_TYPE_I16, &len, out_value);
}

nvs_err_t NVSOnboard::get_u16 ( const char* key, uint16_t* out_value){
	size_t len = 2;
	return get(key, NVS_TYPE_U16, &len, out_value);
}

nvs_err_t NVSOnboard::get_i32 ( const char* key, int32_t* out_value){
	size_t len = 4;
	return get(key, NVS_TYPE_I32, &len, out_value);
}

nvs_err_t NVSOnboard::get_u32 ( const char* key, uint32_t* out_value){
	size_t len = 4;
	return get(key, NVS_TYPE_U32, &len, out_value);
}

nvs_err_t NVSOnboard::get_i64 ( const char* key, int64_t* out_value){
	size_t len = 8;
	return get(key, NVS_TYPE_I64,  &len, out_value);
}

nvs_err_t NVSOnboard::get_u64 ( const char* key, uint64_t* out_value){
	size_t len = 8;
	return get(key, NVS_TYPE_U64, &len, out_value);
}

nvs_err_t NVSOnboard::get_str (
		const char* key,
		char* out_value,
		size_t* length){
	return get(key, NVS_TYPE_STR,  length, out_value);
}

nvs_err_t NVSOnboard::get_blob(
		const char* key,
		void* out_value,
		size_t* length){
	return get(key, NVS_TYPE_BLOB,  length, out_value);
}



nvs_err_t NVSOnboard::erase_key( const char* key){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	if (xDirty.count(key) > 0){
		nvs_entry_t * entry = xDirty[key];
		if (entry->type != NVS_TYPE_ERASE){
			free(entry->value);
		}
		if (xClean.count(key) == 0){
			free(entry);
			xDirty.erase(key);
			return NVS_OK;
		} else {
			entry->type = NVS_TYPE_ERASE;
			entry->len =0;
			return NVS_OK;
		}
	} else if (xClean.count(key) > 0){
		nvs_entry_t * entry = (nvs_entry_t *) malloc(sizeof(nvs_entry_t));
		if (entry == NULL){
			return NVS_ERR_NOT_ENOUGH_MEM;
		}
		strcpy(entry->key, key);
		entry->type = NVS_TYPE_ERASE;
		entry->len =0;
		entry->value=NULL;
		xDirty[key] = entry;
		return NVS_OK;
	}


	return NVS_ERR_NOT_FOUND;
}

nvs_err_t NVSOnboard::erase_all(){

	nvs_err_t res = NVS_OK;
	map<string, nvs_entry_t *>::iterator it = xDirty.begin();
	while (it != xDirty.end()){
		res = erase_key(it->first.c_str());
		if (res != NVS_OK)
			return res;
		it++;
	}
	it = xClean.begin();
	while (it != xClean.end()){
		res = erase_key(it->first.c_str());
		if (res != NVS_OK)
		    return res;
		 it++;
	}

	return res;
}

nvs_err_t NVSOnboard::commit(){
	size_t size = pagesSize();

	if (size == 0){
		return NVS_ERR_NOT_ENOUGH_MEM;
	}

	uint8_t *mem = (uint8_t *)malloc(size);
	if (mem == NULL){
		return NVS_ERR_NOT_ENOUGH_MEM;
	}
	nvs_header_t * header = (nvs_header_t *) mem;
	nvs_entry_t * entry = (nvs_entry_t *) (mem + sizeof(nvs_header_t));
	uint32_t offset = sizeof(nvs_header_t) + (numKeys() * sizeof(nvs_entry_t));

	map<string, nvs_entry_t *>::iterator it = xDirty.begin();
	while (it != xDirty.end()){
		if (it->second->type != NVS_TYPE_ERASE){
			 memcpy(entry,   it->second, sizeof(nvs_entry_t));
			 void * data = (void *) (mem + offset);
			 memcpy(data, it->second->value, it->second->len);
			 entry->value = (void *)(FLASH_READ_START + offset);
			 offset += it->second->len;

			 //printf("Commit %s = %s %u\n", it->first.c_str(), it->second->key, it->second->len);
			 entry ++;
		}
		it++;
	}
	it = xClean.begin();
	while (it != xClean.end()){
		if (xDirty.count(it->first) == 0){
			 memcpy(entry,   it->second, sizeof(nvs_entry_t));
			 void * data = (void *) (mem + offset);
			 memcpy(data, it->second->value, it->second->len);
			 entry->value = (void *)(FLASH_READ_START + offset);
			 offset += it->second->len;
			 entry ++;
		}
		 it++;
	}

	header->count = numKeys();
	entry = (nvs_entry_t *) (mem + sizeof(nvs_header_t));
	header->hash = oat_hash((const char *)entry, size - sizeof(nvs_header_t));
	header->pages = size;

	//printf("Flashing header %u, %u, %u\n", header->count, header->pages, header->hash);

	 //CRITICAL SECTION - NO INTRUP1GT OF MULTIPROCESSOR
	uint32_t status = save_and_disable_interrupts();
#if NVS_CORES == 2
	uint core = 1;
	bool lockout = false;
	if ((uint8_t)sio_hw->cpuid == (uint8_t) core){
		core = 0;
	}
	if (multicore_lockout_victim_is_initialized ( core)){
		lockout = multicore_lockout_start_timeout_us(NVS_WAIT);
	}
#endif
	 flash_range_erase(FLASH_WRITE_START, NVS_SIZE);
     flash_range_program(FLASH_WRITE_START, mem, size);
    restore_interrupts(status);

     free(mem);
     rollback();
     init();
#if NVS_CORES == 2
     if (lockout){
    	 lockout = multicore_lockout_end_timeout_us (NVS_WAIT);
     }
#endif

     return NVS_OK;

}


size_t NVSOnboard::pagesSize(){
	size_t size;
	size_t indexSize;
	size_t dataSize = 0;
	size_t pagesSize;

	indexSize = numKeys() * sizeof(nvs_entry_t);
	map<string, nvs_entry_t *>::iterator it = xDirty.begin();
	while (it != xDirty.end()){
		 nvs_entry_t * entry = it->second;
		if (entry->len > NVS_SIZE){
			return 0; // Corrupt
		}
		 dataSize +=entry->len;
		 it++;
	}
	it = xClean.begin();
	while (it != xClean.end()){
		if (xDirty.count(it->first) == 0){
		    nvs_entry_t * entry = it->second;
		    if (entry->len > NVS_SIZE){
		    	return 0; // Corrupt
		    }
		    dataSize +=entry->len;
		}
		 it++;
	}
	size = 2 * sizeof(uint32_t ) + indexSize + dataSize;
	pagesSize = (size / 256) * 256;
	if (size > (pagesSize)){
		pagesSize += 256;
	}

	//printf("Size %u, index %u, data %u, Pages %u\n", size, indexSize, dataSize, pagesSize);


	return pagesSize;
}

nvs_err_t NVSOnboard::rollback(){
	map<string, nvs_entry_t *>::iterator it = xDirty.begin();
	while (it != xDirty.end()){
		 nvs_entry_t * entry = it->second;
		 if (entry->type != NVS_TYPE_ERASE){
			 free(entry->value);
		 }
		 free(entry);
		 it++;
	}
	xDirty.clear();
	return NVS_OK;
}


nvs_err_t NVSOnboard::validKey(const char* key){
	if ((strlen(key) > 0) && (strlen(key) < NVS_MAX_KEY_LEN)){
		return NVS_OK;
	}
	return NVS_ERR_INVALID_NAME;
}


unsigned int NVSOnboard::numKeys(){
	if (!isDirty()){
		return xClean.size();
	}
	unsigned int count = xClean.size();

	map<string, nvs_entry_t *>::iterator it = xDirty.begin();
	while (it != xDirty.end()){
		  if (xClean.count(it->first) > 0){
			  if (it->second->type == NVS_TYPE_ERASE){
				  count--;
			  }
		  } else {
			  if (it->second->type != NVS_TYPE_ERASE){
				  count++;
			  }
		  }
	      it++;
	}
	return count;
}

bool NVSOnboard::isDirty(){
	return  !xDirty.empty();
}

bool NVSOnboard::contains(const char *key){
	nvs_err_t res;
		res = validKey(key);
		if (res != NVS_OK){
			return false;
		}

		if (xDirty.count(key) > 0){
			return  true;
		}
		if (xClean.count(key) > 0){
			return  true;
		}
		return false;
}


void NVSOnboard::init(){
	void *mem = (void *)FLASH_READ_START;
	nvs_header_t * header = (nvs_header_t *) mem;
    nvs_entry_t * entry = (nvs_entry_t *) ((uint32_t)mem + sizeof(nvs_header_t));

    xDirty.clear();
    xClean.clear();

    //printf("Loading header %u, %u, %u\n", header->count, header->pages, header->hash);
    if (header->pages > NVS_SIZE ){
    	printf("ERROR NVS is corrupt\n");
    	return;
    }

	uint32_t hash = oat_hash((const char *)entry, header->pages - sizeof(nvs_header_t));
	if (hash != header->hash){
		printf("ERROR NVS Hash Check Failed\n");
		return;
	} else {
		//Hash Passed
		for (int i=0; i < header->count; i++){
			xClean[entry[i].key] = &entry[i];
		}
	}


}

/*
 * Bob Jenkins "One-at-a-time" hash
 */
uint32_t NVSOnboard::oat_hash(const char *s, size_t len){
    unsigned char *p = (unsigned char*) s;
    uint32_t h = 0;

    while(len--) {
        h += *p++;
        h += (h << 10);
        h ^= (h >> 6);
    }

    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);

    return h;
}


void NVSOnboard::printNVS(){
	printf("NVS keys: %u, Clean %u Dirty %u\n",
			numKeys(),
			xClean.size(),
			xDirty.size());

	int count = 0;
	int erase = 0;

	map<string, nvs_entry_t *>::iterator it = xClean.begin();
	while (it != xClean.end()){
		if (xDirty.count(it->first) == 0){
			printf("%s [CLEAN] %d\n", it->first.c_str(), it->second->len);
		} else {
			if (xDirty[it->first]->type == NVS_TYPE_ERASE){
				printf("%s [ERASE] %d\n", it->first.c_str(), 0);
				erase++;
			} else {
				printf("%s [CLEAN] %d\n", it->first.c_str(), it->second->len);
			}
		}
		count++;
		it++;
	}

	 it = xDirty.begin();
	while (it != xDirty.end()){
		if (xClean.count(it->first) == 0){
			if (it->second->type != NVS_TYPE_ERASE ){
				printf("%s [NEW] %d\n", it->first.c_str(), it->second->len);
			} else {
				printf("%s [ERASE NEW] %d\n", it->first.c_str(), it->second->len);
				erase++;
			}
			count++;
		}
		it++;
	}

	printf("Entries %d Erase %d Total %d\n", count,  erase, count- erase);

}

nvs_err_t NVSOnboard::clear(){
	nvs_err_t res = rollback();
	if (res != NVS_OK){
		return res;
	}
	xClean.clear();
	return NVS_OK;
}

