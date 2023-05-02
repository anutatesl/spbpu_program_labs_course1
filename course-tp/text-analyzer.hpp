#ifndef TEXT_ANALYZER_H
#define TEXT_ANALYZER_H

#include <ios>
#include <string>
#include <map>
#include <vector>

namespace teslenko
{
  using map = std::map< std::string, std::vector< int > >;
  class TextAnalyzer
  {
  public:
    TextAnalyzer();
    TextAnalyzer(const TextAnalyzer& other) = delete;
    TextAnalyzer(TextAnalyzer&& other) noexcept;
    ~TextAnalyzer() = default;
    TextAnalyzer& operator=(const TextAnalyzer& other) = delete;
    TextAnalyzer& operator=(TextAnalyzer&& other) noexcept;

    const map& getDictionary() const;
    void printDictionary(map& dictionary_, std::ostream& os);

    map analyze(const std::string& filename);
    void printAnalysis(std::ostream& os, const std::string& filename);
    void uniteDictionaries(std::vector< map >& arrayDict, std::ostream& os);
    void subtractionDictionaries(std::vector< map >& arrayDict, std::ostream& os);
    void intersectDictionaries(std::vector< map >& arrayDict, std::ostream& os);
    bool searchWord(map& dictionary_, const std::string& word);

  private:
    std::map< std::string, std::vector< int > > dictionary_;
  };
}

#endif
