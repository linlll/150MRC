#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdint.h>
#include <string.h>

#define POSSIBEL_AVERAGE_ROW_LENGTH 10

typedef struct FileStruct {
  std::ifstream *ifs;
  uint64_t begin;
  uint64_t end;
} FileStruct;

typedef std::unordered_map<char, std::pair<long long, long long>> result_t;

void *work(void *arg) {
  FileStruct *fs = (FileStruct*)arg;
  std::string line;
  fs->ifs->seekg(fs->begin, std::ios::beg);

  long long *ret = new long long[10];
  memset(ret, 0, sizeof(long long)*10);
  while (getline(*(fs->ifs), line)) {
    int i = 0, j;
    char c = line[0];
    switch (line[0]) {
    case 'z': i = 8; j = 0; break;
    case 's': i = 8; j = 1; break;
    case 'n': i = 7; j = 3; break;
    case 'b': i = 7; j = 4; break;
    default: break;
    }

    if (c == 's' && line[4] == 'g') {
      c++;
      j++;
    }

    int t = 0;
    int sign = 1;
    i++;
    if (line[i] == '-') {
      sign = -1;
      i++;
    }
    while (i < line.size()) {
      t = 10 * t + line[i] - '0';
      i++;
    }
    t *= sign;

    ret[2*j] += t;
    ret[2*j+1]++;

    uint64_t pos = fs->ifs->tellg();
    if (pos >= (uint64_t)(fs->end))
      break;
  }
  return ret;
}

int main(int argc, char const *argv[]) {
  long long N = 150000000;
  std::string filepath = "/data/data.txt";
  if (argc != 3) {
    fprintf(stderr, "usage: ./data_generator number filepath\n");
    exit(-1);
  } else {
    N = std::stoll(std::string(argv[1]));
    filepath = std::string(argv[2]);
  }

  const int thread_num = 18;

  std::ifstream ifs(filepath, std::ios::in);
  if (!ifs.is_open()) {
    fprintf(stderr, "file is not open\n");
    exit(-1);
  }

  FileStruct *fs = new FileStruct[thread_num];
  if (fs == nullptr) {
    fprintf(stderr, "malloc failed.\n");
    exit(-1);
  }
  uint64_t _split = 0;
  for (int i = 0; i < thread_num; i++) {
    uint64_t split = ((i+1)*N/thread_num)*POSSIBEL_AVERAGE_ROW_LENGTH;
    ifs.seekg(split, std::ios::beg);
    while (ifs.get() != '\n');
    split = ifs.tellg();

    fs[i].ifs = new std::ifstream(filepath, std::ios::in);
    if (!fs[i].ifs->is_open()) {
      fprintf(stderr, "file not open.\n");
      exit(-1);
    }
    fs[i].begin = _split;
    fs[i].end = split;
    _split = split;
  }
  fs[thread_num-1].end = UINT64_MAX;

  pthread_t *tid = new pthread_t[thread_num];
  if (tid == nullptr) {
    fprintf(stderr, "malloc failed.\n");
    exit(-1);
  }
  for (int i = 0; i < thread_num; i++) {
    pthread_create(&tid[i], NULL, work, &fs[i]);
  }

  long long **ret = new long long*[thread_num];
  for (int i = 0; i < thread_num; i++) {
    pthread_join(tid[i], (void**)(&ret[i]));
  }

  for (int i = thread_num-1; i > 0; i--) {
    for (int j = 0; j < 5; j++) {
      ret[i-1][2*j] += ret[i][2*j];
      ret[i-1][2*j+1] += ret[i][2*j+1];
    }
  }


  printf("%s: %.2f\n", "zhejiang", ((float)ret[0][0])/ret[0][1]);
  printf("%s: %.2f\n", "shandong", ((float)ret[0][2])/ret[0][3]);
  printf("%s: %.2f\n", "shanghai", ((float)ret[0][4])/ret[0][5]);
  printf("%s: %.2f\n", "nanjing", ((float)ret[0][6])/ret[0][7]);
  printf("%s: %.2f\n", "beijing", ((float)ret[0][8])/ret[0][9]);

  for (int i = 0; i < thread_num; i++) {
    fs[i].ifs->close();
    delete fs[i].ifs;
  }
  delete fs;
  for (int i = 0; i < thread_num; i++) {
    delete ret[i];
  }
  delete ret;
  return 0;
}
