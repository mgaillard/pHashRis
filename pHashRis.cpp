#include "pHashRis.h"
#include <iostream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>

const string INDEX_PATH = "index";

pHashRis::pHashRis(const string& index_path) : index_path_(index_path) {

}

pHashRis::~pHashRis() {

}

void pHashRis::LoadIndex() {
    store_.Load(index_path_);
}

void pHashRis::SaveIndex() {
    store_.Save(index_path_);
}

void pHashRis::Index(const string& path) {
    vector<string> files;

    struct stat info;
    if (stat(path.c_str(), &info) == 0) {
        // If it's a directory.
        if (S_ISDIR(info.st_mode)) {
            files = ListImages(path);
        }
            // Else if it's a regular file.
        else if (S_ISREG(info.st_mode)) {
            files.push_back(path);
        }
    }

    if (!files.empty()) {
        cout << "Indexing " << files.size() << " files..." << endl;
        IndexFiles(files);
        cout << "Done." << endl;
    }
}

void pHashRis::Search(const string& path) const {
    vector<string> files;

    struct stat info;
    if (stat(path.c_str(), &info) == 0) {
        // If it's a directory.
        if (S_ISDIR(info.st_mode)) {
            files = ListImages(path);
        }
            // Else if it's a regular file.
        else if (S_ISREG(info.st_mode)) {
            files.push_back(path);
        }
    }

    if (!files.empty()) {
        SearchFiles(files);
    }
}

void pHashRis::Distances(int max_distance) const {
    for (map<ulong64, string>::const_iterator itFirst = store_.Entries().begin();
         itFirst != store_.Entries().end(); ++itFirst) {
        for (map<ulong64, string>::const_iterator itSecond = next(itFirst);
             itSecond != store_.Entries().end(); ++itSecond) {
            int dist = ph_hamming_distance(itFirst->first, itSecond->first);
            if (dist <= max_distance) {
                cout << itFirst->second << "\t" << itSecond->second << "\t" << dist << endl;
            }
        }
    }
}

vector<string> pHashRis::ListImages(const string &dir_path) const {
    vector<string> files;

    DIR *dir = opendir(dir_path.c_str());
    if (dir != NULL) {
        struct dirent *dir_entry;
        while ((dir_entry = readdir(dir)) != NULL) {
            if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..")) {
                string file_path;
                if (dir_path.back() == '/') {
                    file_path = dir_path + dir_entry->d_name;
                } else {
                    file_path = dir_path + '/' + dir_entry->d_name;
                }

                // If it's a regular file.
                struct stat file_info;
                if (stat(file_path.c_str(), &file_info) == 0 && S_ISREG(file_info.st_mode)) {
                    files.push_back(file_path);
                }
            }
        }

        closedir(dir);
    } else {
        cout << "Error reading directory." << endl;
    }

    return files;
}

void pHashRis::IndexFiles(const vector<string>& files) {
    ulong64 file_hash;
    #pragma omp parallel for private(file_hash)
    for (unsigned int i = 0; i < files.size(); i++) {
        if (ph_dct_imagehash(files[i].c_str(), file_hash) >= 0) {
            #pragma omp critical(store_update)
            {
                store_.Add(file_hash, files[i]);
            }
        } else {
            cout << "Error while calculating the hash of the file " << files[i] << "." << endl;
        }
    }
}

void pHashRis::SearchFiles(const vector<string>& files) const {
    ulong64 file_hash;
    #pragma omp parallel for private(file_hash)
    for (unsigned int i = 0; i < files.size(); i++) {
        if (ph_dct_imagehash(files[i].c_str(), file_hash) >= 0) {
            pair<string, int> result = store_.SearchNearest(file_hash);
            #pragma omp critical(output)
            {
                cout << "Query : " << files[i].c_str() << endl
                     << "Found : " << result.first << endl
                     << "distance = " << result.second << endl
                     << endl;
            }
        } else {
            cout << "Error while calculating the hash of the file " << files[i] << "." << endl;
        }
    }
}