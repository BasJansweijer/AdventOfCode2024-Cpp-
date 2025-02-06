#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <span>
#include <string>
#include <chrono>

void print_bits(int64_t num, int n=64) {
    std::cout << 'b';
    for (int i = 0; i<n; i++) {
        int bit = (num>>i)&1;
        std::cout << bit;
    }
    std::cout << std::endl;
}

class Timer {
public:
    Timer() : start_time(std::chrono::high_resolution_clock::now()) {}

    ~Timer() {
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;
        std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};

bool can_be_correct_part2_helper(int64_t ans, const std::span<const int> &nums, int64_t total) {
    if (nums.size() == 0) {
        return ans == total;
    }

    int64_t new_total;

    // Try +
    new_total = total + nums[0];
    if (can_be_correct_part2_helper(ans, nums.subspan(1), new_total)) {
        return true;
    }

    // Try *
    new_total = total * nums[0];
    if (can_be_correct_part2_helper(ans, nums.subspan(1), new_total)) {
        return true;
    }

    // Try ||
    std::string s = std::to_string(total);
    s += std::to_string(nums[0]);
    std::stringstream ss(s);
    ss >> new_total;
    if (can_be_correct_part2_helper(ans, nums.subspan(1), new_total)) {
        return true;
    }

    return false;
}

bool can_be_correct_part2(int64_t ans, const std::vector<int> &nums) {
    std::span<const int> full_span = nums;
    return can_be_correct_part2_helper(ans, full_span.subspan(1), nums[0]);
}

bool can_be_correct_helper(int64_t ans, const std::span<const int> &nums, int64_t total) {
    if (nums.size() == 0) {
        return ans == total;
    }

    int64_t new_total;

    // Try +
    new_total = total + nums[0];
    if (can_be_correct_helper(ans, nums.subspan(1), new_total)) {
        return true;
    }

    // Try *
    new_total = total * nums[0];
    if (can_be_correct_helper(ans, nums.subspan(1), new_total)) {
        return true;
    }

    return false;
}

bool can_be_correct_recursive(int64_t ans, const std::vector<int> &nums) {
    std::span<const int> full_span = nums;
    return can_be_correct_helper(ans, full_span.subspan(1), nums[0]);
}

bool can_be_correct(int64_t ans, const std::vector<int> &nums) {
    // Each bit represents an add/mult (0 is add 1 is mul)
    int64_t opperations = 0;
    // if we have n nums 2^n will set a bit which is too high to influence the opperations
    while (opperations < (1LL << nums.size())) {
        int64_t total = nums[0];
        for (int i = 0; i < nums.size() - 1; i++) {
            // get the ith opperation
            int opp = (opperations>>i)&1;
            int next_num = nums[i+1];
            if (opp) {
                // mult
                total *= next_num;
            } else {
                // add
                total += next_num;
            }
        }

        if (total == ans) {
            return true;
        }

        opperations += 1;
    }
    return false;
}

int64_t run_for_all(bool (*is_correct) (int64_t, const std::vector<int> &)) {
    std::ifstream file("large.input");
    std::string line;
    int64_t total = 0;

    while (std::getline(file, line)) {
        std::stringstream ss (line);

        int64_t ans;
        int num;
        char colon;
        ss >> ans >> colon;
        std::vector<int> nums;
        while (ss >> num) {
            nums.push_back(num);
        }


        if (is_correct(ans, nums)) {
            total += ans;
        }
    }

    return total;

}

int main () {
    int64_t total1, total2, total3;
    {
        Timer t;
        total2 = run_for_all(can_be_correct_recursive);
    }

    {
        Timer t;
        total1 = run_for_all(can_be_correct);
    }

    {
        Timer t;
        total3 = run_for_all(can_be_correct_part2);
    }

    std::cout << total1 << ',' << total2 << std::endl;
    std::cout << total3 << std::endl;

}