#ifndef PHASHRIS_HASHSTORE_H
#define PHASHRIS_HASHSTORE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "Hamming.h"

using namespace std;

template<typename H, double (*HashDistance)(const H&, const H&)>
class HashStore {
public:
    typedef H Hash;

    struct Entry {
        Hash hash;
        string file_path;

        bool operator<(const Entry &other) const {
            return (file_path.compare(other.file_path) < 0);
        }
    };

    HashStore() {}

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
     * Sort the entries by hash and then by file_path.
     */
    void Sort() {
        sort(entries_.begin(), entries_.end());
    }

    /**
     * Return a const vector of all entries in the HashStore.
     * @return A const vector of all entries in the HashStore.
     */
    const vector<Entry>& Entries() const {
        return entries_;
    }

    /**
     * Add a file to the store.
     * @param file_hash The file hash.
     * @param file_path The file path.
     * @return True if the file has been added, false otherwise.
     */
    void Add(Hash file_hash, const string &file_path) {
        Entry entry;
        entry.hash = file_hash;
        entry.file_path = file_path;

        entries_.push_back(entry);
    }

    /**
     * Search in the store for the files that are nearer to the query than a threshold.
     * If the distance is equal to the threshold the file is selected.
     * This function uses a parallel sequential search.
     * @param file_hash The query hash.
     * @param threshold The maximum distance between the query and the files.
     * @return A vector with all the entries and their distances to the query in a pair.
     */
    vector<pair<double, Entry> > Search(const Hash file_hash, const double threshold) const {
        vector<pair<double, Entry> > result;

        for (unsigned int i = 0; i < entries_.size(); i++) {
            double dist = HashDistance(file_hash, entries_[i].hash);
            if (dist <= threshold) {
                result.push_back(make_pair(dist, entries_[i]));
            }
        }

        sort(result.begin(), result.end());

        return result;
    }

private:
    vector<Entry> entries_;
};

//-------------------------------------------------------------- Implementation
template<typename H, double (*HashDistance)(const H&, const H&)>
void HashStore<H, HashDistance>::Load(const string &filename) {
    ifstream file(filename, ios::in);

    if (file) {
        Hash file_hash;
        string file_path;

        while (file >> file_hash) {
            file.ignore(1, ' ');
            getline(file, file_path);

            Add(file_hash, file_path);
        }

        file.close();
    } else {
        cout << "Impossible to load the index file." << endl;
    }
}

template<typename H, double (*HashDistance)(const H&, const H&)>
void HashStore<H, HashDistance>::Save(const string &filename) {
    ofstream file(filename, ios::out | ios::trunc);

    if (file) {
        typename vector<HashStore<H, HashDistance>::Entry>::const_iterator itEntry;
        for (itEntry = entries_.begin(); itEntry != entries_.end(); ++itEntry) {
            file << itEntry->hash << " " << itEntry->file_path << endl;
        }

        file.close();
    } else {
        cout << "Impossible to save the index file." << endl;
    }
}

#endif //PHASHRIS_HASHSTORE_H
