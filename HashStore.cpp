#include <iostream>
#include <limits>
#include <fstream>
#include "pHash.h"

#include "HashStore.h"

using namespace std;

HashStore::HashStore() {

}

void HashStore::Load(const string &filename) {
    ifstream file(filename, ios::in);

    if (file) {
        ulong64 file_hash;
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

void HashStore::Save(const string &filename) {
    ofstream file(filename, ios::out | ios::trunc);

    if (file) {
        vector<HashStore::Entry>::const_iterator itEntry;
        for (itEntry = entries_.begin(); itEntry != entries_.end(); ++itEntry) {
            file << itEntry->hash << " " << itEntry->file_path << endl;
        }

        file.close();
    } else {
        cout << "Impossible to save the index file." << endl;
    }
}

const vector<HashStore::Entry>& HashStore::Entries() const {
    return entries_;
}

void HashStore::Add(ulong64 file_hash, const string &file_path) {
    HashStore::Entry entry;
    entry.hash = file_hash;
    entry.file_path = file_path;

    entries_.push_back(entry);
}

pair<vector<HashStore::Entry>, int> HashStore::SearchNearest(const ulong64 file_hash) const {
    vector<HashStore::Entry> result;
    int min_dist = numeric_limits<int>::max();

    for (unsigned int i = 0; i < entries_.size(); i++) {
        int dist = ph_hamming_distance(file_hash, entries_[i].hash);
        {
            if (dist < min_dist) {
                min_dist = dist;
                result.clear();
                result.push_back(entries_[i]);
            } else if (dist == min_dist) {
                result.push_back(entries_[i]);
            }
        }
    }

    return make_pair(result, min_dist);
}

pair<vector<HashStore::Entry>, int> HashStore::ParallelSearchNearest(const ulong64 file_hash) const {
    vector<HashStore::Entry> result;
    int min_dist = numeric_limits<int>::max();

    #pragma omp parallel for shared(min_dist, result)
    for (unsigned int i = 0; i < entries_.size(); i++) {
        int dist = ph_hamming_distance(file_hash, entries_[i].hash);
        #pragma omp critical(search_min)
        {
            if (dist < min_dist) {
                min_dist = dist;
                result.clear();
                result.push_back(entries_[i]);
            } else if (dist == min_dist) {
                result.push_back(entries_[i]);
            }
        }
    }

    return make_pair(result, min_dist);
}