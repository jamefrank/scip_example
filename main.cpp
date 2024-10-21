#include "config.h"
#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
  Config cfg;
  cfg.load("/home/frank/data/github/scip_example/config.yml");

  std::cout << "host:" << cfg.host << std::endl;
  std::cout << "port:" << cfg.port << std::endl;
  std::cout << "algorithm:" << magic_enum::enum_name(cfg.algorithm)
            << std::endl;

  return 0;
}