#include<algorithm>
#include<string>
#include<iostream>
#include<cstdio>
#include<vector>
#include<map>

#include <FileTokenizer.h>

const std::map<std::string, std::string> name_to_num =
{
    {"one",   "1"},
    {"two",   "2"},
    {"three", "3"},
    {"four",  "4"},
    {"five",  "5"},
    {"six",   "6"},
    {"seven", "7"},
    {"eight", "8"},
    {"nine",  "9"},
};



void convert_name_to_num_first_last(std::string& str)
{
    std::vector<std::pair<size_t, std::string>> first_occurances;
    std::vector<std::pair<size_t, std::string>> last_occurances;
    for(const auto& name_num : name_to_num)
    {
        std::size_t first_pos = str.find(name_num.first);
        std::size_t last_pos = str.rfind(name_num.first);
        if (first_pos != std::string::npos)
        {
            first_occurances.emplace_back(first_pos, name_num.second);
        }
        if (last_pos != std::string::npos)
        {
            last_occurances.emplace_back(last_pos, name_num.second);
        }
    }
    std::sort(first_occurances.begin(), first_occurances.end(),
        [](std::pair<size_t, std::string> a, std::pair<size_t, std::string> b) { return a.first < b.first;});
    std::sort(last_occurances.begin(), last_occurances.end(), 
        [](std::pair<size_t, std::string> a, std::pair<size_t, std::string> b) { return a.first > b.first;});

    if(first_occurances.size() > 0 )
    {
        str.replace(first_occurances[0].first, first_occurances[0].second.length(), first_occurances[0].second);
    }
    if(last_occurances.size() > 0 )
    {
        str.replace(last_occurances[0].first, last_occurances[0].second.length(), last_occurances[0].second);
    }

}

uint32_t calc_first_last(const std::string& str)
{
    char two_digit[3] = {'\0'};
    two_digit[0] = *std::find_if(str.cbegin(), str.cend(), [](char c){ return std::isdigit(c);});
    two_digit[1] = *std::find_if(str.rbegin(), str.rend(), [](char c){ return std::isdigit(c);});
    
    return std::strtoul(two_digit, NULL, 10);
}

int main()
{
    auto tokenizer = FileTokenizer("../input/day1.txt", '\n');
    std::vector<std::string> cal_values;
    uint64_t cur_cal = 0;
    uint64_t part_2_cur_cal = 0;
    for(std::string cal_str = tokenizer.next().value_or("");
         !cal_str.empty();
         cal_str = tokenizer.next().value_or(""))
    {
        cal_values.emplace_back(cal_str);
        cur_cal += calc_first_last(cal_str);
    }
    std::cout << "part1: " << cur_cal << std::endl;

    for(auto& cal_value : cal_values)
    {
        convert_name_to_num_first_last(cal_value);
        part_2_cur_cal += calc_first_last(cal_value);
    }
    std::cout << "part2: " << part_2_cur_cal << std::endl;
    return 1;
}