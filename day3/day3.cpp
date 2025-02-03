#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

int parse_mul(const std::string &text, size_t mul_pos) {
    if (mul_pos == std::string::npos) {
        return 0;
    }

    int end_i = mul_pos;
    while (end_i < text.size() && (std::isdigit(text[end_i]) || text[end_i]== ',' || text[end_i] == ')')) {
        if (text[end_i] == ')'){
            break;
        }
        ++end_i;
    }

    if (text.at(end_i) != ')') {
        return 0;
    }

    std::string_view mul_expr = text.substr(mul_pos, end_i - mul_pos);
    std::istringstream args (mul_expr.data());
    int a,b;
    char comma;
    args >> a >> comma >> b;
    if (comma != ',') {
        return 0;
    }

    return a*b;
}

int main() {
    std::ifstream input_f("large.input");
    std::stringstream input_ss;

    input_ss << input_f.rdbuf();
    std::string input = input_ss.str();

    size_t mul_pos =  -1;
    const size_t mul_offset = 4;
    size_t do_not_pos = -1;
    size_t do_pos = -1;


    int total = 0;
    int total_w_do = 0;

    bool do_mul = true;

    do {
        mul_pos =  input.find("mul(", mul_pos + 1);
        do_pos = input.rfind("do", mul_pos+1);
        do_not_pos = input.rfind("don't", mul_pos+1);

        if (do_pos != std::string::npos && do_pos > do_not_pos) {
            do_mul = true;
        }
        else if (do_not_pos != std::string::npos) {
            do_mul = false;
        }

        int mul_res = parse_mul(input, mul_pos + mul_offset);
        total += mul_res;
        if (do_mul) {
            total_w_do += mul_res;
        }
    } while (mul_pos != std::string::npos);

    std::cout << total << std::endl;
    std::cout << total_w_do << std::endl;
}
