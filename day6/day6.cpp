#include <iostream>
#include <fstream>
#include <sstream>

static int s_width;
static int s_height;

bool in_bounds(int row, int col) {
    return row >= 0 && row < s_height && col >= 0 && col < s_width;
}

char read_board(std::string_view board, int row, int col) {
    if (!in_bounds(row, col)) {
        return '\0';
    }

    return board[(s_width + 1) * row + col];
}

void set_board(std::string &board, int row, int col, char c) {
    if (!in_bounds(row, col)) {
        return;
    }

    board[(s_width + 1) * row + col] = c;
}

bool visited(char c) {
    return c == '^' || c == '<' || c == '>' || c == 'v' || c == '\0';
}

class Guard {
    int dir_c, dir_r;

    void turn() {
        if (dir_c == 0) {
            dir_c = -dir_r;
            dir_r = 0;
        }else {
            dir_r = dir_c;
            dir_c = 0;
        }
    }

    bool same_dir_as_char(char c) {
        return c == '^' && dir_r == -1 
            || c == '<' && dir_c == -1
            || c == '>' && dir_c == 1
            || c == 'v' && dir_r == 1;
    }

  public:
    bool in_loop;
    int row, col;
    int visit_count;
    Guard(int start_i):
      dir_c(0),
      dir_r(-1),
      row(start_i / (s_width+1)),
      col(start_i % (s_width+1)),
      visit_count(1),
      in_loop(false) {
    }

    void move(std::string_view board) {
        read_board(board, row, col);

        int new_r = row + dir_r;
        int new_c = col + dir_c;

        char new_spot;

        while ((new_spot = read_board(board, new_r, new_c)) == '#') {
            turn();
            new_r = row + dir_r;
            new_c = col + dir_c;
        }
        row = new_r;
        col = new_c;

        if (same_dir_as_char(new_spot)) {
            in_loop = true;
        }

        visit_count += !visited(new_spot);
    }

    char cur_dir() {
        if (dir_c == 0 && dir_r == 1) {
            return 'v';
        }else if (dir_c == 0 && dir_r == -1) {
            return '^';
        }else if (dir_c == 1 && dir_r == 0) {
            return '>';
        }else {
            return '<';
        }
        

    }
};


void simulate_board(std::string & board, Guard &g) {
    while (in_bounds(g.row, g.col) && !g.in_loop) {
        set_board(board, g.row, g.col, g.cur_dir());
        g.move(board);
    }
}

// Determines wether the blocker would influence the path
bool could_influence_path(std::string_view board_result, int row, int col) {
    char left, right, up, down;
    left = read_board(board_result, row, col - 1);
    right = read_board(board_result, row, col + 1);
    up = read_board(board_result, row - 1, col);
    down = read_board(board_result, row + 1, col);

    return visited(left) || visited(right) || visited(up) || visited(down);
}

int main () {
    std::ifstream input_f ("large.input");
    std::stringstream ss;
    ss << input_f.rdbuf();
    std::string empty_board = ss.str();
    std::string board = empty_board;
    s_width = board.find('\n');
    s_height = (board.length() + 1) / (s_width+1);

    int guard_i = board.find('^');
    Guard g(guard_i);

    simulate_board(board, g);
    std::cout << "part 1: " << g.visit_count << std::endl;

    int loop_blockers = 0;

    for (int r = 0; r < s_height; r++) {
        for (int c = 0; c < s_width; c++) {

            if (!could_influence_path(board, r, c)) {
                continue;
            }

            Guard guard(guard_i);
            std::string new_board = empty_board;
            set_board(new_board, r, c, '#');
            simulate_board(new_board, guard);
            loop_blockers += guard.in_loop;
        }
    }

    std::cout << loop_blockers << std::endl;
}