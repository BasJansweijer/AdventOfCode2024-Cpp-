#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int find_report_error(vector<int> report) {
    bool all_increasing = (report[0] - report[1]) < 0;
    for (size_t i=0; i < (report.size()-1); ++i) {
        int diff = report[i] - report[i+1];
        bool increasing = diff < 0;
        diff = abs(diff);

        if (all_increasing != increasing || diff < 1 || diff > 3) {
            return i;
        }
    }

    return -1;
}

void copy_vector_without_i(vector<int> cur, vector<int>& cpy, int i) {
    for (size_t j = 0; j<cur.size(); ++j) {
        if (j == i) {
            continue;
        }

        cpy.push_back(cur[j]);
    }
}

bool is_report_save(vector<int> report) {
    return find_report_error(report) == -1;
}

void print_vec(vector<int> vec) {
    for (int i: vec) {
        cout << i << ",";
    }
    cout << endl;
}

bool report_save_with_1_removal(vector<int> report) {
    int first_error = find_report_error(report);
    if (first_error == -1) {
        return true;
    }

    vector<int> new_vec;
    copy_vector_without_i(report, new_vec, first_error);
    if (is_report_save(new_vec)) {
        return true;
    }

    new_vec.clear();
    copy_vector_without_i(report, new_vec, first_error-1);
    if (is_report_save(new_vec)) {
        return true;
    }

    new_vec.clear();
    copy_vector_without_i(report, new_vec, first_error+1);
    if (is_report_save(new_vec)) {
        return true;
    }

    return false;
}

int main(){
    ifstream input_f("large.input");

    // vector<vector<int>> reports;
    int num_save_reports = 0;

    int num_save_w_removal = 0;

    string line;
    while (getline(input_f, line)) {
        int num;
        istringstream line_stream(line);
        vector<int> report;
        while (line_stream >> num){
            report.push_back(num);
        }

        if (is_report_save(report)){
            ++num_save_reports;
        }

        if (report_save_with_1_removal(report)) {
            ++num_save_w_removal;
        }
    }

    cout << num_save_reports << endl;
    cout << num_save_w_removal << endl;

}