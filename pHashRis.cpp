#include "pHashRis.h"
#include <iostream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/extended_p_square.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <algorithm>

using namespace std;
using namespace boost::accumulators;

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

void pHashRis::DisplayIndexStatistics() const {
    const vector<HashStore::Entry>& entries = store_.Entries();

    boost::array<double, 2> quantile_probs = {0.25, 0.75};
    accumulator_set<long long, stats<tag::min,
                               tag::median(with_p_square_quantile),
                               tag::max,
                               tag::extended_p_square,
                               tag::mean,
                               tag::variance> > acc(tag::extended_p_square::probabilities = quantile_probs);

    // Distance can take values from 0 to 64.
    const int MAX_DISTANCE = 64;
    array<long long, MAX_DISTANCE+1> histogram;
    histogram.fill(0);

    #pragma omp parallel for shared(entries, acc, histogram)
    for (unsigned int i = 0; i < entries.size(); i++) {
        for (unsigned int j = i + 1; j < entries.size(); j++) {
            int dist = ph_hamming_distance(entries[i].hash, entries[j].hash);
            // Push distance to the accumulator and histogram.
            #pragma omp critical(statistics)
            {
                acc(dist);
                histogram.at(dist)++;
            }
        }
    }

    cout << "Min: " << extract_result<tag::min>(acc) << endl
         << "First quartile: " << extract_result<tag::extended_p_square>(acc)[0] << endl
         << "Median: " << extract_result<tag::median(with_p_square_quantile)>(acc) << endl
         << "Last quartile: " << extract_result<tag::extended_p_square>(acc)[1] << endl
         << "Max: " << extract_result<tag::max>(acc) << endl
         << endl
         << "Mean: " << extract_result<tag::mean>(acc) << endl
         << "Variance: " << extract_result<tag::variance>(acc) << endl
         << "Standard deviation: " << sqrt(extract_result<tag::variance>(acc)) << endl
         << endl;

    cout << "Histogram:" << endl;
    for (int i = 0; i <= MAX_DISTANCE; i++) {
        cout << i << "\t" << histogram[i] << endl;
    }
}

void pHashRis::DisplayDistances(const int max_distance) const {
    const vector<HashStore::Entry>& entries = store_.Entries();

    #pragma omp parallel for shared(entries)
    for (unsigned int i = 0; i < entries.size(); i++) {
        for (unsigned int j = i + 1; j < entries.size(); j++) {
            int dist = ph_hamming_distance(entries[i].hash, entries[j].hash);
            if (dist <= max_distance) {
                #pragma omp critical(output)
                {
                    cout << entries[i].file_path << "\t" << entries[j].file_path << "\t" << dist << endl;
                }
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
            pair<vector<HashStore::Entry>, int> result = store_.SearchNearest(file_hash);

            #pragma omp critical(output)
            {
                cout << "Query : " << files[i].c_str() << "\n";
                cout << "Found " << result.first.size() << " files\n";
                for (vector<HashStore::Entry>::const_iterator it = result.first.begin();
                     it != result.first.end(); ++it) {
                    cout << "\t" << it->file_path << "\n";
                }
                cout << "Distance = " << result.second << "\n";
                cout << endl;
            }
        } else {
            cout << "Error while calculating the hash of the file " << files[i] << "." << endl;
        }
    }
}