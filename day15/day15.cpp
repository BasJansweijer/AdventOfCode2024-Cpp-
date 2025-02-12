#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

class StringGrid
{

    std::string content;
  public:
    int width, height;
  public:
    struct GridPos {
        int row;
        int col;

        GridPos(){}
        GridPos(int row, int col) : row(row), col(col) {}

        GridPos operator + (const GridPos& other) {
            return GridPos(row + other.row, col + other.col);
        }

        GridPos operator - (const GridPos& other) {
            return GridPos(row - other.row, col - other.col);
        }

        bool operator == (const GridPos& other) {
            return other.row == row && other.col == col;
        }

        bool operator != (const GridPos& other) {
            return !this->operator==(other);
        }

        void operator +=(const GridPos& other) {
            row += other.row;
            col += other.col;
        }
    };

    StringGrid(std::string&& values) 
       : width(values.find('\n')),
         height((values.size() + 1) / (width + 1)),
         content(values)
    {}

    StringGrid(int w, int h) 
       : width(w),
         height(h),
         content(w * h + (h-1), '\n')
    {
        clear(' ');
    }

    bool inBounds(const GridPos& p) const {
        return p.row < height && p.row >= 0 && p.col < width && p.col >= 0;
    }

    char read(const GridPos& p) const {
        if (!inBounds(p)) return '\0';
        
        return content[getIndex(p)];
    }

    void set(const GridPos& p, char c) {
        if (!inBounds(p)) return;
        
        content[getIndex(p)] = c;
    }

    void print() {
        std::cout << content << std::endl;
    }

    const std::string& str() const {
        return content;
    }

    void clear(char emptyChar=' ') {
        for (int r=0; r < height; r++)
        {
            for (int c=0; c < width; c++)
            {
                set({r, c}, emptyChar);
            }
        }
    }

    GridPos find(char c, const GridPos& startAfter={0,-1}) const {
        // We skip the provided start
        int startIdx = getIndex({startAfter.row, startAfter.col}) + 1;
        int found = content.find(c, startIdx);
        if (found == std::string::npos) {
            return {-1, -1};
        }

        return convertIndex(found);
    }

  private:
    int getIndex(const GridPos& p) const {
        return p.row * (width + 1) + p.col;
    }

    GridPos convertIndex(int i) const {
        return {i / (width + 1), i % (width + 1)};
    }
};

bool moveLegal(const StringGrid& field, StringGrid::GridPos robotPos, const StringGrid::GridPos& move) {
    StringGrid::GridPos curPos = robotPos + move;

    char curVal = field.read(curPos);

    while (curVal == 'O' || curVal == '[' || curVal == ']') {

        if (curVal == '[' && move.row != 0) 
        {
            StringGrid::GridPos otherBoxPos = curPos;
            otherBoxPos.col += 1;
            if (!moveLegal(field, otherBoxPos, move)) return false;
        }
        else if (curVal == ']' && move.row != 0)
        {
            StringGrid::GridPos otherBoxPos = curPos;
            otherBoxPos.col -= 1;
            if (!moveLegal(field, otherBoxPos, move)) return false;
        }
        
        curPos += move;
        curVal = field.read(curPos);
    }

    return curVal != '#';
}


void makeMove(StringGrid& field, StringGrid::GridPos& robotPos, const StringGrid::GridPos& move, StringGrid& pushed) {
    char moving_value = field.read(robotPos);
    field.set(robotPos, '.');
    robotPos += move;

    StringGrid::GridPos curPos = robotPos;
    char old_value;

    do {
        // update the current field
        old_value = field.read(curPos);
        field.set(curPos, moving_value);
        
        if (old_value == '[' && move.row != 0)
        {
            StringGrid::GridPos otherBoxPos = curPos;
            otherBoxPos.col += 1;
            if (pushed.read(otherBoxPos) != 'Y') {
                makeMove(field, otherBoxPos, move, pushed);
            }
        }
        else if (old_value == ']' && move.row != 0)
        {
            StringGrid::GridPos otherBoxPos = curPos;
            otherBoxPos.col -= 1;
            if (pushed.read(otherBoxPos) != 'Y') {
                makeMove(field, otherBoxPos, move, pushed);
            }
        }

        // move on and replace the next field
        pushed.set(curPos, 'Y');
        curPos += move;
        moving_value = old_value;
    }
    while ((moving_value = old_value) == 'O' || moving_value == '[' || moving_value == ']');
}

