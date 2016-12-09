#include <iostream>
#include <iterator>
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
        map<ulong64, string>::const_iterator itEntry;
        for (itEntry = entries_.begin(); itEntry != entries_.end(); ++itEntry) {
            file << itEntry->first << " " << itEntry->second << endl;
        }

        file.close();
    } else {
        cout << "Impossible to save the index file." << endl;
    }
}

const map<ulong64, string>& HashStore::Entries() const {
    return entries_;
}

bool HashStore::Add(ulong64 file_hash, const string &file_path) {
    pair<map<ulong64, string>::const_iterator, bool> result;

    result = entries_.insert(make_pair(file_hash, file_path));

    return result.second;
}

pair<string, int> HashStore::SearchNearest(ulong64 file_hash) const {
    map<ulong64, string>::const_iterator itNearest = entries_.begin();
    int min_dist = ph_hamming_distance(file_hash, itNearest->first);

    map<ulong64, string>::const_iterator itEntry;
    for (itEntry = next(entries_.begin()); itEntry != entries_.end(); ++itEntry) {
        int dist = ph_hamming_distance(file_hash, itEntry->first);

        if (dist < min_dist) {
            min_dist = dist;
            itNearest = itEntry;
        }
    }

    return make_pair(itNearest->second, min_dist);
}

