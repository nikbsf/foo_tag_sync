#include "stdafx.h"

#include <memory>

#include "leveldb/options.h"
#include "leveldb/db.h"

#include "leveldb_tag_storage.h"

void leveldb_tag_storage::save_tags(serialized_tags_dict_t serialized_tags_dict) {
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	options.compression = leveldb::kNoCompression;

	auto status = leveldb::DB::Open(options, "c:\\xxx_leveldb", &db);
	std::shared_ptr<leveldb::DB> dbptr(db);

	if (!status.ok())
		return;

	leveldb::WriteOptions writeOptions;
	for (auto it = serialized_tags_dict.begin(); it != serialized_tags_dict.end(); ++it) {
		auto key = it->first;
		auto serialized_tags = it->second;
		dbptr->Put(writeOptions, key.get_ptr(), serialized_tags.get_ptr());
	}
}
