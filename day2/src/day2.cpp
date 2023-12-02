#include<algorithm>
#include <numeric>
#include <sstream>
#include <stdio.h>
#include<string>
#include<iostream>
#include<cstdio>
#include<vector>
#include<sstream>
#include<cassert>

#include <FileTokenizer.h>


static void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

static void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

std::vector<std::string> str_split(const std::string& str, char delim)
{
    std::vector<std::string> ret;
    std::string segment;
    std::stringstream stream(str);
    while(std::getline(stream, segment, delim))
    {
        trim(segment);
        ret.emplace_back(std::move(segment));
        segment.clear();
    }

    return ret;
}

struct GameSet {
    int blue;
    int red;
    int green;

    GameSet (int b, int r, int g) : blue(b), red(r), green(g)
    {}

    GameSet(const std::string& in_str) : blue(0), red(0), green(0)
    {
        auto comma_split = str_split(in_str, ',');
        for(const auto& num_colour : comma_split)
        {
            auto space_split = str_split(num_colour, ' ');
            assert(space_split.size() == 2);
            if(space_split[1] == "blue") {
                blue = strtol(space_split[0].c_str(), NULL, 10);
            }
            else if(space_split[1] == "red") {
                red = strtol(space_split[0].c_str(), NULL, 10);
            }
            else if(space_split[1] == "green") {
                green = strtol(space_split[0].c_str(), NULL, 10);
            }
        }
    }

    bool is_possible(const GameSet& bag) const
    {
        return bag.blue >= blue &&
              bag.red >= red &&
              bag.green >= green;
    }
};

struct Game {
    int id;
    std::vector<GameSet> sets;

    Game(const std::string& in_str)
    {
        std::vector<std::string> id_set_split = str_split(in_str, ':');
        assert(id_set_split.size() == 2);
        sscanf(id_set_split[0].c_str(),"Game %d", &id);
        
        std::vector<std::string> game_sets = str_split(id_set_split[1], ';');
        for(const auto& set : game_sets)
        {
            sets.emplace_back(set);
        }
    }

    bool is_possible(const GameSet& bag) const
    {
        return std::accumulate(sets.cbegin(), sets.cend(), true,
             [&bag](bool valid, const GameSet& set){ return valid && set.is_possible(bag);});
    }
};

const GameSet bag = GameSet(14,12,13);

int main()
{
    auto tokenizer = FileTokenizer("../input/day2.txt", '\n');
    std::vector<Game> games;
    std::vector<Game> valid_games;
    for(std::string game_str = tokenizer.next().value_or("");
         !game_str.empty();
         game_str = tokenizer.next().value_or(""))
    {
        games.emplace_back(game_str);
    }
    std::copy_if (games.begin(), games.end(), std::back_inserter(valid_games),
         [](const Game& game ){return  game.is_possible(bag);} );
    
    uint64_t part1 = std::accumulate(valid_games.cbegin(), valid_games.cend(), 0,
             [](uint64_t acc, const Game& game){ return acc + game.id;});
    std::cout << "part1: " << part1  << std::endl;
    return 1;
}