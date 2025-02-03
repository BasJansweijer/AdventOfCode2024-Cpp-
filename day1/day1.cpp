#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ranges>
using namespace std;

int main() {
    // std::cout << "hello world" << std::endl;
    std::ifstream input("large.input");
    std::string input_line;

    int left, right;
    std::vector<int> list1;
    std::vector<int> list2;

 
    while (input >> left >> right){
        list1.push_back(left);
        list2.push_back(right);
    }

    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    int sum = 0;

    for (size_t i = 0; i < list1.size(); ++i){
        sum += abs(list1[i] - list2[i]);
    }

    std::cout << "abs diff: " << sum << std::endl;

    // Part 2
    int similarity = 0;
    for (int i : list1){
        similarity += i * std::count(list2.begin(), list2.end(), i);
    }

    std::cout << "similarity: " << similarity << std::endl;
    
    return 0;
}