StringGrid::GridPos decodeMove(char move) {
    switch(move)
    {
        case '^': return {-1, 0};
        case '<': return {0, -1};
        case '>': return {0, 1};
        case 'v': return {1, 0};
        default:
            throw std::logic_error("invalid move");
    }

}

int countSubstringOccurrences(const std::string& str, const std::string& sub) {
    int count = 0;
    size_t pos = 0;

    // Loop to find all occurrences of the substring
    while ((pos = str.find(sub, pos)) != std::string::npos) {
        count++;
        pos += sub.length();  // Move past the last found substring
    }

    return count;
}


bool isInvalid(const StringGrid& field) {
    for (int c=0; c < field.width; c++) {
        if (field.read({0, c}) != '#') {
            return true;
        }
    }
    std::string s = field.str();
    int countWhole = countSubstringOccurrences(s, "[]");

    int countR = countSubstringOccurrences(s, "]");

    int countL = countSubstringOccurrences(s, "[");

    return countWhole != countL || countL != countR;

}

void performMovements(StringGrid& field, std::string_view instructions) {
    StringGrid::GridPos robotPos = field.find('@');

    int i = 0;

    for (char c : instructions) {
        StringGrid::GridPos move = decodeMove(c);
        if (moveLegal(field, robotPos, move)) {
            StringGrid pushed = field;
            makeMove(field, robotPos, move, pushed);
            if (isInvalid(field)) {
                field.print();
                std::cout << i << std::endl;
                break;
            }
            i++;
        }
    }
}

int64_t calcBoxScores(const StringGrid& grid, char boxChar) {
    int64_t totalScore = 0;
    StringGrid::GridPos lastFound = {0, -1};
    lastFound = grid.find(boxChar, lastFound);
    while (lastFound != StringGrid::GridPos(-1, -1)) 
    {
        totalScore += lastFound.row * 100 + lastFound.col;
        lastFound = grid.find(boxChar, lastFound);
    }

    return totalScore;
}

StringGrid makeWideBoard(const StringGrid& normalField) {
    StringGrid wideBoard(normalField.width * 2, normalField.height);
    for (int r=0; r < normalField.height; r++)
    {
        for (int c=0; c < normalField.width; c++)
        {
            char square = normalField.read({r, c});
            if (square == 'O') 
            {
                wideBoard.set({r, c*2}, '[');
                wideBoard.set({r, c*2 + 1}, ']');

            } 
            else if (square == '@')
            {
                wideBoard.set({r, c*2}, square);
                wideBoard.set({r, c*2 + 1}, '.');
            }
            else 
            {
                wideBoard.set({r, c*2}, square);
                wideBoard.set({r, c*2 + 1}, square);
            }

        }
    }

    return wideBoard;
}

int main() {
    std::ifstream file("large.input");
    std::string line;

    std::string field = "";

    std::string moveInstructions = "";

    bool loadingField = true;

    while (getline(file, line)) 
    {
    
        if (line.size() <= 1) 
        {
            field.pop_back();
            loadingField = false;
            continue;
        }
        
        if (loadingField) 
        {
            // String field class relies on the \n after each row.
            field += line + '\n';
        }else {
            moveInstructions += line;
        }
    }

    StringGrid grid(std::move(field));
    StringGrid gridPart2 = makeWideBoard(grid);

    // performMovements(grid, moveInstructions);

    std::cout << calcBoxScores(grid, 'O') << std::endl;

    performMovements(gridPart2, moveInstructions);

    gridPart2.print();

    std::cout << calcBoxScores(gridPart2, '[') << std::endl;

}