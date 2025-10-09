#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

bool match_pattern(const string &input_line, const string &pattern) {
  if (pattern.size() == 1) {
    return input_line.find(pattern) != string::npos;
  } else if (pattern == "\\d") {
    // match digits
    return any_of(input_line.begin(), input_line.end(),
                  [](unsigned char ch) { return isdigit(ch); });
  } else if (pattern == "\\w") {
    // match alphanumeric and underscore character
    return any_of(input_line.begin(), input_line.end(),
                  [](unsigned char ch) { return isalnum(ch) || ch == '_'; });
  } else if (pattern.starts_with("[^") && pattern.ends_with(']')) {
    // match negative character groups
    return input_line.find_first_not_of(
               pattern.substr(2, pattern.size() - 3)) != string::npos;
  } else if (pattern.front() == '[' && pattern.back() == ']') {
    // match positive character groups
    return input_line.find_first_of(pattern.substr(1, pattern.size() - 2)) !=
           string::npos;
  } else {
    throw runtime_error("Unhandled pattern " + pattern);
  }
}

int main(int argc, char *argv[]) {
  cout << unitbuf;
  cerr << unitbuf;

  cerr << "Logs from your program will appear here" << endl;

  if (argc != 3) {
    cerr << "Expected two arguments" << endl;
    return 1;
  }

  string flag = argv[1];
  string pattern = argv[2];

  if (flag != "-E") {
    cerr << "Expected first argument to be '-E'" << endl;
    return 1;
  }

  string input_line;
  getline(cin, input_line);

  try {
    if (match_pattern(input_line, pattern)) {
      return 0;
    } else {
      return 1;
    }
  } catch (const runtime_error &e) {
    cerr << e.what() << endl;
    return 1;
  }
}
