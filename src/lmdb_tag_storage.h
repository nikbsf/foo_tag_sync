#pragma once

#include "tag_storage.h"

class lmdb_tag_storage : public tag_storage {
public:
	void save(const serialized_tags_dict_t& serialized_tags_dict) override;
	serialized_tags_dict_t* fetch(serialized_tags_dict_t& existing) override;
};
