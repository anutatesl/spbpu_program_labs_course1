#include "text-analyzer.hpp"
#include <regex>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <vector>

using map = std::map< std::string, std::vector< int > >;

teslenko::TextAnalyzer::TextAnalyzer():
  dictionary_{ }
{ }

teslenko::TextAnalyzer::TextAnalyzer(TextAnalyzer&& other) noexcept:
  dictionary_{ std::move(other.dictionary_) }
{ }

teslenko::TextAnalyzer& teslenko::TextAnalyzer::operator=(TextAnalyzer&& other) noexcept
{
  dictionary_ = std::move(other.dictionary_);
  return *this;
}

const map& teslenko::TextAnalyzer::getDictionary() const
{
  return dictionary_;
}

map teslenko::TextAnalyzer::analyze(const std::string& filename)
{
  auto is = std::ifstream{ filename };
  if (!is)
  {
    throw std::logic_error("Can't open file");
  }
  dictionary_ = std::map< std::string, std::vector< int > >{ };
  std::vector< int > vector;
  auto word_regex = std::regex{ "[a-zA-Z0-9]+" };
  auto line = std::string{ };
  for (int i = 1; is; ++i)
  {
    std::getline(is, line, '\n');
    auto words_begin = std::sregex_iterator{ line.begin(), line.end(), word_regex };
    auto words_end = std::sregex_iterator{ };
    for (auto itr = words_begin; itr != words_end; ++itr)
    {
      auto word = itr->str();
      std::transform(word.begin(), word.end(), word.begin(), [](char c) { return std::tolower(c); });
      std::map< std::string, std::vector< int > >::const_iterator it = dictionary_.find(word);
      if (it != dictionary_.end())
      {
        dictionary_[word].push_back(i);
      }
      else
      {
        vector.clear();
        vector.push_back(i);
        dictionary_.insert(std::make_pair(word, vector));
      }
    }
  }
  is.close();
  return dictionary_;
}

namespace teslenko
{
  std::string generateSpaces(size_t length);
}

void teslenko::TextAnalyzer::printAnalysis(std::ostream& os, const std::string& filename)
{
  std::map< std::string, std::vector< int > > dictionary_ = analyze(filename);
  printDictionary(dictionary_, os);
}

void teslenko::TextAnalyzer::printDictionary(map& dictionary_, std::ostream& os)
{
  auto word = std::string{ "Word" };
  auto colwidth = word.length();
  for (auto itr = dictionary_.begin(); itr != dictionary_.end(); ++itr)
  {
    colwidth = std::max(itr->first.length(), colwidth);
  }
  const auto margin = size_t{ 2 };
  os << "Word" << teslenko::generateSpaces(colwidth - word.length() + margin) << "Lines\n";
  for (auto itr = dictionary_.begin(); itr != dictionary_.end(); ++itr)
  {
    os << itr->first << teslenko::generateSpaces(colwidth - itr->first.length() + margin);
    std::for_each(itr->second.begin(), itr->second.end(), [&os](int e) { os << e << ' '; });
    os << '\n';
  }
}

namespace teslenko
{
  std::string generateSpaces(size_t length)
  {
    auto spaces = std::string{ };
    for (size_t i = 0u; i < length; ++i)
    {
      spaces += " ";
    }
    return spaces;
  }
}

namespace tesl = teslenko;

