#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

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

    void clear(char emptyChar) {
        for (int r=0; r < height; r++) {
            for (int c=0; c < width; c++) {
                set(r, c, emptyChar);
            }
        }
    }

  private:
    int getIndex(int row, int col) const {
        return row * (width + 1) + col;
    }
};


const char PART_OF_REGION = '*';
static StringGrid *regionTrackerRef = nullptr;

void expand_region(StringGrid& field, char regionType, int row, int col, int& area, int& perimeter) {
    if (regionType != field.read(row, col)) {
        perimeter += 1;
        return;
    }

    if (regionTrackerRef->read(row, col) == PART_OF_REGION) return;

    area += 1;
    regionTrackerRef->set(row, col, PART_OF_REGION);
    field.set(row, col, PART_OF_REGION);

    expand_region(field, regionType, row - 1, col, area, perimeter);
    expand_region(field, regionType, row + 1, col, area, perimeter);
    expand_region(field, regionType, row, col - 1, area, perimeter);
    expand_region(field, regionType, row, col + 1, area, perimeter);
}

int countRegionSides(const StringGrid& region) {
    int sides = 0;

    // horizontal scan south edges
    for (int r = 0; r < region.height; r++) {
        bool previouslyOnEdge = false;
        for (int c = 0; c < region.width; c ++) {
            bool onEdge = region.read(r, c) == PART_OF_REGION
                       && region.read(r+1, c) != PART_OF_REGION;
            if (onEdge && !previouslyOnEdge) {
                sides ++;
            }
            previouslyOnEdge = onEdge;
        }
    }

    // horizontal scan north edges
    for (int r = 0; r < region.height; r++) {
        bool previouslyOnEdge = false;
        for (int c = 0; c < region.width; c ++) {
            bool onEdge = region.read(r, c) == PART_OF_REGION
                       && region.read(r-1, c) != PART_OF_REGION;
            if (onEdge && !previouslyOnEdge) {
                sides ++;
            }
            previouslyOnEdge = onEdge;
        }
    }

    // verical scan west facing
    for (int c = 0; c < region.width; c++) {
        bool previouslyOnEdge = false;
        for (int r = 0; r < region.height; r ++) {
            bool onEdge = region.read(r, c) == PART_OF_REGION
                       && region.read(r, c+1) != PART_OF_REGION;
            if (onEdge && !previouslyOnEdge) {
                sides ++;
            }
            previouslyOnEdge = onEdge;
        }
    }

    for (int c = 0; c < region.width; c++) {
        bool previouslyOnEdge = false;
        for (int r = 0; r < region.height; r ++) {
            bool onEdge = region.read(r, c) == PART_OF_REGION
                       && region.read(r, c-1) != PART_OF_REGION;
            if (onEdge && !previouslyOnEdge) {
                sides ++;
            }
            previouslyOnEdge = onEdge;
        }
    }

    return sides;
}

int main() {
    std::ifstream file("large.input");
    std::stringstream ss;
    ss << file.rdbuf();
    StringGrid field = ss.str();

    StringGrid regionTracker = field;
    regionTracker.clear('\0');
    regionTrackerRef = &regionTracker;

    int totalCost = 0;
    int totalCostPart2 = 0;

    for (int row=0; row < field.height; row++) {
        for (int col=0; col < field.width; col++) {
            regionTracker.clear('\0');
            char regionType = field.read(row, col);
            if (field.read(row, col) == PART_OF_REGION) {
                continue;
            }


            int regionArea = 0;
            int regionPerimeter = 0;
            expand_region(field, regionType, row, col, regionArea, regionPerimeter);
            int regionCost = regionArea * regionPerimeter;
            totalCost += regionCost;
            int sides = countRegionSides(regionTracker);
            totalCostPart2 += regionArea * sides;
            // std::cout << regionType << " sides:" << sides << " area:" << regionArea << std::endl;

        }
    }

    std::cout << "Total cost: " << totalCost << std::endl;
    std::cout << "Total cost (part2): " << totalCostPart2 << std::endl;
}

