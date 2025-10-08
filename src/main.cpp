#include <cctype>
#include <cmath>
#include <iostream>
#include <string>
using namespace std;

bool check_pattern(const string &input_line, const string &pattern) {
  if (pattern == "\\d") {
    // match digits
    return input_line.find_first_of("0123456789") != string::npos;
  } else if (pattern == "\\w") {
    // match alphanumeric and underscore
    for (char ch : input_line) {
      if (isalnum(ch) || ch == '_') {
        return true;
      }
    }
    return false;
  } else {
    return input_line.find(pattern) != string::npos;
  }
}

bool match_pattern(const string &input_line, const string &pattern) {
  if (pattern.length() >= 1) {
    return check_pattern(input_line, pattern);
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
