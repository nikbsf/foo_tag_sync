#pragma once

#include "shared.h"

class tag_storage : public service_base {
public:
	virtual void save(const serialized_tags_dict_t& serialized_tags_dict) = 0;
	virtual serialized_tags_dict_t* fetch(serialized_tags_dict_t& existing) = 0;
};
