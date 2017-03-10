#ifndef RISBENCHMARKRESULT_QUERYRESULT_H
#define RISBENCHMARKRESULT_QUERYRESULT_H

#include <string>
#include <vector>

using namespace std;

class QueryResult {
public:
    QueryResult(const string &query_file);

    void AddResult(double dist, const string &result_file);

    string QueryType() const;

    unsigned long NumberRelevantRetrieved(double threshold) const;

    unsigned long NumberRetrieved(double threshold) const;

    void Print() const;

private:
    struct Result {
        double dist_;
        string file_;
    };

    string baseImage(const string &file) const;
    string typeImage(const string &file) const;

    string file_;
    vector<Result> results_;
};


#endif //RISBENCHMARKRESULT_QUERYRESULT_H
