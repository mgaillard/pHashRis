#ifndef PHASHRIS_HASHSTORE_H
#define PHASHRIS_HASHSTORE_H

#include <string>
#include <map>
#include "pHash.h"

using namespace std;

class HashStore {
public:
    HashStore();

    /**
     * Load the index data from a file.
     * @param filename A path to a file.
     */
    void Load(const string &filename);

    /**
     * Save the index data to a file.
     * @param filename A path to a file.
     */
    void Save(const string &filename);

    /**
     * Add a file to the store.
     * @param file_hash The file hash.
     * @param file_path The file path.
     * @return True if the file has been added, false otherwise.
     */
    bool Add(ulong64 file_hash, const string &file_path);

    /**
     * Search for the nearest file in the store.
     * This function is thread-safe.
     * @param file_hash The query hash.
     * @return A pair with the path to the nearest file and the distance to the query file.
     */
    pair<string, int> SearchNearest(ulong64 file_hash) const;

private:
    map<ulong64, string> _entries;
};

#endif //PHASHRIS_HASHSTORE_H
