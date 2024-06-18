#include "Searcher.h"


bool isOperator(const std::string& c) {
    return c == "AND" || c == "OR";
}
int precedence(const std::string& c) {
    if (c == "AND") {
        return 2;
    } else if (c == "OR") {
        return 1;
    }
    return 0;
}

std::vector<std::pair<std::string, size_t>> concat(std::vector<std::pair<std::string, size_t>>& fst, std::vector<std::pair<std::string, size_t>>& scnd) {
    if (fst.size() < scnd.size()) {
        std::swap(fst, scnd);
    }
    for (auto& i: scnd) {
        fst.push_back(i);
    }
    return fst;
}

std::vector<std::pair<std::string, size_t>> intersect(std::vector<std::pair<std::string, size_t>>& fst, std::vector<std::pair<std::string, size_t>>& scnd) {
    std::vector<std::pair<std::string, size_t>> res;
    if (fst.size() < scnd.size()) {
        std::swap(fst, scnd);
    }
    for (auto& i: scnd) {
        if (std::find(fst.begin(), fst.end(),i) != fst.end()) {
            res.push_back(i);
        }
    }
    return res;
}
std::vector<std::string> Config::infixToRPN(const std::vector<std::string>& infix) {
    std::vector<std::string> rpn;
    std::stack<std::string> operators;

    for (const auto& c : infix) {
        if (c != "AND" && c != "OR" && c != "(" && c != ")") {
            rpn.push_back(c);
        } else if (isOperator(c)) {
            while (!operators.empty() && precedence(operators.top()) >= precedence(c)) {
                if (operators.top() != "(" && operators.top() != ")") {
                    rpn.push_back(operators.top());
                }
                operators.pop();
            }
            operators.push(c);
        } else if (c == "(") {
            operators.push(c);
        } else if (c == ")") {
            while (!operators.empty() && operators.top() != "(") {
                rpn.push_back(operators.top());
                operators.pop();
            }
            operators.pop();
        }
    }

    while (!operators.empty()) {
        if (operators.top() != "(" && operators.top() != ")") {
            rpn.push_back(operators.top());
        }
        operators.pop();
    }
    return rpn;
}

Config::Config(int argc, char **argv) {
    int bracesCount = 0;
    for (int i = 1; i < argc; ++i) {
        for (int j = 0; j < strlen(argv[i]); ++j) {
            if (argv[i][j] == '(') {
                ++bracesCount;
            } else if (argv[i][j] == ')'){
                --bracesCount;
            }
        }
    }
    if (bracesCount != 0) {
        std::cerr << "Incorrect input.";
        exit(EXIT_FAILURE);
    }

    std::vector<std::string> tmp;
    std::string tmpWord;
    for (int i = 0;;++i) {
        if (argv[1][i] == '\0') {
            tmp.push_back(tmpWord);
            break;
        } else if (argv[1][i] == ' ') {
            tmp.push_back(tmpWord);
            tmpWord = "";
        } else {
            tmpWord += argv[1][i];
        }

    }
    for (const auto & i : tmp) {
        if (!isOperator(i)) {
            keys.push_back(i);
        }
    }
    RPN = infixToRPN(tmp);
}

std::vector<std::string> Config::getRPN() {
    return RPN;
}

 std::string Config::checkKey(const std::string &key) {
    for (const auto& i: keys) {
        if (key.find(i) != std::string::npos) {
            return i;
        }
    }
    return "";
}

std::vector<std::pair<std::string, size_t>> Searcher::evalRPN(const std::vector<std::string>& tokens) {
    std::stack<std::vector<std::pair<std::string, size_t>>> stack;
    for (const auto& t : tokens) {
        if (!isOperator(t)) {
            stack.push(files[t]);
        } else {
            auto b = stack.top();
            stack.pop();
            auto a = stack.top();
            stack.pop();
            if (t == "OR") {
                stack.push(concat(a, b));
            } else if (t == "AND") {
                stack.push(intersect(a, b));
            }
        }
    }
    return stack.top();
}

