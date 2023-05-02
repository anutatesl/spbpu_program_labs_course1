#include <string>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <functional>
#include <map>
#include <vector>
#include "text-analyzer.hpp"

namespace teslenko
{
  std::string getTheLineElem(std::string& line)
  {
    size_t nextSpace = line.find_first_of(' ', 0);
    std::string elem = line.substr(0, nextSpace);
    nextSpace = (nextSpace == line.npos) ? nextSpace : nextSpace + 1;
    line.erase(0, nextSpace);
    return elem;
  }

  void analyzeText(std::string& line, std::ostream& out)
  {
    std::string fileDictianoryForAnalysis = getTheLineElem(line);
    std::string way = getTheLineElem(line);
    TextAnalyzer textAnalyzer{ };
    if (way == "console")
    {
      textAnalyzer.printAnalysis(std::cout, fileDictianoryForAnalysis);
    }
    else if (way == "file")
    {
      textAnalyzer.printAnalysis(out, fileDictianoryForAnalysis);
    }
  }

  void search(std::string& line, std::ostream& out)
  {
    std::string fileDictianory = teslenko::getTheLineElem(line);
    TextAnalyzer textAnalyzer{ };
    std::string word = teslenko::getTheLineElem(line);
    map dictionary_ = { };
    dictionary_ = textAnalyzer.analyze(fileDictianory);
    if (textAnalyzer.searchWord(dictionary_, word))
    {
      out << "Key-value pair present in dictionary\n";
    }
    else
    {
      out << "Key-value pair not present in dictionary\n";
    }
  }

  std::vector< map > getDictArray(std::string& line)
  {
    teslenko::TextAnalyzer textAnalyzer{ };
    std::vector< map > arrayDictionaries;
    std::string countFilesString = teslenko::getTheLineElem(line);
    size_t countFiles = std::stoi(countFilesString);
    if (!countFiles || (countFiles < 2))
    {
      throw std::logic_error("Not a number\n");
    }
    for (size_t i = 0; i < countFiles; ++i)
    {
      std::string fileWithDictionary = teslenko::getTheLineElem(line);
      map dictionary_ = { };
      dictionary_ = textAnalyzer.analyze(fileWithDictionary);
      arrayDictionaries.push_back(dictionary_);
    }
    return arrayDictionaries;
  }

  void unite(std::string& line, std::ostream& out)
  {
    std::vector< map > arrayDictionaries = getDictArray(line);
    teslenko::TextAnalyzer textAnalyzer{ };
    textAnalyzer.uniteDictionaries(arrayDictionaries, out);
  }

  void subtraction(std::string& line, std::ostream& out)
  {
    std::vector< map > arrayDictionaries = getDictArray(line);
    teslenko::TextAnalyzer textAnalyzer{ };
    textAnalyzer.subtractionDictionaries(arrayDictionaries, out);
  }

  void intersect(std::string& line, std::ostream& out)
  {
    std::vector< map > arrayDictionaries = getDictArray(line);
    teslenko::TextAnalyzer textAnalyzer{ };
    textAnalyzer.intersectDictionaries(arrayDictionaries, out);
  }
}

int main()
{
  std::ifstream in;
  std::string inFileName = "";
  std::ofstream out;
  std::string outFileName = "";
  std::cout << "Enter the name of the source data file including extension (e.g. input.txt): ";
  std::cin >> inFileName;
  try
  {
    in.open(inFileName);
    if (!in)
    {
      throw std::logic_error("Error, file not open\n");
    }
    std::cout << "Enter the file name where to output the result: ";
    std::cin >> outFileName;
    out.open(outFileName);
    if (!out)
    {
      throw std::logic_error("Error, file not open\n");
    }
  }
  catch (const std::exception& ex)
  {
    std::cerr << ex.what() << '\n';
    return 1;
  }
  using namespace std::placeholders;
  std::map< std::string, std::function< void(std::string& line) > > commands({
    {"ANALYZE", std::bind(teslenko::analyzeText, _1, std::ref(out))},
    {"UNITE", std::bind(teslenko::unite, _1, std::ref(out))},
    {"SUBTRACT", std::bind(teslenko::subtraction, _1, std::ref(out))},
    {"INTERSECT", std::bind(teslenko::intersect, _1, std::ref(out))},
    {"SEARCH", std::bind(teslenko::search, _1, std::ref(out))},
  });
  std::string line = "";
  while (!in.eof())
  {
    try
    {
      std::getline(in, line);
      std::string command = teslenko::getTheLineElem(line);
      if (!command.empty())
      {
        auto commandIter = commands.find(command);
        if (commandIter == commands.end())
        {
          throw std::logic_error("No command\n");
        }
        commandIter->second(line);
        out << "\n";
      }
    }
    catch (const std::exception& ex)
    {
      std::cerr << ex.what() << '\n';
      return 1;
    }
  }
  in.close();
  out.close();
  in.open(outFileName);
  std::ifstream f;
  std::string expectedResFile = "";
  std::cout << "Enter the name of the expected result file including extension (e.g. input.txt): ";
  std::cin >> expectedResFile;
  try
  {
    f.open(expectedResFile);
    if (!f)
    {
      throw std::logic_error("Error, file not open");
    }
    std::string line1 = "";
    std::string line2 = "";
    while (!in.eof() || !f.eof())
    {
      std::getline(in, line1);
      std::getline(f, line2);
      if (line1 != line2)
      {
        throw std::logic_error("ERROR_PROGRAM_NOT_CORRECTLY\n");
      }
    }
    if ((in.eof() && !f.eof()) || (!in.eof() && f.eof()))
    {
      throw std::logic_error("ERROR_PROGRAM_NOT_CORRECTLY\n");
    }
    in.close();
    f.close();
  }
  catch (const std::exception& ex)
  {
    std::cerr << ex.what() << '\n';
    return 1;
  }
  return 0;
}
