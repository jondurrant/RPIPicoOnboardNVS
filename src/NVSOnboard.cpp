/*
 * NVSOnboard.cpp
 *
 *  Created on: 7 Dec 2023
 *      Author: jondurrant
 */

#include "NVSOnboard.h"

NVSOnboard::NVSOnboard() {
	init();

}

NVSOnboard::~NVSOnboard() {
	rollback();
}


nvs_err_t NVSOnboard::set_i8 ( const char* key, int8_t value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	if (xDirty.count(key) > 0){
		nvs_entry_t * old = xDirty[key];
		free(old->value);
		free(old);
	}
	nvs_entry_t * entry = (nvs_entry_t *)malloc(sizeof(nvs_entry_t));
	if (entry != NULL){
	strcpy(entry->key, key);
	entry->type = NVS_TYPE_I8;
	entry->len = 1;
	entry->value = malloc(entry->len);
	memcpy(entry->value, &value, entry->len);
	xDirty[key]=entry;
	} else {
		return NVS_ERR_NOT_ENOUGH_MEM;
	}
	return NVS_OK;
}

nvs_err_t NVSOnboard::set_u8 ( const char* key, uint8_t value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::set_i16 ( const char* key, int16_t value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::set_u16 ( const char* key, uint16_t value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::set_i32 ( const char* key, int32_t value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::set_u32 ( const char* key, uint32_t value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::set_i64 ( const char* key, int64_t value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::set_u64 ( const char* key, uint64_t value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::set_str ( const char* key, const char* value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::set_blob( const char* key, const void* value, size_t length){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}


nvs_err_t NVSOnboard::get_i8 ( const char* key, int8_t* out_value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	if (xDirty.count(key) > 0){
		nvs_entry_t * old = xDirty[key];
		if (old->type == NVS_TYPE_ERASE){
			return NVS_ERR_NOT_FOUND;
		}
		if (old->type == NVS_TYPE_I8){
			memcpy(out_value, old->value, old->len);
			return NVS_OK;
		} else {
			return NVS_ERR_INVALID_TYPE;
		}
	}

	if (xClean.count(key) > 0){
		nvs_entry_t * old = xClean[key];
		memcpy(out_value, old->value, old->len);
		return NVS_OK;
	}

	return NVS_ERR_NOT_FOUND;
}

nvs_err_t NVSOnboard::get_u8 ( const char* key, uint8_t* out_value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::get_i16 ( const char* key, int16_t* out_value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::get_u16 ( const char* key, uint16_t* out_value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::get_i32 ( const char* key, int32_t* out_value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::get_u32 ( const char* key, uint32_t* out_value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::get_i64 ( const char* key, int64_t* out_value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::get_u64 ( const char* key, uint64_t* out_value){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::get_str ( const char* key, char* out_value, size_t* length){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::get_blob( const char* key, void* out_value, size_t* length){
	nvs_err_t res;
	res = validKey(key);
	if (res != NVS_OK){
		return res;
	}

	return NVS_FAIL;
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

	map<string, nvs_entry_t *>::iterator it = xDirty.begin();
	while (it != xDirty.end()){
		erase_key(it->first.c_str());
		it++;
	}
	it = xClean.begin();
	while (it != xClean.end()){
		erase_key(it->first.c_str());
		 it++;
	}

	return NVS_OK;
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
	printf("hash(%u, %u) = %u\n",  entry,
			header->pages - sizeof(nvs_header_t),
			header->hash);

	printf("Saving header %u, %u, %u\n", header->count, header->pages, header->hash);

	 flash_range_erase(FLASH_WRITE_START, NVS_SIZE);
     flash_range_program(FLASH_WRITE_START, mem, size);

     free(mem);
     rollback();
     init();

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

	printf("Size %u, index %u, data %u, Pages %u\n", size, indexSize, dataSize, pagesSize);


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
		  if (xClean.count(it->first) == 0){
			  if (it->second->type != NVS_TYPE_ERASE){
				  count++;
			  } else {
				  count--;
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

    printf("Loading header %u, %u, %u\n", header->count, header->pages, header->hash);
    if (header->pages > NVS_SIZE ){
    	printf("NVS is corrupt\n");
    	return;
    }

	uint32_t hash = oat_hash((const char *)entry, header->pages - sizeof(nvs_header_t));
	printf("hash(%u, %u) = %u\n",  entry,
			header->pages - sizeof(nvs_header_t),
			hash);
	if (hash != header->hash){
		printf("Hash Check Failed\n");
		return;
	} else {
		printf("Hash matches\n");
		for (int i=0; i < header->count; i++){
			printf("Loading key %s\n", entry[i].key);
			xClean[entry[i].key] = entry;
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
