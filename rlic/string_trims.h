#ifndef STRING_TRIMS_H
#define STRING_TRIMS_H

#include <algorithm>
#include <cctype>

// trim from start
static inline std::string &ltrim(std::string &s) {
  s.erase(s.begin(),
          std::find_if(s.begin(),
                       s.end(),
                       std::not1(std::ptr_fun<int, int>(std::isspace))
                      )
         );
  // for (auto &i = s.begin(); i != s.end(); ++i) {
  //   if (!std::isspace(*i)) {
  //     s.erase(s.begin(), i);
  //     break;
  //   }
  // }
  return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(),
                       s.rend(),
                       std::not1(std::ptr_fun<int, int>(std::isspace))
                      ).base(),
          s.end()
         );
  // for (auto &i = s.rbegin(); i != s.rend(); ++i) {
  //   if (!std::isspace(*i)) {
  //     s.erase(s.rbegin(), i);
  //     break;
  //   }
  // }
  return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
  return ltrim(rtrim(s));
}

#endif // STRING_TRIMS_H
