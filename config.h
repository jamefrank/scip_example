#include <iostream>
#include <string>
#include <vector>

#include "magic_enum.hpp"
#include <yaml-cpp/yaml.h>

enum class Algorithm { greedy, hungarian };

// #define PROPERTY(TYPE, NAME)                                                   \
//   void parse_##NAME##_property(YAML::Node &yaml) {                             \
//     std::cout << "parsing " #NAME << std::endl;                                \
//     NAME = yaml[#NAME].as<TYPE>();                                             \
//   }

#define PROPERTY(TYPE, NAME)                                                   \
  void parse_##NAME##_property(YAML::Node &yaml) {                             \
    std::cout << "parsing " #NAME << std::endl;                                \
    parse_property<TYPE>(yaml, #NAME, NAME);                                   \
  }

// 定义一个通用的解析函数
template <typename T>
void parse_property(YAML::Node &yaml, const std::string &name, T &value) {
  value = yaml[name].as<T>(); // TODO magic_enum
}

// 特化解析函数，针对不同类型的属性进行不同的处理
template <>
void parse_property<Algorithm>(YAML::Node &yaml, const std::string &name,
                               Algorithm &value) {
  std::cout << "Handling int type for " << name << std::endl;
  std::string str = yaml[name].as<std::string>();
  auto optional_value = magic_enum::enum_cast<Algorithm>(str);
  if (optional_value)
    value = optional_value.value();
  else {
    std::cout << "Invalid value for " << name << std::endl;
    exit(-1);
  }
}

#define APPLY0(t, dummy)
#define APPLY1(t, a) t(a)
#define APPLY2(t, a, b) t(a) t(b)
#define APPLY3(t, a, b, c) t(a) t(b) t(c)
#define APPLY4(t, a, b, c, d) t(a) t(b) t(c) t(d)
#define APPLY5(t, a, b, c, d, e) t(a) t(b) t(c) t(d) t(e)
#define APPLY6(t, a, b, c, d, e, f) t(a) t(b) t(c) t(d) t(e) t(f)

#define NUM_ARGS_H1(dummy, x6, x5, x4, x3, x2, x1, x0, ...) x0
#define NUM_ARGS(...) NUM_ARGS_H1(dummy, ##__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define APPLY_ALL_H3(t, n, ...) APPLY##n(t, __VA_ARGS__)
#define APPLY_ALL_H2(t, n, ...) APPLY_ALL_H3(t, n, __VA_ARGS__)
#define APPLY_ALL(t, ...) APPLY_ALL_H2(t, NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

#define PARSE_ALL(...) APPLY_ALL(PARSE, __VA_ARGS__)
#define PARSE(n) parse_##n##_property(yaml);

#define DEFINE_ALL(...) APPLY_ALL(DEFINE, __VA_ARGS__)
#define DEFINE(n) PROPERTY(decltype(n), n)

#define ADD_PROPERTIES(...)                                                    \
  DEFINE_ALL(__VA_ARGS__)                                                      \
  void load(const std::string &path) {                                         \
    auto yaml = YAML::LoadFile(path);                                          \
    PARSE_ALL(__VA_ARGS__)                                                     \
  };

struct Config {
  std::string host;
  int port;
  Algorithm algorithm;

  ADD_PROPERTIES(host, port, algorithm)
  // ADD_PROPERTIES(host, port)
};