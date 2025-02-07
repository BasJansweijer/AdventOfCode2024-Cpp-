#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>

static int s_width;
static int s_height;

bool in_bounds(int row, int col) {
    return row < s_height && row >= 0 && col < s_width && col >= 0;
}

char get_char(std::string_view string, int row, int col) {
    if (!in_bounds(row, col)) {
        return '\0';
    }
    return string.at(row * (s_width + 1) + col);
}

void set_char(std::string &string, int row, int col, char c) {
    if (!in_bounds(row, col)) {
        return;
    }
    string[row * (s_width + 1) + col] = c;
}

std::pair<int, int> index_to_row_col(int index) {
    int row = index / (s_width + 1);
    int col = index % (s_width + 1);
    return std::pair<int, int>(row, col);
}

std::vector<std::pair<int,int>> find_antenas(char antena_type, std::string_view field) {
    std::vector<std::pair<int,int>> positions;

    size_t index = -1;
    do {
        index = field.find(antena_type, index+1);
        int row = index / (s_width + 1);
        int col = index % (s_width + 1);
        positions.emplace_back(row, col);
    } while (index != std::string::npos);

    return positions;
}

void find_char_antinodes(char antena_type, std::string_view field, std::string& anti_node_locations) {
    std::vector<std::pair<int,int>> antena_locs = find_antenas(antena_type, field);

    for (std::pair<int,int> a1 : antena_locs) {
        for (std::pair<int,int> a2 : antena_locs) {
            if (a1 == a2) {
                continue;
            }

            int row_diff = a1.first - a2.first;
            int col_diff = a1.second - a2.second;

            // location 1
            int anti_node1_row = a1.first + row_diff;
            int anti_node1_col = a1.second + col_diff;
            set_char(anti_node_locations, anti_node1_row, anti_node1_col, '#');

            // location 2
            int anti_node2_row = a2.first - row_diff;
            int anti_node2_col = a2.second - col_diff;
            set_char(anti_node_locations, anti_node2_row, anti_node2_col, '#');
        }
    }
}

void set_anti_nodes_in_line(int row, int col, int row_diff, int col_diff, std::string& anti_node_locations) {
    int an_row = row;
    int an_col = col;
    while (in_bounds(an_row, an_col)) {
        set_char(anti_node_locations, an_row, an_col, '#');
        an_row -= row_diff;
        an_col -= col_diff;
    }

    an_row = row + row_diff;
    an_col = col + col_diff;
    while (in_bounds(an_row, an_col)) {
        set_char(anti_node_locations, an_row, an_col, '#');
        an_row += row_diff;
        an_col += col_diff;
    }
}

void find_char_antinodes_part2(char antena_type, std::string_view field, std::string& anti_node_locations) {
    std::vector<std::pair<int,int>> antena_locs = find_antenas(antena_type, field);

    for (std::pair<int,int> a1 : antena_locs) {
        for (std::pair<int,int> a2 : antena_locs) {
            if (a1 == a2) {
                continue;
            }

            int row_diff = a1.first - a2.first;
            int col_diff = a1.second - a2.second;

            set_anti_nodes_in_line(a1.first, a1.second, row_diff, col_diff, anti_node_locations);
        }
    }
}

int main() {
    std::ifstream file("large.input");
    std::stringstream ss;
    ss << file.rdbuf();
    std::string field = ss.str();
    s_width = field.find('\n');
    s_height = (field.size() + 1) / (s_width + 1);

    // will be updated with * on every anti node location.
    std::string anti_node_locations = field;

    std::set<char> charachters(field.begin(), field.end());
    charachters.erase('.');
    charachters.erase('\n');

    for (char c : charachters) {
        find_char_antinodes(c, field, anti_node_locations);
    }

    int num_anti_nodes = std::count(anti_node_locations.begin(), anti_node_locations.end(), '#');
    std::cout << num_anti_nodes << std::endl;

    for (char c : charachters) {
        find_char_antinodes_part2(c, field, anti_node_locations);
    }

    num_anti_nodes = std::count(anti_node_locations.begin(), anti_node_locations.end(), '#');
    std::cout << num_anti_nodes << std::endl;
}