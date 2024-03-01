#include <fstream>
#include <string>
#include <iostream>

int main(int argc, char const *argv[])
{
  std::ifstream ifs("run.log", std::ios::in);
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
    if (!isdigit(line[0]))
      continue;
    int i = 0;
    while (line[i++] != 'u');
    std::string s = line.substr(0, i-1);
    time += std::stof(s);
    total++;
  }
  printf("%f\n", time/total);
  return 0;
}
