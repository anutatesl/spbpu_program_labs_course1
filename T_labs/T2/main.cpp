#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "DataStruct.hpp"

int main()
{
  std::vector< teslenko::DataStruct > dataVect;
  using istreamIter = std::istream_iterator< teslenko::DataStruct >;
  using ostreamIter = std::ostream_iterator< teslenko::DataStruct >;
  while (!std::cin.eof())
  {
    std::cin.clear();
    std::copy(istreamIter(std::cin), istreamIter(), std::back_inserter(dataVect));
  }
  std::sort(dataVect.begin(), dataVect.end());
  std::copy(dataVect.begin(), dataVect.end(), ostreamIter(std::cout, "\n"));
  return 0;
}
