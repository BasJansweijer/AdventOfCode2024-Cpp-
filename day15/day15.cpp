#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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
        std::cout << startIdx << ',' << found << std::endl;
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

    while (field.read(curPos) == 'O') {
        curPos += move;
    }

    return field.read(curPos) != '#';
}

void makeMove(StringGrid& field, StringGrid::GridPos& robotPos, const StringGrid::GridPos& move) {
    field.set(robotPos, '.');
    robotPos += move;

    StringGrid::GridPos curPos = robotPos;
    char moving_value = '@';
    char old_value;

    do {
        // update the current field
        old_value = field.read(curPos);
        field.set(curPos, moving_value);

        // move on and replace the next field
        curPos += move;
        moving_value = old_value;
    }
    while ((moving_value = old_value) == 'O');
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

void performMovements(StringGrid& field, std::string_view instructions) {
    StringGrid::GridPos robotPos = field.find('@');

    for (char c : instructions) {
        StringGrid::GridPos move = decodeMove(c);
        if (moveLegal(field, robotPos, move)) {
            makeMove(field, robotPos, move);
        }
    }
}

int calcBoxScores(const StringGrid& grid) {
    int totalScore = 0;
    StringGrid::GridPos lastFound = {0, -1};
    lastFound = grid.find('O', lastFound);
    std::cout << lastFound.row << ',' << lastFound.col << std::endl;
    while (lastFound != StringGrid::GridPos(-1, -1)) 
    {
        totalScore += lastFound.row * 100 + lastFound.col;
        lastFound = grid.find('O', lastFound);
    }

    return totalScore;
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
    performMovements(grid, moveInstructions);

    grid.print();

    std::cout << calcBoxScores(grid) << std::endl;

}