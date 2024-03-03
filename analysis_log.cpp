#include <fstream>
#include <string>
#include <iostream>

int main(int argc, char const *argv[])
{
  std::string filepath = "run.log";
  if (argc == 2) {
    filepath = std::string(argv[1]);
  }

  std::ifstream ifs(filepath, std::ios::in);
  if (!ifs.is_open()) {
    fprintf(stderr, "file is not open\n");
    exit(-1);
  }
  std::string line;
  float time = 0;
  int total = 0;
  while(getline(ifs, line)) {
    if (line[0] == '0' && line[1] == 'i')
      continue;
    if (line.empty())
      continue;
    if (line == "\n")
      continue;
    if (!isdigit(line[0]))
      continue;
    int i = line.find("system")+7;
    int j = line.find(':', i);
    int k = line.find("elapsed", j);
    std::string s1 = line.substr(i,j-i);
    std::string s2 = line.substr(j+1, k-j-1);
    time += std::stof(s1)*60+std::stof(s2);
    total++;
  }
  printf("%f\n", time/total);
  return 0;
}
