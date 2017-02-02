#pragma once

#include "shared.h"

class tag_storage : public service_base {
public:
	virtual void save(const serialized_tags_dict_t& serialized_tags_dict) = 0;
	virtual void fetch(serialized_tags_dict_t* out) = 0;
};
