#include "config.h"
#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
  Config cfg;
  std::string yaml_path = "/home/frank/data/github/scip_example/config.yml";
  YAML::Node pqr = YAML::LoadFile(yaml_path);
  cfg.load(pqr);

  std::cout << "host:" << cfg.host << std::endl;
  std::cout << "port:" << cfg.port << std::endl;
  std::cout << "algorithm:" << magic_enum::enum_name(cfg.algorithm)
            << std::endl;

  partial<Config> partial_config;
  partial_config.algorithm(Algorithm::hungarian);

  update(cfg, partial_config);
  std::cout << "algorithm updated:" << magic_enum::enum_name(cfg.algorithm)
            << std::endl;

  return 0;
}