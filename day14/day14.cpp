#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <chrono>
#include <thread>

class StringGrid
{
    std::string content;
  public:
    int width, height;
  public:
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
                set(r, c, emptyChar);
            }
        }
    }

  private:
    int getIndex(int row, int col) const {
        return row * (width + 1) + col;
    }
};

static int s_width = 101;
static int s_height = 103;

enum Quadrant
{
    TL, TR, BL, BR, None
};


class Robot
{
    int vx, vy;
  public:
    int px, py;

    Robot(int px, int py, int vx, int vy)
        :px(px), py(py), vx(vx), vy(vy)
    { }

    void move() 
    {
        px = ((px + vx) % s_width + s_width) % s_width;
        py = ((py + vy) % s_height + s_height) % s_height;
    }

    Quadrant getQuadrant() 
    {
        if (px == s_width/2 || py == s_height/2) 
        {
            return Quadrant::None;
        }

        bool top = py < (s_height / 2);
        bool left = px < (s_width / 2);
        int key = top + left*2;
        switch (key) 
        {
            case 0: return Quadrant::BR;
            case 1: return Quadrant::TR;
            case 2: return Quadrant::BL;
            case 3: return Quadrant::TL;
            default: throw std::logic_error("Error in getQuadrant");
        }
    }
};

void updateStringGrid(const std::vector<Robot>& robots, StringGrid& grid) 
{
    grid.clear('.');
    for (auto r : robots) 
    {
        grid.set(r.py, r.px, 'x');
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

bool couldBeTree(const StringGrid& grid) 
{
    int touchingCount = countSubstringOccurrences(grid.str(), "xx");
    return touchingCount > 75;
}

int64_t calcSafetyScore(const std::vector<Robot>& robots)
{
    int64_t BRcount = 0;
    int64_t TRcount = 0;
    int64_t BLcount = 0;
    int64_t TLcount = 0;

    for (auto r : robots)
    {
        switch (r.getQuadrant())
        {
            case Quadrant::BL:
                BLcount++;
                break;
            case Quadrant::BR:
                BRcount++;
                break;
            case Quadrant::TR:
                TRcount++;
                break;
            case Quadrant::TL:
                TLcount++;
                break;
            default:
                break;
        }
    }

    return BRcount * TRcount * BLcount * TLcount;
}

int main()
{
    std::ifstream file("large.input");
    std::string line;

    std::regex inputLineReg("p=(-?\\d+),(-?\\d+) v=(-?\\d+),(-?\\d+)");
    std::smatch match;

    std::vector<Robot> robots;

    while (getline(file, line)) 
    {
        if(std::regex_match(line, match, inputLineReg)) 
        {
            int px = std::stoi(match[1]);
            int py = std::stoi(match[2]);

            int vx = std::stoi(match[3]);
            int vy = std::stoi(match[4]);

            robots.emplace_back(px, py, vx, vy);
        }
    }

    StringGrid g(s_width, s_height);

    for (int s = 0; s < 100; s++) 
    {
        for (Robot& r : robots) 
        {
            r.move();
        }
        
    }

    std::cout << "Safety score: " << calcSafetyScore(robots) << std::endl;

    for (int s = 101; s < 1000000; s++) 
    {
        for (Robot& r : robots) 
        {
            r.move();
        }

        updateStringGrid(robots, g);
        if (couldBeTree(g)) {
            g.print();
            std::cout << s << " seconds" << std::endl;
            std::cin.get();
        }
    }



}