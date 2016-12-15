#ifndef PHASHRIS_HASHSTORE_H
#define PHASHRIS_HASHSTORE_H

#include <string>
#include <vector>
#include "pHash.h"

using namespace std;

class HashStore {
public:
    struct Entry {
        ulong64 hash;
        string file_path;
    };

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
     * Return a const vector of all entries in the HashStore.
     * @return A const vector of all entries in the HashStore.
     */
    const vector<Entry>& Entries() const;

    /**
     * Add a file to the store.
     * @param file_hash The file hash.
     * @param file_path The file path.
     * @return True if the file has been added, false otherwise.
     */
    void Add(ulong64 file_hash, const string &file_path);

    /**
     * Search for the nearest files in the store.
     * This function uses a parallel sequential search.
     * @param file_hash The query hash.
     * @return A vector with all the nearest files and the distance to the query file.
     */
    pair<vector<Entry>, int> SearchNearest(const ulong64 file_hash) const;

private:
    vector<Entry> entries_;
};

#endif //PHASHRIS_HASHSTORE_H
