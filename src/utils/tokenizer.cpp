#pragma once

#include <string>
#include <vector>

class Tokenizer {
   public:
    static std::vector<std::string> getTokens(const std::string s, const std::string delim = " ") {
        std::vector<std::string> res;
        std::string token = "";
        for (int i = 0; i < s.size(); i++) {
            bool flag = true;
            for (int j = 0; j < delim.size(); j++) {
                if (s[i + j] != delim[j]) flag = false;
            }
            if (flag) {
                if (token.size() > 0) {
                    res.push_back(token);
                    token = "";
                    i += delim.size() - 1;
                }
            } else {
                token += s[i];
            }
        }
        res.push_back(token);
        return res;
    }
};