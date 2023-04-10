// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <algorithm> 
#include <iostream> 
#include <vector> 
#include <map>
#include <unordered_map>
#include "TableEntry.h"
#include "silly.h"

using namespace std;

unordered_map<string, Table> alltables;

bool quiet_mode = false;

void create(string tablename){

    if(alltables.count(tablename) != 0) {
        cout << "Error during" << "CREATE: Cannot create already existing table " << tablename << " \n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else alltables[tablename].create(tablename);

    // /*DEBUG*/ cout << "[DEBUG] " << "CREATED " << tablename << "\n";
}

void remove(string tablename){

    if(alltables.count(tablename) == 0) {
        cout << "Error during " << "REMOVE: " << tablename << " does not name a table in the database \n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else { //cerr
        alltables.erase(tablename);
        cout << "Table " << tablename << " deleted\n";
    }
}
void quit() {

    alltables.clear();
    cout << "Thanks for being silly!\n";
}

// figures what cols to be printed
// print all
// print where --> col name make sure exists, operation(char) to move past it, 4 way split on the val type of the column
// print where helper--> has to do 3 way split based on the operation
// loop through every row, see if the col matches
// reuse it in print all, print where
// duplicate --> 4 loop, if statement, call to a function

void print(string tablename){

    if(alltables.count(tablename) == 0) {
        //Error during <COMMAND>: <tablename> does not name a table in the database
        cout << "Error during " << "PRINT: " << tablename << " does not name a table in the database \n" ;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    else {
        alltables[tablename].print();
    }    
}

void insert(string tablename) {

    if(alltables.count(tablename) == 0) {
        cout << "Error during" << " INSERT: " << tablename << " does not name a table in the database \n" ;
    
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // for(uint32_t i = 0 ; i < num_of_rows; i ++) {
        //     getline(cin, junk);
        // }
        return;
    }
    else{
        alltables[tablename].insert();
    }
}

void delete_(string tablename) {

    if(alltables.count(tablename) == 0) {
        cout << "Error during " << "DELETE: " << tablename << " does not name a table in the database \n" ;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    else { 
        alltables[tablename].delete_row();
    } 
}

void generate(string tablename){
    if(alltables.count(tablename) == 0) {
        cout << "Error during " << "GENERATE: " << tablename << " does not name a table in the database \n" ;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    else { 
        alltables[tablename].generate();
    } 
}

void Table::create(string tablename){
    
    uint32_t num_col;
    cin >> num_col;
    
    this->tablename = tablename;
    // this->quiet = 
    
    type_of_the_entry.reserve(num_col);
    
    // cout << "CREATE g_indextype = " << g_indextype << "\n"; 
    
    for(uint32_t i = 0; i < num_col; i ++) {

        string typestr;
        cin >> typestr;

        if(typestr == "string") {
            this->type_of_the_entry.push_back(EntryType::String);
        }
        else if(typestr == "int"){
            this->type_of_the_entry.push_back(EntryType::Int);
        }
        else if(typestr == "double"){
            this->type_of_the_entry.push_back(EntryType::Double);
        }
        else if(typestr == "bool"){
            this->type_of_the_entry.push_back(EntryType::Bool);
        }
    }

    cout << "New table " << this->tablename << " with column(s) " ;
    
    // string col_name;
    colnames.reserve(num_col);

    for(uint32_t i = 0; i < num_col; i ++) {

        string col_name;
        cin >> col_name;
        this->colnames.push_back(col_name);
        alltables_w_idx[col_name] = i;
        cout << col_name << " ";
    }

    cout << "created\n";

}


void join(string tablename1){

   if(alltables.count(tablename1) == 0) {
        cout << "Error during " << "JOIN: " << tablename1 << " does not name a table in the database \n" ;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    else alltables[tablename1].join();
}

void Table::join() {

    string tablename2;
    string junk;
    string colname1;
    string colname2;
    uint32_t num_of_rows;

    cin >> junk >> tablename2 ;

    if(alltables.count(tablename2) == 0) {
        cout << "Error during JOIN: " << tablename2 << " does not name a table in the database \n" ;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin >> junk >> colname1 ;
    if(this->alltables_w_idx.count(colname1) == static_cast<uint32_t>(0)) {
        cout << "Error during JOIN: " << colname1 << " does not name a column in " << this->tablename <<" \n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin >> junk >> colname2 ;
    if(alltables[tablename2].alltables_w_idx.count(colname2) == static_cast<uint32_t>(0)) {
        cout << "Error during JOIN: " << colname2 << " does not name a column in " << alltables[tablename2].tablename <<" \n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    uint32_t index2 = alltables[tablename2].alltables_w_idx[colname2];

    unordered_map<TableEntry, vector<uint32_t>> table2_map;

    for(uint32_t i = 0 ; i < static_cast<uint32_t>(alltables[tablename2].entry.size()) ; i ++) {

        TableEntry ent(alltables[tablename2].entry[i][index2]);
        table2_map[ent].push_back(i);
    }
    // AND PRINT <N> store
    cin >> junk >> junk >> num_of_rows ;

    // cout << "num of rows = " << num_of_rows << "\n";

    uint32_t index1 = this->alltables_w_idx[colname1];
    // !!!!!!!!!!!! <print_colname1> <1|2> <print_colname2> <1|2> ... <print_colnameN> <1|2>

    vector<string> colnames;
    vector<int> one_or_two;
    
    for(uint32_t i = 0 ; i < num_of_rows ; i ++) {

        string colname_;
        int one_two;

        cin >> colname_ >> one_two;
        colnames.push_back(colname_);
        one_or_two.push_back(one_two);
        
    }

    if(!quiet_mode) {

        for(uint32_t i = 0 ; i < num_of_rows ; i ++) {
            cout << colnames[i] << " ";
        }
        cout << "\n";
    }

    
    size_t count = 0;

    for(uint32_t i = 0 ; i < static_cast<uint32_t>(entry.size()) ; i ++) {

        if(table2_map[entry[i][index1]].size() != 0) {

            count += table2_map[entry[i][index1]].size();

            if(!quiet_mode) {

                for(uint32_t j = 0 ; j < static_cast<uint32_t>(table2_map[entry[i][index1]].size()) ; j ++) {

                    uint32_t matched = table2_map[entry[i][index1]][j];

                    for(uint32_t t = 0 ; t < static_cast<uint32_t>(colnames.size()) ; t ++) {

                        uint32_t temp_index;

                        if(one_or_two[t] == 1) {
                            temp_index = alltables_w_idx[colnames[t]];
                            cout << entry[i][temp_index] << " ";
                        }
                        else if(one_or_two[t] == 2) {
                            temp_index = alltables[tablename2].alltables_w_idx[colnames[t]];
                            cout << alltables[tablename2].entry[matched][temp_index] << " ";
                        }

                    }
                    cout << "\n";
                }
            }
        }
    }

    //Printed 3 rows from joining pets to 281class
    cout << "Printed " << count << " rows from joining " << this->tablename << " to " << tablename2  <<"\n";
}

void Table::print(){

    // cout << "PRINT STARTED\n";

    uint32_t num_rows;

    cin >> num_rows; // 3
    // cout << "[DEBUG] rows = " << num_rows << "\n";

    string colname;
    vector<string> pcolnames;
    vector<uint32_t> colnames_w_idx;

    for(uint32_t i = 0 ; i < num_rows; i ++) {

        cin >> colname;
        // keep reading the column
        pcolnames.push_back(colname); // emotion person Y/N
        if(this->alltables_w_idx.count(colname) == static_cast<uint32_t>(0)) {
            cout << "Error during PRINT: " << colname << " does not name a column in " << this->tablename <<" \n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        colnames_w_idx.push_back(alltables_w_idx[colname]);
    }

    cin >> colname;

    if(colname == "WHERE") {
        // sort(colnames_w_idx.begin(), colnames_w_idx.end());
        this->print_where(colnames_w_idx);
    }

    else if(colname == "ALL") {
        this->print_all(colnames_w_idx);
    }    
}

void Table::print_all(const vector<uint32_t>& cols) {

    vector<uint32_t> getindex;
    // size_t count = entry.size();

    if(!quiet_mode) {

        for(uint32_t i = 0 ; i < static_cast<uint32_t>(cols.size()) ; i ++) {

            cout << colnames[cols[i]] << " ";
        }
        cout << "\n";

        for(uint32_t i = 0 ; i < static_cast<uint32_t>(this->entry.size()) ; i ++ ) {

            for(uint32_t j = 0 ; j < cols.size() ; j ++) {

                cout << this->entry[i][cols[j]] << " " ;
            }
            cout << "\n";
        }
    }

    size_t count = entry.size();
    cout << "Printed "<< count << " matching rows from " << this->tablename << "\n";
    
}

// template <typename T>
void Table::print_where_operations(const vector<uint32_t>& pw_colnames, uint32_t idx, char op, const TableEntry & ent) {

    size_t count = 0;

    switch(op) {

        case '<': {

            if(bst_col == idx && type_of_the_entry[idx] != EntryType::Bool) {

                for (auto it = bstmap.begin(); it != bstmap.lower_bound(ent); ++it) {

					count += it->second.size();

                    if(!quiet_mode) {

                        for (uint32_t i = 0; i < static_cast<uint32_t>(it->second.size()); ++i) {

                            for (uint32_t c = 0; c < static_cast<uint32_t>(pw_colnames.size()); ++c) {

                                cout << entry[it->second[i]][pw_colnames[c]] << " ";
                                
                            }
                            cout << "\n";

                        }
                    }
				}
                //true is greater than false

            }

            else if(bst_col == idx && type_of_the_entry[idx] == EntryType::Bool) {

                // if(type_of_the_entry[idx] == EntryType::Bool) {

                    for(uint32_t i = 0 ; i < static_cast<uint32_t>(entry.size()); i ++) {

                        // sort(pw_colnames.begin(), pw_colnames.end(), uint32_t);
                        // TableEntry val(entry[i][idx]);

                        if(entry[i][idx] == false) {

                            if(ent == true) {

                                count += 1;

                                if(!quiet_mode) {

                                    for(uint32_t j = 0 ; j < static_cast<uint32_t>(pw_colnames.size()) ; j ++) {
                                        cout << this->entry[i][pw_colnames[j]] << " " ;
                                    }
                                    cout << "\n";
                                }
                            }
                        }
                    }
                // }
            }

            else {
                
                for(uint32_t i = 0 ; i < static_cast<uint32_t>(entry.size()); i ++) {

                    // sort(pw_colnames.begin(), pw_colnames.end(), uint32_t);
                    TableEntry val(entry[i][idx]);

                    if(val < ent) {

                        count += 1;

                        if(!quiet_mode) {

                            for(uint32_t j = 0 ; j < static_cast<uint32_t>(pw_colnames.size()) ; j ++) {
                                cout << this->entry[i][pw_colnames[j]] << " " ;
                            }
                            cout << "\n";
                        }
                    }
                }
            }
            
            break;
        }
        case '>': {

            if(bst_col == idx && type_of_the_entry[idx] != EntryType::Bool) {

                for (auto it = bstmap.upper_bound(ent); it != bstmap.end(); ++it) {

					count += it->second.size();

                    if(!quiet_mode) {

                        for (uint32_t i = 0; i < static_cast<uint32_t>(it->second.size()); ++i) {
                            for (uint32_t c = 0; c < static_cast<uint32_t>(pw_colnames.size()); ++c) {
                                cout << entry[it->second[i]][pw_colnames[c]] << " ";
                            }
                            cout << "\n";
                        }
                    }
				}
            }

            else if(bst_col == idx && type_of_the_entry[idx] == EntryType::Bool) {

                // if(type_of_the_entry[idx] == EntryType::Bool) {

                    for(uint32_t i = 0 ; i < static_cast<uint32_t>(entry.size()); i ++) {

                        // sort(pw_colnames.begin(), pw_colnames.end(), uint32_t);
                        // TableEntry val(entry[i][idx]);

                        if(entry[i][idx] == true) {

                            if(ent == false) {
                                
                                count += 1;

                                if(!quiet_mode) {

                                    for(uint32_t j = 0 ; j < static_cast<uint32_t>(pw_colnames.size()) ; j ++) {
                                        cout << this->entry[i][pw_colnames[j]] << " " ;
                                    }
                                    cout << "\n";
                                }
                            }
                        }
                    }
                // }
            }
            else {
                for(uint32_t i = 0 ; i < static_cast<uint32_t>(entry.size()); i ++) {

                    TableEntry val(entry[i][idx]);

                    if(val > ent) {

                        count += 1;

                        if(!quiet_mode) {

                            for(uint32_t j = 0 ; j < static_cast<uint32_t>(pw_colnames.size()) ; j ++) {
                                cout << this->entry[i][pw_colnames[j]] << " " ;
                            }
                            cout << "\n";
                        }
                    }
                }
            }
            
            break;
        }
        case '=': {

            // if(g_colname == colnames[idx]) {

            if(type_hash == indexType::hash && hash_col == idx) {

                // TableEntry ent(val);

                count += hashtable[ent].size();

                if(!quiet_mode) {

                    for(uint32_t i = 0 ; i < static_cast<uint32_t>(hashtable[ent].size()) ; i ++ ) {
                        for(uint32_t j = 0 ; j < static_cast<uint32_t>(pw_colnames.size()) ; j ++) {
                            cout << this->entry[hashtable[ent].at(i)][pw_colnames[j]] << " " ;
                        }
                        cout << "\n";
                    }
                }
                
            }

            else {

                for(uint32_t i = 0 ; i < static_cast<uint32_t>(entry.size()); i ++) {

                    TableEntry val(entry[i][idx]);

                    if(val == ent) {

                        count += 1;

                        if(!quiet_mode) {

                            for(uint32_t j = 0 ; j < static_cast<uint32_t>(pw_colnames.size()) ; j ++) {
                                cout << this->entry[i][pw_colnames[j]] << " " ;
                            }
                            cout << "\n";
                        }
                    }
                }

            }
            break;
        }

        default : {	
            count = 0;	
        }
    }

    cout << "Printed "<< count << " matching rows from " << this->tablename << "\n";
}

void Table::print_where(const vector<uint32_t>& pcolnames){

    string colname;
    char op;

    cin >> colname;
    
    if(alltables_w_idx.count(colname) == 0) {
        cout << "Error during PRINT: " << colname << " does not name a column in " << this->tablename <<" \n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    uint32_t index = alltables_w_idx[colname]; 

    if(!quiet_mode) {

        for(uint32_t i = 0 ; i < static_cast<uint32_t>(pcolnames.size()) ; i ++) {
            cout << colnames[pcolnames[i]] << " ";
        }
        cout << "\n";
    }

    switch(type_of_the_entry[index]){

        case EntryType::String : {

            string value;
            cin >> op >> value;

            TableEntry val(value);

            this->print_where_operations(pcolnames, index, op, val);
            break;
        }
        case EntryType::Bool : {

            bool value;
            cin >> op >> value;
            TableEntry val(value);
            this->print_where_operations(pcolnames, index, op, val);
            break;
        }
        case EntryType::Double : {

            double value;
            cin >> op >> value;
            TableEntry val(value);
            this->print_where_operations(pcolnames, index, op, val);
            break;
        }
        case EntryType::Int : {

            int value;
            cin >> op >> value;
            TableEntry val(value);
            this->print_where_operations(pcolnames, index, op, val);
            break;
        }
    }
}

void Table::generate(){
//hash and bst index
    //GENERATE FOR <tablename> <hash/bst> INDEX ON <colname>
    //GENERATE FOR 281class hash INDEX ON emotion

    //hash 
    //the program should create a hash table 
    //that allows a row in the table to be found rapidly 
    //given a particular value in the column <colname>

    //bst
    //the program should create a binary search tree 
    //that allows rows in the table to be found rapidly 
    //given a particular value in the column <colname>

    //  If a valid index is requested on a table that already has one, 
    // discard the old index before building the new one

    //count

    string indextype;
    
    string junk;
    string colname;
    cin >> indextype >> junk >> junk >> colname ;

    if(alltables_w_idx.count(colname) == 0) {
        cout << "Error during GENERATE: " << colname << " does not name a column in " << this->tablename <<" \n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    uint32_t index = alltables_w_idx[colname];

    switch(indextype[0]) {

        case 'h':{

            hashtable.reserve(entry.size());
            // bstmap.clear();
            hashtable.clear();

            // uint32_t e_size = entry.size();

            for(uint32_t i = 0 ; i < entry.size() ; i ++) {
                // TableEntry t_entry(entry[i][index]);
                hashtable[entry[i][index]].push_back(i);
            }
            hash_col = index;
            type_hash = indexType::hash;

            break;
        }

        case 'b' :{

            bstmap.clear();

            for(uint32_t i = 0 ; i < entry.size() ; i ++) {
                // TableEntry t_entry(entry[i][index]);
                //bst[table[r][it - colnames.begin()]].push_back(r);
                bstmap[entry[i][index]].push_back(i);
            }
            bst_col = index;
            type_bst = indexType::bst;

            break;

        }
    }
    // Created <indextype> index for table <tablename> on column <colname>
    cout << "Created " << indextype << " index for table " << this->tablename << " on column " << colname << "\n";

}

void Table::insert(){

    // cout << "\n[DEBUG] INSERT STARTED\n";

    //add to the end --> easy to update the index

    // alltables[tablename].entry
    // resize 2d vector
    // insert 10 rows--> resize it to 10
    // loop throiugh row 0 less than 10
    // remember the existing num of rows-> old numof rows
    // another insert 5 rows --> resize it to 15 (oldsize(do not make it variable 2d vector knows its size just make a local variable)+newsize)
    // loop through old number to less than new number(data.size)
    // data[row].reserve(#of col)
    // for loop for every new col
        // 4 way split
        // switch Entry::String:
        //    
        //data[row]-->contains table entry I have a string
        //push back is when type match
        // data[row].emplace_back(s) --> use when types don't match but thers a constructor that changes the type

    uint32_t num_of_rows;
    string junk;
    cin >> num_of_rows >> junk;
    
    uint32_t old_size = static_cast<uint32_t>(entry.size()) ;

    this->entry.resize(old_size + num_of_rows);

    for(uint32_t i = old_size ; i < old_size + num_of_rows; i ++) {

        entry[i].reserve(colnames.size());

        for(uint32_t j = 0 ; j < colnames.size() ; j ++){

            switch(type_of_the_entry[j]){

                case EntryType::String: {
                    string values;
                    cin >> values;
                    //cout << values << " ";
                    this->entry[i].emplace_back(values); 
                    break;
                }
                case EntryType::Int: {
                    int values;
                    cin >> values;
                    //cout << values << " ";
                    this->entry[i].emplace_back(values); 
                    break;
                }
                case EntryType::Bool: {
                    bool values;
                    cin >> values;
                    //cout << values << " ";
                    this->entry[i].emplace_back(values); 
                    break;
                }
                case EntryType::Double: {
                    double values;
                    cin >> values;
                    //cout << values << " ";
                    this->entry[i].emplace_back(values); 
                    break;
                }
                default :{
                    break;
                }
            }
        }
    }

    if (type_bst == indexType::bst) {

        bstmap.clear();

        for(uint32_t i = 0 ; i < entry.size() ; i ++) {

			bstmap[entry[i][bst_col]].push_back(i);
        }
	}

    if (type_hash == indexType::hash) {
            // cout << "[insert]1WHY HERE?\n";
        hashtable.reserve(entry.size());
        hashtable.clear();

        for(uint32_t i = 0 ; i < entry.size() ; i ++) {
			hashtable[entry[i][hash_col]].push_back(i);
        }
            
	}
    
	

    cout << "Added " << num_of_rows << " rows to " << tablename << " from position " << old_size << " to " << num_of_rows + old_size - 1 << "\n";
    old_size = old_size + num_of_rows;
}


void Table::delete_where_operations(uint32_t index, char op, const TableEntry & val){

    // int count = 0;

    switch(op) {

        case '<': {
            // cout << "delete delete_where_operations:  op = " << op << ", value = " << val << "\n";
            //  && type_of_the_entry[idx] != EntryType::Bool

            entry.erase(remove_if(entry.begin(), entry.end(), Less(val, index)), entry.end());
            break;
        }
        case '>': {

            // cout << "delete delete_where_operations:  op = " << op << ", value = " << val << "\n";
            entry.erase(remove_if(entry.begin(), entry.end(), Greater(val, index)), entry.end());
            break;
        }
        case '=': {

            // cout << "delete delete_where_operations:  op = " << op << ", value = " << val << "\n";
            entry.erase(remove_if(entry.begin(), entry.end(), EqualTo(val, index)), entry.end());
            break;
        }
    }
}

void Table::delete_row(){

    //careful : col%
    //when regenerate on the original generate command 

    // remove if 
    // have index --> call.clear
    // no easy way to fix the index

    // 4 way split and read like a string/int/.. then call helper
    // need helper function 
    // --> what the operation is table entry, comparison, what column number 
    // --> 3 way split with the operation
    // -->create right functor for 3 operations
    //--> call remove if --> iterator where it left off --> 
    //-->functor will be struct
    //-->must be 3 --> faster
    //--> return how many rows are deleted but could be void
    
    // DELETE FROM <tablename> WHERE <colname> <OP> <value>

    string junk; //age < 20;
    string colname;
    char op;

    cin >> colname ; //need to read in value;

    if(alltables_w_idx.count(colname) == 0) {
        cout << "Error during DELETE: " << colname << " does not name a column in " << this->tablename <<" \n";
        getline(cin, junk);
        return;
    }

    uint32_t index = alltables_w_idx[colname];
    uint32_t prev_size = static_cast<uint32_t>(entry.size());
    // cout << "before [DELETE] entry size = " << entry.size() << "\n";

    TableEntry ty(colnames[index]);
    // size_t k = bstmap[ty].size();

    // cout << "before [DELETE] bstmap  = " << bstmap.size() << "\n";

    switch(type_of_the_entry[index]){

        case EntryType::String : {
            //call helper function that does three way operator split
            string value;
            cin >> op >> value;
            TableEntry val(value);
            // cout << "[DELETE] bstmap size = " << bstmap.size() << "\n";
            // cout << "delete WHERE[STRING] op = " << op << ", value = " << value << "\n";
            this->delete_where_operations(index, op, val);
            // cout << "entry size = " << entry.size() << "\n";
            // cout << "bst = " << bst << "\n";
            // cout << "bst col = " << bst_col << "\n";
            // cout << "col = " << colname << "\n";
            
            if(type_hash == indexType::hash) {
                
                hashtable.reserve(entry.size());
                hashtable.clear();
                // bstmap.clear();

                // uint32_t n_index = alltables_w_idx[hash_col];
                
                for(uint32_t i = 0 ; i < entry.size(); i ++) {
                    // cout << "entry[i][index] = " << entry[i][index] << "\n";
                    // TableEntry ent(entry[i][hash_col]);
                    hashtable[entry[i][hash_col]].push_back(i);
                }
            }
            if(type_bst == indexType::bst) {

                // cout << "!![DELETE] bstmap size = " << bstmap[ty][0] << "\n";
                // cout << "!!after [DELETE] entry size = " << entry.size() << "\n";

                bstmap.clear();
                // uint32_t n_index = bst_col;
                
                for(uint32_t i = 0 ; i < entry.size(); i ++) {

                    // TableEntry ent(entry[i][bst_col]);
                    bstmap[entry[i][bst_col]].push_back(i);
                    // bstmap[entry[i][index]].push_back(i);
                    // cout << "!!after [DELETE] bstmap size = " << bstmap[ent].size() << "\n";
                }
                // cout << "!!after [DELETE] bstmap size = " << bstmap[ent].size() << "\n";

                // cout << "[DELETE] bstmap size = " << bstmap.size() << "\n";
            }
            //rehash?????
            break;
        }
        case EntryType::Bool : {

            bool value;
            cin >> op >> value;
            TableEntry val(value);
            // cout << "delete WHERE[BOOL] op = " << op << ", value = " << value << "\n";
            this->delete_where_operations(index, op, val);
            
            if(type_hash == indexType::hash) {
                
                hashtable.reserve(entry.size());

                hashtable.clear();
                // bstmap.clear();
                // uint32_t n_index = hash_col;

                // uint32_t index = alltables_w_idx[colname];
                
                for(uint32_t i = 0 ; i < entry.size(); i ++) {
                    // cout << "entry[i][index] = " << entry[i][index] << "\n";
                    TableEntry ent(entry[i][hash_col]);
                    hashtable[ent].push_back(i);
                }
            }
            if(type_bst == indexType::bst) {

                bstmap.clear();
                // uint32_t n_index = bst_col;
                
                for(uint32_t i = 0 ; i < entry.size(); i ++) {
                    TableEntry ent(entry[i][bst_col]);
                    bstmap[ent].push_back(i);
                }
            }

            break;
        }
        case EntryType::Double : {

            double value;
            cin >> op >> value;
            TableEntry val(value);
            // cout << "delete WHERE[DOUBLE] op = " << op << ", value = " << value << "\n";
            this->delete_where_operations(index, op, val);

            if(type_hash == indexType::hash) {
                
                hashtable.reserve(entry.size());

                hashtable.clear();
                // uint32_t n_index = hash_col;
                
                for(uint32_t i = 0 ; i < entry.size(); i ++) {
                    // cout << "entry[i][index] = " << entry[i][index] << "\n";
                    TableEntry ent(entry[i][hash_col]);
                    hashtable[ent].push_back(i);
                }
            }
            if(type_bst == indexType::bst) {

                bstmap.clear();
                // uint32_t n_index = alltables_w_idx[bst_col];

                for(uint32_t i = 0 ; i < entry.size(); i ++) {
                    TableEntry ent(entry[i][bst_col]);
                    bstmap[ent].push_back(i);
                }
            }

            break;
        }
        case EntryType::Int : {

            int value;
            cin >> op >> value;
            TableEntry val(value);
            // cout << "delete WHERE[INT] op = " << op << ", value = " << value << "\n";
            this->delete_where_operations(index, op, val);

            if(type_hash == indexType::hash) {
                
                hashtable.reserve(entry.size());

                hashtable.clear();
                // uint32_t n_index = hash_col;
                
                for(uint32_t i = 0 ; i < entry.size(); i ++) {
                    // cout << "entry[i][index] = " << entry[i][index] << "\n";
                    TableEntry ent(entry[i][hash_col]);
                    hashtable[ent].push_back(i);
                }
            }
            if(type_bst == indexType::bst) {

                bstmap.clear();
                // hashtable.clear();

                // uint32_t index = alltables_w_idx[colname];
                // uint32_t n_index = alltables_w_idx[bst_col];
                
                for(uint32_t i = 0 ; i < entry.size(); i ++) {
                    TableEntry ent(entry[i][bst_col]);
                    bstmap[ent].push_back(i);
                }
            }

            break;
        }
    }

    // cout << "[DELETE] bstmap size = " << bstmap.size() << "\n";
    // cout << "[DELETE] entry size = " << entry.size() << "\n";

    cout << "Deleted "<< prev_size - entry.size() << " rows from " << this->tablename << "\n";
    
}

int main(int argc, char**argv)
{

    ios_base::sync_with_stdio(false); 
    cin >> boolalpha;  
    cout << boolalpha; 

    int c;

    while (1) {

        int option_index = 0;
        static struct option long_options[] = {
            {"quiet",  no_argument, 0, 'q'},
            {"help", no_argument, 0, 'h'},
        };

        c = getopt_long(argc, argv, "qh",
                long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            case 'q':
                quiet_mode = true;
                break;
            case 'h':

                cout << "This program is a basic relational database with an interface based on a subset of a standard query language." << '\n'
                << "Command line arguments are as follows: " << '\n'
                << "'-q' or '--quiet': The program will run very similarly to normal, except that it will print only numerical summaries of the rows affected by the command, not any of the actual data." << '\n'
                << "'-h' or '--help': Prints the message about how to use the program and ends." << '\n';
                exit(0);
                break;

            default:
                exit(1);
        }
    }

    // vector<Table> alltables;

    string junk;
    string commands;
    string tablename;
    string tablename1;
    string tablename2;
    // uint32_t num_of_rows;

    do{

        cout << "% " ;
        cin >> commands;

        if (cin.fail()) {
            cerr << "Error: Reading from cin has failed" << endl;
            exit(1);
        } // if

        // cout << "COMMANDS: " << commands << "\n";

        switch(commands[0]) {

            case '#': {
                // string junk;
                // getline(cin, junk);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                //cin.ignore<>
                // cout << "";
                break;
            }

            case 'Q': {
                quit();
                break;
            }

            case 'C': {
                // int num_of_col = 0;
                cin >> tablename ;
                // cout << tablename << "\n";
                create(tablename);
                break;
            }
            
            case 'R': {
                cin >> tablename ;
                remove(tablename);
                break;
            }
            
            case 'I': {
                // num_of_rows = 0;
                cin >> junk >> tablename;
                insert(tablename);
                break;
            }

            case 'P': {
            //don't have to use functor -- much easier without it
            //print calls print_where and just manually do it
            //for loop if statement print 

                // num_of_rows = 0;
                cin >> junk >> tablename;
                
                print(tablename);
                break;
            }
            
            case 'D': {
            //have to use functor to use STL
            //union for delete

                // int num_of_rows = 0;
                cin >> junk >> tablename >> junk;
                // cout << junk << tablename ;
                //FROM students WHERE age < 20;
                delete_(tablename);
                // alltables[tablename].delete_row(tablename , num_of_rows);
                break;
            }
            
            case 'J': {
                // num_of_rows = 0;
                cin >> tablename1 ;
                join(tablename1);
                break;
            }

            case 'G': {
                cin >> junk >> tablename;
                //GENERATE FOR <tablename> <indextype> INDEX ON <colname>
                generate(tablename);
                // getline(cin, junk);
                break;
            }
            default :
                cout << "Error: unrecognized command\n";
                // string junk;
                // getline(cin, junk);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
        }

    } while(commands != "QUIT");


    return 0;
    
}
