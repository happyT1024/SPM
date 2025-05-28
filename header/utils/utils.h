#pragma once

#include <boost/algorithm/string/replace.hpp>
#include <iostream>

void printSipMessage(const std::string& message) {
  std::string cleaned = message;
  // Заменяем \r\n на \n для корректного отображения
  boost::replace_all(cleaned, "\r\n", "\n");
  std::cout << cleaned;
}