std::string Searcher::getKey(const std::string& str) {
    std::string res;
    for (auto i: str) {
        if (i == ':') {
            return res;
        }
        res += i;
    }
    return "";
}

long long getInd(std::vector<std::pair<std::string, size_t>>& vec, const std::string& word) {
    for (long long i = 0; i < vec.size(); ++i) {
        if (vec[i].first == word) {
            return i;
        }
    }
    return -1;
}

std::string getNum(size_t i, const std::string& words) {
    std::string number;
    for (size_t j = i + 1; j < words.size(); ++j) {
        if (words[j] == '|' && !number.empty()) {
            return number;
        }
        number += words[j];
    }
    return "";
}

void Searcher::Parse(const std::string& key, const std::string& words) {
    std::string currFile;
    size_t startInd = -1;
    for (size_t i = 0; i < words.size(); ++i) {
        if (words[i] == ':' && words[i+1] == ' ') {
            startInd = i + 1;
        }
        if (words[i] == '|' && words[i+1] != '\0') {
            long long index = getInd(files[key], currFile);
            std::string num = getNum(i, words);
            if (index != -1) {
                files[key][index].second += std::stoll(num);
            } else {
                files[key].push_back({currFile, std::stoll(num)});
            }
            currFile = "";
            i += num.size() + 2;
        }
        if (i > startInd) {
            currFile += words[i];
        }
    }
}

double Searcher::countIDF() {
    size_t N = fileSize;
    double val = 0;
    for (const auto& i: files) {
        val += std::log((N-i.second.size()+0.5)/(i.second.size()+0.5));
    }
    return val;
}

size_t getWordsCount(const std::string& filename) {
    std::ifstream f(filename, std::ios::in);
    std::string s;
    size_t words = 0;
    while (true) {
        getline(f, s, ' ');
        if (!f.eof()) break;
        words++;
    }
    return words;
}

bool Compare(std::pair<std::string, double> &a,
             std::pair<std::string, double> &b) {
    return a.second > b.second;
}

void sortScore(std::map<std::string, double>& scores) {
    std::vector<std::pair<std::string, double>> sorted;
    for (const auto& i: scores) {
        sorted.push_back(i);
    }
    sort(sorted.begin(), sorted.end(), Compare);
}

void PrintStrokes(const std::string& word, const std::string& filename) {
    std::ifstream file(filename.substr(1, filename.size()-2));
    if (!file.is_open()) {
        return;
    }
    std::string curr;
    bool printed = false;
    size_t numCurr = 0;
    while (getline(file, curr)) {
        if (curr.find(word) != std::string::npos) {
            printed = true;
            std::cout << numCurr << ' ';
        }
        ++numCurr;
    }
    if (!printed) {
        std::cout << '-' << ' ';
    }

}

void Searcher::PrintResult() {
    countBM25();
    for (const auto& i: scores) {
        std::ifstream file(i.first.substr(1, i.first.size() - 2));
        if (!file.is_open()) {
            return;
        }
        for (const auto& j: files) {
            std::cout << j.first << ' ';
            PrintStrokes(j.first, i.first);
        }
        std::cout << i.first << '\n';
    }
}

void Searcher::countBM25() {
    std::unordered_map<std::string, size_t> fileSizes;
    double avg;
    double IDF = countIDF();
    for (const auto& i: RPN) {
        fileSizes[i.first] = getWordsCount(i.first);
        avg += fileSizes[i.first];
    }
    avg /= RPN.size();
    for (const auto& i: RPN) {
        scores[i.first] = 0;
        double currScore = 0;
        for (const auto& j: files) {
            size_t freq = 0;
            for (const auto& k: j.second) {
                if (i.first == k.first) {
                    freq += k.second;
                }
            }
            currScore += (freq * 3)/(freq + 2*(1-0.75+0.75*fileSizes[i.first]/avg));
        }
        scores[i.first] = currScore * IDF;
    }

    sortScore(scores);
}
