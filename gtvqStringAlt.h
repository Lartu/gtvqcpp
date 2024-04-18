#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
typedef string gtvqString;

// Converts string to uppercase
void gtvqString_toUpper(gtvqString &out) {
  std::transform(out.begin(), out.end(), out.begin(), ::toupper);
}

// Converts string to lowercase
void gtvqString_toLower(gtvqString &out) {
  std::transform(out.begin(), out.end(), out.begin(), ::tolower);
}

// Removes all trailing and ending whitespace from a string
void gtvqString_trim(gtvqString &line) {
  // If the string is empty
  if (line.size() == 0) return;

  // If the string has only one character
  if (line.size() == 1 && !isspace(line[0])) return;

  // Left trim
  int first = 0;
  for (unsigned int i = 0; i < line.size(); ++i) {
    if (!isspace(line[i])) {
      first = i;
      break;
    }
  }

  // Right trim
  int last = 0;
  for (unsigned int i = line.size() - 1; i >= 0; --i) {
    if (!isspace(line[i])) {
      last = i + 1;
      break;
    }
    //--i will break with unsigned int when reaching 0, so we check
    // if i == 0 and, if it is and it's not a space, we break
    if (i == 0) {
      last = 0;
      break;
    }
  }

  // Trim the string
  line = line.substr(first, last - first);
}