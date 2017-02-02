#include "stdafx.h"

#include "leveldb/options.h"
#include "leveldb/write_batch.h"

#include "leveldb_tag_storage.h"

leveldb::DB* leveldb_tag_storage::get_db_connection() {
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	options.compression = leveldb::kNoCompression;

	auto status = leveldb::DB::Open(options, "c:\\xxx_leveldb", &db);
	// todo check and throw

	return db;
}

void leveldb_tag_storage::fetch(serialized_tags_dict_t* out) {
	std::shared_ptr<leveldb::DB> db(get_db_connection());

	std::shared_ptr<leveldb::Iterator> it(db->NewIterator(leveldb::ReadOptions()));
	for (it->SeekToFirst(); it->Valid(); it->Next()) {
		auto key = it->key().ToString();
		auto value = it->value().ToString();

		if ((*out)[key] != value)
			(*out)[key] = value;
	}
}

void leveldb_tag_storage::save(const serialized_tags_dict_t& serialized_tags_dict) {
	std::shared_ptr<leveldb::DB> db(get_db_connection());
	leveldb::WriteOptions write_options;

	leveldb::WriteBatch batch;
	for (auto it = serialized_tags_dict.begin(); it != serialized_tags_dict.end(); ++it) {
		auto key = it->first;
		auto serialized_tags = it->second;
		batch.Put(key, serialized_tags);
	}

	db->Write(write_options, &batch);
}
