#ifndef TEXT_ANALYZER_H
#define TEXT_ANALYZER_H

#include <ios>
#include <string>
#include "RedBlackTree.h"
#include "list.h"

class TextAnalyzer
{
public:
    TextAnalyzer();
    TextAnalyzer(const TextAnalyzer& other) = delete;
    TextAnalyzer(TextAnalyzer&& other) noexcept;
    ~TextAnalyzer() = default;
    TextAnalyzer& operator=(const TextAnalyzer& other) = delete;
    TextAnalyzer& operator=(TextAnalyzer&& other) noexcept;

    const RedBlackTree<std::string, List<int>>& getDictionary() const;
    void analyze(const std::string& filename);
    void analyze(std::istream& is);
    static void enumerateLines(const std::string& inFilename, const std::string& outFileName);
    static void enumerateLines(std::istream& is, std::ostream& os);
    void printAnalysis(const std::string& filename);
    void printAnalysis(std::ostream& os);
    bool deleteWord(const std::string& word);

private:
    RedBlackTree<std::string, List<int>> dictionary_;
};

#endif