#include <iostream>
#include <vector>
#include <string>
#include <fstream>

typedef std::pair<std::string, std::string> word_pair_t;

void extract_words(const std::string& str, std::vector<std::string>& target) {
    if (str.empty()) return;

    int start = 0, end = 0;

    while ((start = str.find_first_not_of(' ', end)) != std::string::npos) {
        end = str.find(' ', start);
        target.push_back(str.substr(start, end - start));
    }
}

void change_current_word(std::string& word, std::vector<word_pair_t>& reserve_dictionary) {
    if (word.empty()) return;

    auto need_string = [](const word_pair_t& list, const std::string& target) {
        return strcmp(list.first.c_str(), target.c_str()) == 0 ? list.second : target;
    };

    for (auto& element : reserve_dictionary)
        word = need_string(element, word);
}

std::string compile_string(std::vector<std::string>& list_of_words) {
    std::string resilt = "";

    for (auto& str : list_of_words) {
        resilt.append(str);
        if (strcmp(str.c_str(), list_of_words.at(list_of_words.size() - 1).c_str()) != 0)
            resilt.append(" ");
    }

    return resilt;
}

void change_all_words(std::string& str, std::vector<word_pair_t>& reserve_dictionary) {
    if (!str.empty()) {

        std::vector<std::string> words = { };
        extract_words(str, words);

        for (auto& word : words)
            change_current_word(word, reserve_dictionary);

        str = compile_string(words);
    }
}

void read_dictionary(const std::string& path, std::vector<word_pair_t>& target) {
    std::ifstream file(path);

    if (!file.is_open()) return;

    std::string cache_line;

    while (getline(file, cache_line)) {
        size_t pos = cache_line.find("\\t");
        word_pair_t pair = { cache_line.substr(0, pos), cache_line.substr(pos + 2) };
        target.push_back(pair);
    }

    file.close();
}

void print_dictionary(std::vector<word_pair_t>& reserve_dictionary) {
    std::cerr << "[ ~ ] Current dictionary:" << std::endl;

    for (auto& str : reserve_dictionary) {
        std::cerr << "[" << str.first << " | " << str.second << "]" << std::endl;
    }
}

int main() {
    std::vector<word_pair_t> word_pairs = { };
    read_dictionary("words.txt", word_pairs);

    std::cerr << "[ + ] Dictionary read sucessfully" << std::endl;

    print_dictionary(word_pairs);

    std::cerr << "[ ~ ] Input strings: " << std::endl;
  
    std::string cache_string = "";

    while (cache_string.empty()) 
        std::getline(std::cin, cache_string);

    change_all_words(cache_string, word_pairs);
    std::cout << cache_string << std::endl;
  
    std::cerr << "[ + ] Done" << std::endl;
    system("pause");

    return 0;
} 