void tesl::TextAnalyzer::uniteDictionaries(std::vector< map >& arrayDict, std::ostream& os)
{
  for (size_t i = 0; i < arrayDict.size() - 1; i++)
  {
    auto ot = arrayDict[0].begin();
    if (arrayDict[0].size() == 0 && arrayDict[i + 1].size() != 0)
    {
      for (auto it = arrayDict[i + 1].begin(); it != arrayDict[i + 1].end(); ++it)
      {
        arrayDict[0].insert(std::make_pair((*it).first, (*it).second));
      }
    }
    else
    {
      for (auto it = arrayDict[i + 1].begin(); it != arrayDict[i + 1].end(); ++it)
      {
        while (ot != arrayDict[0].end() && (*ot).first < (*it).first)
        {
          ot++;
        }
        if (ot == arrayDict[0].end())
        {
          while (it != arrayDict[i + 1].end())
          {
            arrayDict[0].insert(std::make_pair((*it).first, (*it).second));
            ++it;
          }
          break;
        }
        if ((*ot).first == (*it).first)
        {
          if ((*ot).second != (*it).second)
          {
            size_t i = 0;
            size_t j = 0;
            while (i < (*ot).second.size() && j < (*it).second.size())
            {
              if ((*ot).second[i] < (*it).second[j])
              {
                if ((i + 1) == (*ot).second.size() && j != (*it).second.size())
                {
                  while (j != (*it).second.size())
                  {
                    (*ot).second.push_back((*it).second[j]);
                    j++;
                  }
                }
                i++;
              }
              else if ((*ot).second[i] > (*it).second[j])
              {
                (*ot).second.push_back((*it).second[j]);
                j++;
              }
              else
              {
                i++;
                j++;
              }
            }
            std::sort((*ot).second.begin(), (*ot).second.end());
          }
        }
        else
        {
          arrayDict[0].insert(std::make_pair((*it).first, (*it).second));
        }
      }
    }
  }
  printDictionary(arrayDict[0], os);
}

void tesl::TextAnalyzer::subtractionDictionaries(std::vector< map >& arrayDict, std::ostream& os)
{
  std::vector< std::string > tempKeys;
  for (size_t i = 0; i < arrayDict.size() - 1; i++)
  {
    auto ot = arrayDict[0].begin();
    if (arrayDict[0].size() == 0 && arrayDict[i + 1].size() != 0)
    {
      printDictionary(arrayDict[0], os);
      return;
    }
    for (auto it = arrayDict[i + 1].begin(); it != arrayDict[i + 1].end(); ++it)
    {
      while (ot != arrayDict[0].end() && (*ot).first < (*it).first)
      {
        ot++;
      }
      if (ot == arrayDict[0].end())
      {
        break;
      }
      if ((*ot).first == (*it).first && (*ot).second == (*it).second)
      {
        tempKeys.push_back((*ot).first);
      }
    }
  }
  for (size_t i = 0; i < tempKeys.size(); i++ )
  {
    std::string key = tempKeys[i];
    arrayDict[0].erase(key);
  }
  printDictionary(arrayDict[0], os);
}

void tesl::TextAnalyzer::intersectDictionaries(std::vector< map >& arrayDict, std::ostream& os)
{
  std::vector< std::string > tempKeys;
  for (size_t i = 0; i < arrayDict.size() - 1; i++)
  {
    auto ot = arrayDict[0].begin();
    for (auto it = arrayDict[i + 1].begin(); it != arrayDict[i + 1].end(); ++it)
    {
      while (ot != arrayDict[0].end() && (*ot).first < (*it).first)
      {
        tempKeys.push_back((*ot).first);
        ot++;
      }
      if (ot == arrayDict[0].end())
      {
        break;
      }
      if ((*ot).first == (*it).first)
      {
        if ((*ot).second != (*it).second)
        {
          tempKeys.push_back((*ot).first);
        }
        ot++;
      }
    }
  }
  for (size_t i = 0; i < tempKeys.size(); i++)
  {
    std::string key = tempKeys[i];
    arrayDict[0].erase(key);
  }
  printDictionary(arrayDict[0], os);
}

bool tesl::TextAnalyzer::searchWord(map& dictionary_, const std::string& word)
{
  std::map< std::string, std::vector< int > >::const_iterator it = dictionary_.find(word);
  if (it == dictionary_.end())
  {
    return false;
  }
  else
  {
    return true;
  }
}
