#ifndef PHASHRIS_PHASHRIS_H
#define PHASHRIS_PHASHRIS_H

#include <vector>
#include "HashStore.h"

class pHashRis {
public:
    /**
     * Construct a pHashRis Application.
     * @param index_path Path to the index file.
     */
    pHashRis(const string &index_path);

    virtual ~pHashRis();

    /**
     * Load the index.
     */
    void LoadIndex();

    /**
     * Save the index.
     */
    void SaveIndex();

    /**
     * Index a set of image.
     * If the given path is a directory, index all images in it.
     * @param path A path to an image or a directory.
     */
    void Index(const string& path);

    /**
     * Search for a set of image into the index.
     * If the given path is a directory, search for all images in it.
     * @param path A path to an image or a directory.
     */
    void Search(const string& path) const;

    /**
     * Display statistical information about the distances between the indexed images.
     */
    void DisplayIndexStatistics() const;

    /**
     * Display the distance between the indexed images.
     * The image pairs whose distance is greater than max_distance are not displayed.
     * @param max_distance Maximum distance between displayed image pair.
     */
    void DisplayDistances(const int max_distance = 64) const;

private:
    /**
     * List all image files in a directory.
     * @param dir_path A path to a directory.
     * @return An array of all image files in the given directory.
     */
    vector<string> ListImages(const string &dir_path) const;

    /**
     * Index all files given in an array.
     * @param files An array of image files to index.
     */
    void IndexFiles(const vector<string>& files);

    /**
     * Search for many image files into the index.
     * @param files An array of image files to search.
     */
    void SearchFiles(const vector<string>& files) const;

    /**
     * Path to the index file.
     */
    string index_path_;

    HashStore store_;
};


#endif //PHASHRIS_PHASHRIS_H
