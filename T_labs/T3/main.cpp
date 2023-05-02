#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <iostream>
#include <fstream>
#include <IofmtGuard.hpp>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Invalid number of arguments\n";
    return 1;
  }
  //switch (std::atoi(argv[1]))
  std::string filename = argv[1];
  std::ifstream in(filename);
  if (!in.is_open())
  {
    std::cerr << "Can't open file\n";
    return 1;
  }
  std::vector< teslenko::Polygon > polygons;
  using istreamIter = std::istream_iterator< teslenko::Polygon >;
  using ostreamIter = std::ostream_iterator< teslenko::Polygon >;
  while (!in.eof())
  {
    in.clear();
    std::copy(istreamIter(in), istreamIter(), std::back_inserter(polygons));
  }
  in.close();

  using namespace std::placeholders;
  teslenko::Commands commands(polygons, std::cin, std::cout);
  std::map< std::string, std::function< void(void) > > command(
    {
      {"AREA", std::bind(&teslenko::Commands::getArea, commands)},
      {"MAX", std::bind(&teslenko::Commands::getMax, commands)},
      {"MIN", std::bind(&teslenko::Commands::getMin, commands)},
      {"COUNT", std::bind(&teslenko::Commands::getCount, commands)},
      {"RECTS", std::bind(&teslenko::Commands::getMaxSeq, commands)},
      {"SAME", std::bind(&teslenko::Commands::getSame, commands)}
    });

  while (!std::cin.eof())
  {
    std::cin.clear();
    std::string commandName = "";
    std::cin >> commandName;
    if (!commandName.empty())
    {
      auto iter = command.find(commandName);
      if (iter != command.end())
      {
        try
        {
          iter->second();
          std::cout << '\n';
        }
        catch (const std::logic_error& e)
        {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
          std::cout << e.what() << '\n';
        }
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
        if ((std::cin.fail() && !std::cin.eof()) || iter == command.end())
        {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
      }
    }
  }
  return 0;
}
