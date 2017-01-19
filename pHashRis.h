#ifndef PHASHRIS_PHASHRIS_H
#define PHASHRIS_PHASHRIS_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/extended_p_square.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include "HashStore.h"

using namespace std;
using namespace boost::accumulators;

template<typename H, double (*HashDistance)(H, H), H (*HashFunction)(const string&)>
class pHashRis {
public:
    typedef H Hash;
    typedef typename HashStore<Hash, HashDistance>::Entry HashEntry;

    /**
     * Construct a pHashRis Application.
     * @param index_path Path to the index file.
     */
    pHashRis(const string &index_path, double threshold) :
            index_path_(index_path),
            threshold_(threshold) {
    }

    virtual ~pHashRis() {}

    /**
     * Load the index.
     */
    void LoadIndex() {
        store_.Load(index_path_);
    }

    /**
     * Sort and save the index.
     */
    void SaveIndex() {
        store_.Sort();
        store_.Save(index_path_);
    }

    /**
     * Index a set of image.
     * If the given path is a directory, index all images in it.
     * @param path A path to an image or a directory.
     */
    void Index(const string& path) {
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

    /**
     * Search for a set of image into the index.
     * If the given path is a directory, search for all images in it.
     * @param path A path to an image or a directory.
     */
    void Search(const string& path) const {
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

    /**
     * Display statistical information about the distances between the indexed images.
     */
    void DisplayIndexStatistics() const {
        const vector<HashEntry>& entries = store_.Entries();

        boost::array<double, 2> quantile_probs = {0.25, 0.75};
        accumulator_set<double, stats<tag::min,
                tag::median(with_p_square_quantile),
                tag::max,
                tag::extended_p_square,
                tag::mean,
                tag::variance> > acc(tag::extended_p_square::probabilities = quantile_probs);

        for (unsigned int i = 0; i < entries.size(); i++) {
            for (unsigned int j = i + 1; j < entries.size(); j++) {
                double dist = HashDistance(entries[i].hash, entries[j].hash);
                // Push distance to the accumulator.
                acc(dist);
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
    }

    /**
     * Display the distance between the indexed images.
     * The image pairs whose distance is greater than max_distance are not displayed.
     * @param max_distance Maximum distance between displayed image pair.
     */
    void DisplayDistances(const double max_distance = 1.0) const  {
        const vector<HashEntry>& entries = store_.Entries();

        #pragma omp parallel for shared(entries)
        for (unsigned int i = 0; i < entries.size(); i++) {
            for (unsigned int j = i + 1; j < entries.size(); j++) {
                double dist = HashDistance(entries[i].hash, entries[j].hash);
                if (dist <= max_distance) {
                    #pragma omp critical
                    {
                        cout << entries[i].file_path << "\t" << entries[j].file_path << "\t" << dist << endl;
                    }
                }
            }
        }
    }

private:
    /**
     * List all image files in a directory.
     * @param dir_path A path to a directory.
     * @return An array of all image files in the given directory.
     */
    vector<string> ListImages(const string &dir_path) const {
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

    /**
     * Index all files given in an array.
     * @param files An array of image files to index.
     */
    void IndexFiles(const vector<string>& files) {
        Hash file_hash;
        #pragma omp parallel for private(file_hash)
        for (unsigned int i = 0; i < files.size(); i++) {
            file_hash = HashFunction(files[i]);
            #pragma omp critical
            {
                store_.Add(file_hash, files[i]);
            }
        }
    }

    /**
     * Search for many image files into the index.
     * @param files An array of image files to search.
     */
    void SearchFiles(const vector<string>& files) const {
        Hash file_hash;
        #pragma omp parallel for private(file_hash)
        for (unsigned int i = 0; i < files.size(); i++) {
            file_hash = HashFunction(files[i]);

            vector<pair<double, HashEntry> > result = store_.Search(file_hash, threshold_);
            #pragma omp critical
            {
                cout << "Query : " << files[i].c_str() << "\n";
                cout << "Files found : " << result.size() << "\n";
                for (typename vector<pair<double, HashEntry> >::const_iterator it = result.begin();
                     it != result.end(); ++it) {
                    cout << it->first << " : " << it->second.file_path << "\n";
                }
                cout << endl;
            }
        }
    }

    /**
     * Path to the index file.
     */
    string index_path_;

    /**
     * Threshold for the search.
     */
    double threshold_;

    HashStore<Hash, HashDistance> store_;
};


#endif //PHASHRIS_PHASHRIS_H
