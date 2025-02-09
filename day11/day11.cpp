#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <unordered_map>

void blink(std::list<int64_t>& stones) {
    for (auto it = stones.begin(); it != stones.end(); it++) {
        int64_t stone = *it;
        if (stone == 0) {
            *it = 1;
        } else if (std::to_string(stone).size() % 2 == 0) {
            int substr_len = std::to_string(stone).size() / 2;
            std::string num_str = std::to_string(stone);
            int64_t left = std::stoi(num_str.substr(0, substr_len));
            int64_t right = std::stoi(num_str.substr(substr_len));
            *it = right;
            stones.insert(it, left);
        } else {
            *it = stone * 2024;
        }
    }
}

int64_t countStoneRecursive(int64_t stone, int remainingSteps, std::unordered_map<std::string, int64_t>& memoizationMap) {
    std::string name = std::to_string(stone) + 'r' + std::to_string(remainingSteps);
    auto it = memoizationMap.find(name);
    if (it != memoizationMap.end()) {
        return it->second;
    }

    int64_t res;
    if (remainingSteps == 0) res =  1;
    else if (stone == 0) res = countStoneRecursive(1, remainingSteps-1, memoizationMap); 
    else if (std::to_string(stone).size() % 2 == 0) {
        int substr_len = std::to_string(stone).size() / 2;
        std::string num_str = std::to_string(stone);
        int64_t left = std::stoi(num_str.substr(0, substr_len));
        int64_t right = std::stoi(num_str.substr(substr_len));
        
        res = countStoneRecursive(left, remainingSteps-1, memoizationMap) 
            + countStoneRecursive(right, remainingSteps-1, memoizationMap);
    } else {
        res = countStoneRecursive(stone * 2024, remainingSteps-1, memoizationMap);
    }

    memoizationMap.insert({name, res});
    return res;
}

int main() {
    std::ifstream file("large.input");
    int num;
    std::list<int64_t> stones;

    while (file >> num) {
        stones.push_back(num);
    }
    
    int blinks = 25;
    // for (int i = 0; i < blinks; i++) {
    //     blink(stones);
    // }

    std::unordered_map<std::string, int64_t> memMap;

    int total = 0;
    for (int stone : stones) {
        total += countStoneRecursive(stone, 25, memMap);
    }

    std::cout << total << std::endl;

    int part2_blinks = 75;
    int64_t total2 = 0;
    for (int stone : stones) {
        total2 += countStoneRecursive(stone, 75, memMap);
    }


    std::cout << total2 << std::endl;
}