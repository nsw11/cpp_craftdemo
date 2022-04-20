#include "stdafx.h"
#include <stdint.h>
#include <string>
#include <vector>
#include <thread>
#include <algorithm>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <ratio>

/**
    Represents a Record Object
*/
struct QBRecord
{
    uint column0; // unique id column
    std::string column1;
    long column2;
    std::string column3;
};

/**
Represents a Record Collections
*/
typedef std::vector<QBRecord> QBRecordCollection;


/**
    Return records that contains a string in the StringValue field
    records - the initial set of records to filter
    matchString - the string to search for
*/
QBRecordCollection QBFindMatchingRecords(const QBRecordCollection& records, const std::string& columnName, const std::string& matchString)// there was a typo on this line columnname only had 1 colon
    {
    //error handling-> make sure colname is valid

    QBRecordCollection result;
    int col = (int)(columnName[columnName.length()-1] -'0');

     std::copy_if(records.begin(), records.end(), std::back_inserter(result), [&](QBRecord rec){

// if you have more columns converting these if statements will most likely result in a speed-up but given only 4 columns, I found a slight slowdown when using a switch()
        if (col == 0) {
            uint matchValue = std::stoul(matchString);
            return matchValue == rec.column0;
        } else if (col == 1) {
            return rec.column1.find(matchString) != std::string::npos;
        } else if (col == 2) {
            long matchValue = std::stol(matchString);
            return matchValue == rec.column2;
        } else if (col == 3) {
            return rec.column3.find(matchString) != std::string::npos;
        } else {
            return false;
        }

    });
    return result;
    }



QBRecordCollection QBFindMatchingRecordsOriginal(const QBRecordCollection& records, const std::string& columnName, const std::string& matchString)
    {
    QBRecordCollection result;
    std::copy_if(records.begin(), records.end(), std::back_inserter(result), [&](QBRecord rec){
        if (columnName == "column0") {
            uint matchValue = std::stoul(matchString);
            return matchValue == rec.column0;
        } else if (columnName == "column1") {
            return rec.column1.find(matchString) != std::string::npos;
        } else if (columnName == "column2") {
            long matchValue = std::stol(matchString);
            return matchValue == rec.column2;
        } else if (columnName == "column3") {
            return rec.column3.find(matchString) != std::string::npos;
        } else {
            return false;
        }
    });
    return result;
    }


int DeleteRecordByID(QBRecordCollection records, int id){
    //vector.find_if(startiter, enditer, lambdafunction(id == record.column1))
    auto to_delete = std::find_if (records.begin(), records.end(), [&](QBRecord rec){
        return id == rec.column0;
    });// since this takes in a unique id there should only ever be 1 entry
    if (to_delete == records.end()){
        return 1;
    }
    else{
        records.erase(to_delete);
        return 0;
    }
    
}
/**
    Utility to populate a record collection
    prefix - prefix for the string value for every record
    numRecords - number of records to populate in the collection
*/
QBRecordCollection populateDummyData(const std::string& prefix, int numRecords)
    {
    QBRecordCollection data;
    data.reserve(numRecords);
    for (uint i = 0; i < numRecords; i++)
        {
        QBRecord rec = { i, prefix + std::to_string(i), i % 100, std::to_string(i) + prefix };
        data.emplace_back(rec);
        }
    return data;
    }

int main(int argc, _TCHAR* argv[])
{
    using namespace std::chrono;
    // populate a bunch of data
    double imp =0,swi=0,ori=0, rem=0, th =0;
    
    auto data = populateDummyData("testdata", 1000);

    auto startTimer = steady_clock::now();
    auto filteredSet = QBFindMatchingRecords(data, "column1", "testdata500");
    auto filteredSet2 = QBFindMatchingRecords(data, "column2", "24");
    std::cout << "profiler: " << double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;

    auto startTimer2 = steady_clock::now();
    auto filteredSet3 = QBFindMatchingRecordsOriginal(data, "column1", "testdata500");
    auto filteredSet4 = QBFindMatchingRecordsOriginal(data, "column2", "24");
    std::cout << "original profiler: " << double((steady_clock::now() - startTimer2).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;
    // make sure that the function is correct
    assert(filteredSet.size() == 1);
	return 0;
}

