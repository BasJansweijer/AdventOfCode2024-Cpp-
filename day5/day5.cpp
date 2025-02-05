#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <set>

std::set<int> find_related(const std::vector<std::pair<int,int>>& rules, int num, bool isPredecessor) {
    std::set<int> related;
    for (std::pair<int,int> rule : rules) {
        if (isPredecessor && rule.first == num) {
            related.insert(rule.second);
        } else if (!isPredecessor && rule.second == num) {
            related.insert(rule.first);
        }
    }
    return related;
}

const std::pair<int,int> violated_rule(const std::vector<std::pair<int,int>>& rules, const std::vector<int>& ordering) {
    for (int i = 0; i < ordering.size(); i++) {
        std::set<int> forbidden_predecessors = find_related(rules, ordering[i], true);
        for (int forbidden : forbidden_predecessors) {
            if (std::count(ordering.begin(), ordering.begin() + i, forbidden) > 0) {
                // Not a correct order
                return {ordering[i], forbidden};
            }
        }
    }
    return {-1, -1};
}

bool correct_order(const std::vector<std::pair<int,int>>& rules, const std::vector<int>& ordering) {
    return violated_rule(rules, ordering).first == -1;
}

void rearange_order(std::vector<std::pair<int,int>> rules, std::vector<int>& ordering) {
    std::pair<int,int> bad_rule;
    while ((bad_rule = violated_rule(rules, ordering)).first != -1) {
        // We need to switch the first with the second

        int first_i = std::find(ordering.begin(), ordering.end(), bad_rule.first) - ordering.begin();
        int second_i = std::find(ordering.begin(), ordering.end(), bad_rule.second) - ordering.begin();
        ordering[first_i] = bad_rule.second;
        ordering[second_i] = bad_rule.first;
    }
}

int main () {
    std::ifstream input_f ("large.input");

    int num1, num2;
    char vbar;

    std::vector<std::pair<int, int>> rules;

    std::vector<std::vector<int>> incorrect_ordered_seqs;

    std::string line;
    while (std::getline(input_f, line)) {
        std::istringstream line_stream(line);
        if (line_stream >> num1 >> vbar >> num2 && vbar == '|') {
            rules.emplace_back(num1, num2);
        }else {
            break;
        }
    }
    
    int cur_num;
    char comma;

    int total = 0;

    while (std::getline(input_f, line)) {
        std::vector<int> order;
        std::istringstream line_stream(line);

        while (line_stream >> cur_num) {
            line_stream >> comma;
            order.push_back(cur_num);
        }

        if (!correct_order(rules, order)) {
            incorrect_ordered_seqs.push_back(order);
            continue;
        }

        // add middel number to total
        total += order[order.size()/2];
    }

    std::cout << "Total part 1: " << total << std::endl;

    total = 0;
    for (auto order : incorrect_ordered_seqs) {
        rearange_order(rules, order);
        total += order[order.size()/2];
    }

    std::cout << "Total part 2: " << total << std::endl;
}