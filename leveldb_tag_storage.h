#pragma once

#include "tag_storage.h"

class leveldb_tag_storage : public tag_storage {
public:
	void save_tags(serialized_tags_dict_t serialized_tags_dict) override;
};
