#ifndef PHASHRIS_RIS_H
#define PHASHRIS_RIS_H

#include <string>

using namespace std;

/**
 * Generic Reverse Image Search class.
 */
class Ris {
public:
    virtual ~Ris() {}

    /**
     * Load the index.
     */
    virtual void LoadIndex() = 0;

    /**
     * Save the index.
     */
    virtual void SaveIndex() = 0;

    /**
     * Index a set of image.
     * If the given path is a directory, index all images in it.
     * @param path A path to an image or a directory.
     */
    virtual void Index(const string& path) = 0;

    /**
     * Search for a set of image into the index.
     * If the given path is a directory, search for all images in it.
     * @param path A path to an image or a directory.
     */
    virtual void Search(const string& path) const = 0;

    /**
     * Display statistical information about the distances between the indexed images.
     */
    virtual void DisplayIndexStatistics() const = 0;

    /**
     * Display the distance between the indexed images.
     * The image pairs whose distance is greater than max_distance are not displayed.
     * @param max_distance Maximum distance between displayed image pair.
     */
    virtual void DisplayDistances(const double max_distance = 1.0) const = 0;
};


#endif //PHASHRIS_RIS_H
