#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>

void show_mem(const std::vector<int>& mem) {
    for (int i : mem) {
        if (i == -1) {
            std::cout << '.';
        }else {
            std::cout << i;
        }
    }
    std::cout << std::endl;
}

int main () {
    std::ifstream file("large.input");
    std::stringstream ss;
    ss << file.rdbuf();
    std::string chars = ss.str();

    std::vector<int> mem;

    std::stack<int> seen_file_blocks;

    bool is_file = true;
    int f_id = 0;
    for (char c : chars) {
        int num = c - '0';
        for (int i = 0; i < num; i++) {
            if (is_file) {
                seen_file_blocks.push(f_id);
                mem.push_back(f_id);
            }else {
                mem.push_back(-1);
            }
        }

        is_file = !is_file;
        f_id += is_file;
    }

    int num_file_blocks = seen_file_blocks.size();

    for (int i = 0; i < num_file_blocks; i++) {
        if (mem[i] == -1) {
            mem[i] = seen_file_blocks.top();
            seen_file_blocks.pop();
        }
    }

    int64_t checksum = 0;
    for (int i = 0; i < num_file_blocks; i++) {
        if (mem[i] == -1) {
            break;
        }

        checksum += mem[i] * i;
    }

    std::cout << checksum << std::endl;
}