#include <iostream>
#include <vector>
#include <string>
#include <fstream>

typedef std::pair<std::string, std::string> WordPair;

class WordsChanger {
public:
    WordsChanger(std::vector<std::string>& StringsVector, std::vector<WordPair>& Dict) : StringsToStransform(StringsVector), ReserveDictionary(Dict) { };

    void Change() {
        if (StringsToStransform.empty() || ReserveDictionary.empty()) return;

        for (auto& String : StringsToStransform) {
            ChangeAllWords(String);
        }
    }

    void PrintStrings() {
        std::cout << "[ ~ ] Current strings state:" << std::endl;

        for (auto& String : StringsToStransform) {
            std::cout << String << std::endl;
        }
    }

    void PrintDictionary() {
        std::cout << "[ ~ ] Current dictionary:" << std::endl;

        for (auto& String : ReserveDictionary) {
            std::cout << "[" << String.first << " | " << String.second << "]" << std::endl;
        }
    }
private:
    void ExtractWords(const std::string& String, std::vector<std::string>& Target) {
        if (String.empty()) return;

        //Можно было через stringstream т.к. он принимает строки разделенные пробелом
        //но решил ограничиться библиотеками и сделать используя только string

        int Start = 0, End = 0;
       
        while ((Start = String.find_first_not_of(' ', End)) != std::string::npos) {
            End = String.find(' ', Start);
            Target.push_back(String.substr(Start, End - Start));
        }
    }

    void ChangeCurrentWord(std::string& Word) { 
        if (Word.empty()) return;

        auto NeededString = [](const std::pair<std::string, std::string>& List, const std::string& Target) {
            return strcmp(List.first.c_str(), Target.c_str()) == 0 ? List.second : Target;
        };

        for (auto& Element : ReserveDictionary)
            Word = NeededString(Element, Word);
    }

    std::string CompileString(std::vector<std::string>& ListOfWords) {
        std::string Result = "";
       
        for (auto& String : ListOfWords) {
            Result.append(String);
            if (strcmp(String.c_str(), ListOfWords.at(ListOfWords.size() - 1).c_str()) != 0)
                Result.append(" ");
        }

        return Result;
    }

    std::string& ChangeAllWords(std::string& String) {
        if (!String.empty()) {
          
            std::vector<std::string> Words = { };
            ExtractWords(String, Words);
         
            for (auto& Word : Words) 
                ChangeCurrentWord(Word);

            String = CompileString(Words);
        }
    }

private:
    std::vector<std::string> StringsToStransform = { };
    std::vector<WordPair> ReserveDictionary = { };
};

class Reader {
public:
    void ReadDictionary(const std::string& Path, std::vector<WordPair>& Target) {
        std::ifstream File(Path);

        if (!File.is_open()) return;
        
        std::string CacheLine;

        while (getline(File, CacheLine)) {
            size_t Position = CacheLine.find("\\t");
            WordPair Pair = { CacheLine.substr(0, Position), CacheLine.substr(Position + 2) };
            Target.push_back(Pair);
        }

        File.close();
    }

    void ReadStrings(const std::string& End, std::vector<std::string>& StringsToStransform) {
        std::string CacheString = "";

        while (std::getline(std::cin, CacheString)) {
            if (strcmp(CacheString.c_str(), End.c_str()) != 0) {
                if (!CacheString.empty()) {
                    StringsToStransform.push_back(CacheString);
                }
            }
            else
                break;  
        }
    }
};

int main() {
    Reader ReaderObject;

    std::vector<WordPair> WordPairs = { };
    ReaderObject.ReadDictionary("words.txt", WordPairs);

    std::cout << "[ + ] Dictionary read sucessfully" << std::endl;
    std::cout << "[ ~ ] Input strings: " << std::endl;

    std::vector<std::string> StringsToStransform = { };
    ReaderObject.ReadStrings("\\n", StringsToStransform);

    WordsChanger ChangerObject = WordsChanger(StringsToStransform, WordPairs);
    ChangerObject.PrintStrings();

    std::cout << "[ ~ ] Changing..." << std::endl;

    ChangerObject.Change();
    ChangerObject.PrintDictionary();
    ChangerObject.PrintStrings();

    std::cout << "[ + ] Done" << std::endl;
    system("pause");

    return 0;
}