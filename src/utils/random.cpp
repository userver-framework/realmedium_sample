#include <random>

#include <iostream>
#include "random.hpp"

namespace real_medium::utils::random {
std::string GenerateSalt() {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(48,122);
  const int kSaltLength = 32;
  std::string salt;
  for (size_t i = 0; i < kSaltLength; ++i) {
    salt += static_cast<char>(dist(rng));
  }
  return salt;
}
}  // namespace real_medium::utils::random