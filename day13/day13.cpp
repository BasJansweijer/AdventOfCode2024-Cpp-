#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>

struct ClawData {
    int64_t prizeX, prizeY;
    int64_t AdeltaX, AdeltaY;
    int64_t BdeltaX, BdeltaY;
};

int64_t minimumTokens(const ClawData& claw) {
    int64_t detDivisor = claw.AdeltaX * claw.BdeltaY - claw.BdeltaX * claw.AdeltaY;
    
    int64_t a = (claw.prizeX * claw.BdeltaY - claw.prizeY * claw.BdeltaX) / detDivisor;
    int64_t b = (claw.prizeX * -claw.AdeltaY + claw.prizeY * claw.AdeltaX) / detDivisor;
    if (a*claw.AdeltaX + b*claw.BdeltaX != claw.prizeX || a*claw.AdeltaY + b*claw.BdeltaY != claw.prizeY) {
        return 0;
    }
    return a*3 + b;
}


int main() {
    std::ifstream file("large.input");
    std::string line;

    std::regex buttonRegex("Button (\\w): X\\+(\\d+), Y\\+(\\d+)");
    std::regex prizeRegex("Prize: X=(\\d+), Y=(\\d+)");
    std::smatch regexMatch;
    ClawData inputClaw;

    int total = 0;
    int64_t totalPart2 = 0;


    while (getline(file, line))
    {
        if (std::regex_search(line, regexMatch, buttonRegex)) {
            char buttonName = regexMatch[1].str()[0];
            int deltaX = std::stoi(regexMatch[2].str());
            int deltaY = std::stoi(regexMatch[3].str());

            if (buttonName == 'A') {
                inputClaw.AdeltaX = deltaX;
                inputClaw.AdeltaY = deltaY;
            } else {
                inputClaw.BdeltaX = deltaX;
                inputClaw.BdeltaY = deltaY;
            }
        } else if (std::regex_search(line, regexMatch, prizeRegex)) {
            inputClaw.prizeX = std::stoi(regexMatch[1].str());
            inputClaw.prizeY = std::stoi(regexMatch[2].str());
            total += minimumTokens(inputClaw);

            inputClaw.prizeX += 10000000000000;
            inputClaw.prizeY += 10000000000000;
            totalPart2 += minimumTokens(inputClaw);
        }
    }

    std::cout << "part1: " << total << std::endl;
    std::cout << "part2: " << totalPart2 << std::endl;
}