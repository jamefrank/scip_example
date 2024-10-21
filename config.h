#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "magic_enum.hpp"
#include "refl.hpp"
#include <yaml-cpp/yaml.h>

enum class Algorithm { greedy, hungarian };
enum class Criterion {
  dist,
  cos_dist,
  size_dist,
  bb3d_dist,
  iou_img,
  iou_bev,
  iou_3d,
  giou
};

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
std::enable_if_t<!std::is_enum<T>::value, void>
parse_property(YAML::Node &yaml, const std::string &name, T &value) {
  value = yaml[name].as<T>(); // TODO magic_enum
}

template <typename EnumType>
std::enable_if_t<std::is_enum<EnumType>::value, void>
parse_property(YAML::Node &yaml, const std::string &name, EnumType &value) {
  std::string str = yaml[name].as<std::string>();
  auto optional_value = magic_enum::enum_cast<EnumType>(str);
  if (optional_value)
    value = optional_value.value();
  else {
    std::cout << "Invalid value for " << name << std::endl;
    exit(-1);
  }
}

// // 特化解析函数，针对不同类型的属性进行不同的处理
// template <>
// void parse_property<Algorithm>(YAML::Node &yaml, const std::string &name,
//                                Algorithm &value) {
//   std::cout << "Handling int type for " << name << std::endl;
//   std::string str = yaml[name].as<std::string>();
//   auto optional_value = magic_enum::enum_cast<Algorithm>(str);
//   if (optional_value)
//     value = optional_value.value();
//   else {
//     std::cout << "Invalid value for " << name << std::endl;
//     exit(-1);
//   }
// }

// template <>
// void parse_property<Criterion>(YAML::Node &yaml, const std::string &name,
//                                Criterion &value) {
//   std::cout << "Handling int type for " << name << std::endl;
//   std::string str = yaml[name].as<std::string>();
//   auto optional_value = magic_enum::enum_cast<Criterion>(str);
//   if (optional_value)
//     value = optional_value.value();
//   else {
//     std::cout << "Invalid value for " << name << std::endl;
//     exit(-1);
//   }
// }

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
  }                                                                            \
  void load(YAML::Node &yaml){PARSE_ALL(__VA_ARGS__)};

struct Config {
public:
  std::string host;
  int port;
  Algorithm algorithm;

  ADD_PROPERTIES(host, port, algorithm)
};

REFL_AUTO(type(Config), field(host), field(port), field(algorithm))

// partial update

/**
 * Used with trait::map_t to provide storage type for the member.
 */
template <typename Member> struct make_optional_storage {
  using underlying_type = decltype(
      Member{}(std::declval<const typename Member::declaring_type &>()));
  using type = std::optional<refl::trait::remove_qualifiers_t<underlying_type>>;
};

/**
 * A proxy which stores properties of the target type as std::optionals.
 */
template <typename T>
class partial : public refl::runtime::proxy<partial<T>, T> {
public:
  // Fields and property getters.
  static constexpr auto members =
      filter(refl::member_list<T>{}, [](auto member) {
        return is_readable(member) && has_writer(member);
      });

  using member_list = std::remove_cv_t<decltype(members)>;

  // Trap getter calls.
  template <typename Member, typename Self, typename... Args>
  static decltype(auto) invoke_impl(Self &&self) {
    static_assert(is_readable(Member{}));
    return self.template get<Member>();
  }

  // Trap setter calls.
  template <typename Member, typename Self, typename Value>
  static void invoke_impl(Self &&self, Value &&value) {
    static_assert(is_writable(Member{}));
    using getter_t = decltype(get_reader(Member{}));
    self.template get<getter_t>() = std::forward<Value>(value);
  }

  template <typename Member> auto &get() {
    constexpr size_t idx = refl::trait::index_of_v<Member, member_list>;
    static_assert(idx != -1);
    return refl::util::get<idx>(data);
  }

  template <typename Member> const auto &get() const {
    constexpr size_t idx = refl::trait::index_of_v<Member, member_list>;
    static_assert(idx != -1);
    return refl::util::get<idx>(data);
  }

private:
  using member_storage_list =
      refl::trait::map_t<make_optional_storage, member_list>;

  refl::trait::as_tuple_t<member_storage_list> data;
};

template <typename T> void update(T &target, const partial<T> &source) {
  for_each(source.members, [&](auto member) {
    using member_type = decltype(member);
    constexpr auto writer = get_writer(member);
    auto opt_value = source.template get<member_type>();
    if (opt_value) {
      writer(target, *opt_value);
    }
  });
}
