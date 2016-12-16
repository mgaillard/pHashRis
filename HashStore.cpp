#include <iostream>
#include <fstream>
#include "pHash.h"
#include "Hamming.h"
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

vector<pair<int, HashStore::Entry> > HashStore::Search(const ulong64 file_hash, const int threshold) const {
    vector<pair<int, HashStore::Entry> > result;

    for (unsigned int i = 0; i < entries_.size(); i++) {
        int dist = hamming_distance(file_hash, entries_[i].hash);
        if (dist <= threshold) {
            result.push_back(make_pair(dist, entries_[i]));
        }
    }

    return result;
}

vector<pair<int, HashStore::Entry> > HashStore::ParallelSearch(const ulong64 file_hash, const int threshold) const {
    vector<pair<int, HashStore::Entry> > result;

    #pragma omp parallel for shared(result)
    for (unsigned int i = 0; i < entries_.size(); i++) {
        int dist = hamming_distance(file_hash, entries_[i].hash);
        if (dist <= threshold) {
            #pragma omp critical
            {
                result.push_back(make_pair(dist, entries_[i]));
            }
        }
    }

    return result;
}