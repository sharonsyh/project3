// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <algorithm> 
#include <getopt.h>
#include <string>
#include <queue>
#include "TableEntry.h"

using namespace std;

class Table{

    public:

        vector<vector<TableEntry>> entry;
        vector<EntryType> type_of_the_entry;
        string tablename;
        vector<string> colnames; //size
        unordered_map<string, uint32_t> alltables_w_idx;
        unordered_map<TableEntry, vector<uint32_t>> hashtable; // for hash
        map<TableEntry, vector<uint32_t>> bstmap; // for bst
        
        void create(string);
        void generate();
        void print();
        void print_all(const vector<uint32_t>&);
        void print_where(const vector<uint32_t>&);
        // template <typename T>
        void print_where_operations(const vector<uint32_t>&, uint32_t, char, const TableEntry &);
        void delete_where_operations(uint32_t, char , const TableEntry & );
        void delete_row();
        void join();
        void insert();
        // uint32_t find_index(string);

        enum indexType { hash, bst, none };
        indexType type_hash = indexType::none;
        indexType type_bst = indexType::none;

        // string hash = "";
        // string bst = "";

        uint32_t hash_col = static_cast<uint32_t>(-1);
        uint32_t bst_col = static_cast<uint32_t>(-1);

};

class Less {

    public:

        TableEntry val;
        uint32_t index;

        Less(const TableEntry &v , uint32_t idx): val{v}, index{idx} {}// val index

        bool operator()(const vector<TableEntry> &row) 
        {
            return row[index] < val;
        }

};
//bst == --> work like hashmap
//3 way split for bst less-->bst.lowerbound
// greater -->iupperbounf
class EqualTo {

    public:

        TableEntry val;
        uint32_t index;

        EqualTo(const TableEntry &v , uint32_t idx): val{v}, index{idx} {}

        bool operator()(const vector<TableEntry> &row) // val index
        {
            return row[index] == val;
        }

};

class Greater {

    public:

        TableEntry val;
        uint32_t index;

        Greater(const TableEntry &v , uint32_t idx): val{v}, index{idx} {}

        bool operator()(const vector<TableEntry> &row) // val index
        {
            return row[index] > val;
        }

};



