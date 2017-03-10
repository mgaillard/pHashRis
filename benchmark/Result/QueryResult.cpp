#include <iostream>
#include "QueryResult.h"

using namespace std;

QueryResult::QueryResult(const string &query_file) : file_(query_file) {

}

void QueryResult::AddResult(double dist, const string &result_file) {
    Result result;
    result.dist_ = dist;
    result.file_ = result_file;

    results_.push_back(result);
}

string QueryResult::QueryType() const {
    return typeImage(file_);
}

unsigned long QueryResult::NumberRelevantRetrieved(double threshold) const {
    unsigned long relevant_retrieved = 0;

    string query_base_image = baseImage(file_);
    for (unsigned int i = 0; i < results_.size(); i++) {
        if (results_[i].dist_ <= threshold) {
            string result_base_image = baseImage(results_[i].file_);
            if (query_base_image.compare(result_base_image) == 0) {
                relevant_retrieved++;
            }
        }
    }

    return relevant_retrieved;
}

unsigned long QueryResult::NumberRetrieved(double threshold) const {
    unsigned long retrieved = 0;

    for (unsigned int i = 0; i < results_.size(); i++) {
        if (results_[i].dist_ <= threshold) {
            retrieved++;
        }
    }

    return retrieved;
}

void QueryResult::Print() const {
    cout << "Query : " << file_ << "\n";
    cout << "Files found : " << results_.size() << "\n";
    for (vector<Result>::const_iterator it = results_.begin(); it != results_.end(); ++it) {
        cout << it->dist_ << " : " << it->file_ << "\n";
    }
}

string QueryResult::baseImage(const string &file) const {
    return file.substr(file.find_last_of('/') + 1);
}

string QueryResult::typeImage(const string &file) const {
    string path = file.substr(0, file.find_last_of('/'));
    return path.substr(path.find_last_of('/') + 1);
}