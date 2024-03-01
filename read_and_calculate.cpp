#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

int main(int argc, char const *argv[]) {
  std::ifstream ifs("data.txt", std::ios::in);
  if (!ifs.is_open()) {
    fprintf(stderr, "file is not open\n");
    exit(-1);
  }
  std::unordered_map<std::string, std::pair<float, int>> map;
  std::string line;
  while (std::getline(ifs, line)) {
    int i = 0;
    while (line[i++] != ',');
    std::string c = line.substr(0, i-1);
    std::string v = line.substr(i, line.size());
    float t = std::stof(v);
    map[c].first += t;
    map[c].second++;
  }
  for (auto it = map.begin(); it != map.end(); it++) {
    printf("%s: %.2f\n", it->first.c_str(), it->second.first/it->second.second);
  }
  ifs.close();
  return 0;
}
