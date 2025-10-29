#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> tokenize_pattern(const string &pattern) {
  vector<string> tokens;

  size_t i = 0;

  // handle ^ at beginning
  if (!pattern.empty() && pattern[0] == '^') {
    tokens.push_back("^");
    i = 1;
  }

  for (; i < pattern.size();) {
    if (pattern[i] == '\\') {
      if (i + 1 < pattern.size()) {
        tokens.push_back(pattern.substr(i, 2));
        i += 2;
      } else {
        throw runtime_error("Invalid escape sequence in pattern");
      }
    } else if (pattern[i] == '[') {
      size_t end = pattern.find(']', i);
      if (end != string::npos) {
        tokens.push_back(pattern.substr(i, end - i + 1));
        i = end + 1;
      } else {
        throw runtime_error("Unclosed [ in pattern");
      }
    } else {
      size_t j = i;
      while (j < pattern.size() && pattern[j] != '\\' && pattern[j] != '[')
        j++;
      tokens.push_back(pattern.substr(i, j - i));
      i = j;
    }
  }
  return tokens;
}

bool match_token(const string &input, size_t pos, const string &token) {
  if (token == "\\d")
    return pos < input.size() && isdigit(input[pos]);
  else if (token == "\\w")
    return pos < input.size() && (isalnum(input[pos]) || input[pos] == '_');
  else if (token.starts_with("[^") && token.ends_with(']')) {
    string neg_chars = token.substr(2, token.size() - 3);
    return pos < input.size() && neg_chars.find(input[pos]) == string::npos;
  } else if (token.front() == '[' && token.back() == ']') {
    string chars = token.substr(1, token.size() - 2);
    return pos < input.size() && chars.find(input[pos]) != string::npos;
  } else {
    return input.compare(pos, token.size(), token) == 0;
  }
}

bool match_pattern(const string &input_line, const string &pattern) {
  auto tokens = tokenize_pattern(pattern);

  bool anchored_start = false;
  size_t token_start_index = 0;

  // chech if pattern begins with ^
  if(!tokens.empty() && tokens[0] == "^") {
    anchored_start = true;
    token_start_index = 1;
  }

  size_t start_limit = anchored_start ? 1 : input_line.size();

  for (size_t start = 0; start < start_limit; ++start) {
    size_t pos = start;
    bool matched = true;

    for (size_t ti = token_start_index; ti < tokens.size(); ++ti) {
      const auto &token = tokens[ti];

      if (token == "\\d" || token == "\\w" || token.front() == '[') {
        if (!match_token(input_line, pos, token)) {
          matched = false;
          break;
        }
        pos += 1;
      } else {
        if (!match_token(input_line, pos, token)) {
          matched = false;
          break;
        }
        pos += token.size();
      }
    }

    if (matched && pos <= input_line.size()) {
      return true;
    }
  }
  return false;
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
