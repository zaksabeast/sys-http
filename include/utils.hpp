#pragma once

#include <iomanip>
#include <string>
#include <switch.h>

std::string convertByteArrayToHex(u8 *bytes, size_t size) {
  std::stringstream stream;

  stream << std::setfill('0') << std::hex;

  for (u64 i = 0; i < size; i++) {
    stream << std::setw(2) << static_cast<int>(bytes[i]);
  }

  return stream.str();
}

std::string convertNumToHexString(u64 num) {
  std::stringstream hex;
  hex << std::hex << num;

  return hex.str();
}