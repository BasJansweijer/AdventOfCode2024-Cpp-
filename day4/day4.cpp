#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

static int letters_per_line = -1;
static int lines = -1;

char get_char(std::string_view string, int row, int col) {
    if (row >= lines || row < 0 || col >= letters_per_line || col < 0) {
        return '\0';
    }
    return string.at(row * (letters_per_line + 1) + col);
}

int count_substring(std::string_view str, std::string_view sub_str){
    int count = 0;
    size_t pos = str.find(sub_str);

    while (pos != std::string::npos) {
        ++count;
        pos = str.find(sub_str, pos+1);
    }

    return count;
}

int count_horizontal(std::string_view input) {
    return count_substring(input, "XMAS") + count_substring(input, "SAMX");
}

int count_vertical(std::string_view input) {
    int count = 0;

    for (int col = 0; col < letters_per_line; ++col) {
        std::string col_str;
        for (int row = 0; row < lines; ++row) {
            col_str += get_char(input, row, col);
        }  
        
        count += count_substring(col_str, "XMAS") + count_substring(col_str, "SAMX");
    }

    return count;
}

int count_diagonal(std::string_view input) {
    int count = 0;

    for (int col = -lines; col < letters_per_line + lines; ++col) {
        std::string diag_left_str;
        std::string diag_right_str;

        int left_col = col;
        int right_col = col;

        for (int row = 0; row < lines; ++row) {
            char c = get_char(input, row, left_col);
            if (c != '\0'){
                diag_left_str += c;
            }

            c = get_char(input, row, right_col);
            if (c != '\0'){
                diag_right_str += c;
            }

            --left_col;
            ++right_col;
        }  
        
        count += count_substring(diag_left_str, "XMAS") + count_substring(diag_left_str, "SAMX");
        count += count_substring(diag_right_str, "XMAS") + count_substring(diag_right_str, "SAMX");
    }

    return count;
}

bool x_mas_at_pos(std::string_view input, int row, int col) {
    if (get_char(input, row, col) != 'A') {
        return false;
    }
    char ul = get_char(input, row-1, col-1);
    char ur = get_char(input, row-1, col+1);
    char dl = get_char(input, row+1, col-1);
    char dr = get_char(input, row+1, col+1);

    if (!((ul == 'M' && dr == 'S') || (ul == 'S' && dr == 'M'))) {
        return false;
    }

    if (!((dl == 'M' && ur == 'S') || (dl == 'S' && ur == 'M'))) {
        return false;
    }

    return true;
}

int main () {
    std::ifstream input_f ("large.input");
    std::stringstream input_s;
    input_s << input_f.rdbuf();
    std::string letters = input_s.str();
    letters_per_line = letters.find('\n');
    lines = (letters.size() + 1) / (letters_per_line + 1);

    int total = count_vertical(letters) + count_horizontal(letters) + count_diagonal(letters);

    std::cout << total << std::endl;

    int total_x_mas = 0;

    for (int r = 0; r < lines; ++r) {
        for (int c = 0; c < letters_per_line; ++c) {
            total_x_mas += x_mas_at_pos(letters, r, c);
        }
    }

    std::cout << total_x_mas << std::endl;

}