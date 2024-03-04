# 大文本读取

## 梦之开始

https://zhuanlan.zhihu.com/p/683955185

可先查看该链接，是一个十亿行文本的挑战赛，文件大小为13GB，冠军仅用了1.5秒，比赛中的文本读取是在**内存**当中进行的，不需要考虑磁盘读取到内存的时间，所以文本读取的上限很高。

我们可以随机生成一些数据，弄成一个较小的文本，然后供我们读取，文件存放在**磁盘**，即需要考虑磁盘到内存的IO时间。

文本格式：city,temperature，我们要计算出所有城市的平均温度，然后打印到终端即可。浮点类型的输出保留2位小数即可（1.00也行）。

他们叫1BRC，我们就叫150MRC（The 150 Million Row Challenge）

## 数据生成

数据生成程序，运行一次即可，所有的值都是随机值，1.5亿行数据，可产生一个2.4GB的文本文件。

```cpp
#include <iostream>
#include <fstream>
#include <time.h>
#include <random>
#include <vector>

int main(int argc, char const *argv[]) {
  const std::vector<std::string> city = {"zhejiang", "nanjing", "shanghai", "beijing", "shandong"};
  std::mt19937 gen;
  std::vector<std::uniform_real_distribution<>> temp_dis = {
    std::uniform_real_distribution<>(0, 2.468),
    std::uniform_real_distribution<>(-10.2, 24.4997),
    std::uniform_real_distribution<>(-30, 9.76),
    std::uniform_real_distribution<>(-14.06, -10),
    std::uniform_real_distribution<>(25, 35.9)
  };
  std::uniform_int_distribution<> city_dis(0,4); // city
  std::ofstream ofs("data.txt", std::ios::out);

  clock_t start = clock();
  long long N = 150000000;
  while (N--) {
    int i = city_dis(gen);
    ofs << city[i] << "," << temp_dis[i](gen) << "\n";
  }

  ofs.close();
  return 0;
}
```

## 示例代码

```cpp
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

int main(int argc, char const *argv[]) {
  std::ifstream ifs("data.txt", std::ios::in);
  std::unordered_map<std::string, std::pair<float, int>> map;
  std::string line;
  while (std::getline(ifs, line)) {
    int i = 0;
    while (line[i++] != ',');
    std::string c = line.substr(0, i-1);
    std::string v = line.substr(i, line.size());
    float t = std::stof(v);
    if (map.find(c) == map.end()) {
      map[c].first = t;
      map[c].second = 1;
    } else {
      map[c].first += t;
      map[c].second++;
    }
  }
  for (auto it = map.begin(); it != map.end(); it++) {
    printf("%s: %.2f\n", it->first.c_str(), it->second.first/it->second.second);
  }
  ifs.close();
  return 0;
}
```

## 运行

1.5亿行数据，2.4GB大小的文本文件，下面是一次的运行时间

```bash
time ./read_and_calculate

shanghai: -10.46
nanjing: 7.05
zhejiang: 1.17
beijing: -8.95
shandong: 27.79

real	0m49.556s
user	0m8.088s
sys	0m36.745s
```
共用时49秒左右。

## 目标

尝试优化它。