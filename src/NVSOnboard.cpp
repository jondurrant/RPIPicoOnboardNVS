/*
 * NVSOnboard.cpp
 *
 *  Created on: 7 Dec 2023
 *      Author: jondurrant
 */

#include "NVSOnboard.h"

NVSOnboard::NVSOnboard() {
	// TODO Auto-generated constructor stub

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

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::erase_all(){

	return NVS_FAIL;
}

nvs_err_t NVSOnboard::commit(){


	return NVS_FAIL;
}

nvs_err_t NVSOnboard::rollback(){
	map<string, nvs_entry_t *>::iterator it = xDirty.begin();
	while (it != xDirty.end()){
		 nvs_entry_t * entry = it->second;
		 free(entry->value);
		 free(entry);
		 xDirty.erase(it->first);
		 //it++;
	}
	return NVS_OK;
}


nvs_err_t NVSOnboard::validKey(const char* key){
	if ((strlen(key) > 0) && (strlen(key) < NVS_MAX_KEY_LEN)){
		return NVS_OK;
	}
	return NVS_ERR_INVALID_NAME;
}


unsigned int NVSOnboard::numKeys(){
	return 0;
}

bool NVSOnboard::isDirty(){
	return  !xDirty.empty();
}
