#include "TestData.h"


std::vector<std::string> mysqlstrs;

void export_data_to_test_table() {
    std::fstream newfile;

    newfile.open("testdata/testdata.txt", std::ios::in);
    if(newfile.is_open()) {
        std::string tp;
        std::string substr;
        while(getline(newfile, tp)) {
            // cout << tp << endl;
            // stringstream tpp(tp);
            // while(tpp.good()) {
            //     std::getline(tpp, substr, ',');
            //     res.push_back(substr);
            // }
            std::string sqlstr("INSERT INTO tb_daily_freeze_current_out values(");
            // for(auto i : res) {
            //     sqlstr += i;
    
            //     // std::cout << i << " ";
            // }
            sqlstr += tp;
            sqlstr += ");";
            // std::cout << sqlstr;
            // std::cout << std::endl;
            mysqlstrs.push_back(sqlstr);
        }
        newfile.close();
    }
}