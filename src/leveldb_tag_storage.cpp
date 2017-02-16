#include "stdafx.h"

#include "leveldb/options.h"
#include "leveldb/write_batch.h"

#include "leveldb_tag_storage.h"

extern advconfig_string_factory_MT cfg_storage_path;

leveldb::DB* leveldb_tag_storage::get_db_connection() {
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	options.compression = leveldb::kNoCompression;

	auto storage_path = get_setting(cfg_storage_path);
	if (storage_path.is_empty())
		throw fts_exception("Storage path not set");

	auto status = leveldb::DB::Open(options, storage_path.get_ptr(), &db);
	if (!status.ok())
		throw fts_exception(status.ToString());

	return db;
}

serialized_tags_dict_t* leveldb_tag_storage::fetch(serialized_tags_dict_t& existing) {
	std::shared_ptr<leveldb::DB> db(get_db_connection());
	auto result = new serialized_tags_dict_t();

	std::shared_ptr<leveldb::Iterator> it(db->NewIterator(leveldb::ReadOptions()));
	for (it->SeekToFirst(); it->Valid(); it->Next()) {
		auto key = it->key().ToString();
		auto value = it->value().ToString();

		if (existing[key] != value)
			(*result)[key] = value;
	}
	return result;
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
