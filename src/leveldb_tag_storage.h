#pragma once

#include "leveldb/db.h"

#include "tag_storage.h"

class leveldb_tag_storage : public tag_storage {
	leveldb::DB* get_db_connection();
public:
	void save(const serialized_tags_dict_t& serialized_tags_dict) override;	
	serialized_tags_dict_t* fetch(serialized_tags_dict_t& existing) override;
};
