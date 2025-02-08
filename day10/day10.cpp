#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class StringGrid{
    std::string content;
public:
    int width, height;
public:
    StringGrid(std::string&& values) 
       : width(values.find('\n')),
         height((values.size() + 1) / (width + 1)),
         content(values)
    {}

    bool inBounds(int row, int col) const {
        return row < height && row >= 0 && col < width && col >= 0;
    }

    char read(int row, int col) const {
        if (!inBounds(row, col)) return '\0';
        
        return content[getIndex(row, col)];
    }

    void set(int row, int col, char c) {
        if (!inBounds(row, col)) return;
        
        content[getIndex(row, col)] = c;
    }

    std::string str() {
        return content;
    }

private:
    int getIndex(int row, int col) const {
        return row * (width + 1) + col;
    }
};

int countTrails(const StringGrid& grid, int curRow, int curCol, int prevNum, StringGrid& visited) {
    if (visited.read(curRow, curCol) == 'X') return 0;

    int curNum = grid.read(curRow, curCol) - '0';
    
    // no trail
    if (prevNum + 1 != curNum) return 0;

    // found complete trail
    if (curNum == 9) {
        visited.set(curRow, curCol, 'X');
        return 1;
    }

    int trails = 0;
    // left
    trails += countTrails(grid, curRow, curCol-1, curNum, visited);
    
    // right
    trails += countTrails(grid, curRow, curCol+1, curNum, visited);

    // up
    trails += countTrails(grid, curRow-1, curCol, curNum, visited);

    // down
    trails += countTrails(grid, curRow+1, curCol, curNum, visited);

    return trails;
}


int countTrailsPart2(const StringGrid& grid, int curRow, int curCol, int prevNum) {

    int curNum = grid.read(curRow, curCol) - '0';
    
    // no trail
    if (prevNum + 1 != curNum) return 0;

    // found complete trail
    if (curNum == 9) return 1;

    int trails = 0;
    // left
    trails += countTrailsPart2(grid, curRow, curCol-1, curNum);
    
    // right
    trails += countTrailsPart2(grid, curRow, curCol+1, curNum);

    // up
    trails += countTrailsPart2(grid, curRow-1, curCol, curNum);

    // down
    trails += countTrailsPart2(grid, curRow+1, curCol, curNum);

    return trails;
}

int countCompleteTrails(const StringGrid& grid, bool part2=false) {
    int trails = 0;
    for (int r=0; r < grid.height; r++) {
        for (int c=0; c < grid.width; c++) {
            if (grid.read(r, c) - '0' != 0) continue;

            
            // prevNum -1 means we only continue if the next one is 0 (trail head)
            int res;
            if (!part2) {
                StringGrid visited = grid;
                res = countTrails(grid, r, c, -1, visited);
            }else {
                res = countTrailsPart2(grid, r, c, -1);
            }

            trails += res;
        }
    }
    return trails;
}

int main () {
    std::ifstream file("large.input");
    std::stringstream ss;
    ss << file.rdbuf();
    std::string input_str = ss.str();
    StringGrid grid = std::move(input_str);

    StringGrid visited = grid;

    std::cout << countCompleteTrails(grid) << std::endl;

    std::cout << countCompleteTrails(grid, true) << std::endl;

}