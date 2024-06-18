#pragma once
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <limits>

class Argument {
public:
    bool m_haveStore = false;
    std::pair<std::string, std::string> m_name;
    std::string m_help;
    bool m_isPositional = false;
    bool m_isDefault = false;
    bool m_isMulti = false;
    size_t m_multiSize = -1;
    std::string type;

    std::vector<std::string> m_stringValue;
    std::string m_stringDefaultVal;
    std::string* m_storeStringValuePtr = nullptr;
    std::vector<std::string>* m_storeStringValuesPtr = nullptr;

    std::vector<int> m_intValue;
    int m_intDefaultVal;
    int* m_storeIntValuePtr = nullptr;
    std::vector<int>* m_storeIntValuesPtr = nullptr;

    bool* m_storeBoolValuePtr = nullptr;
    std::vector<bool> m_boolValue;
    bool m_boolDefaultVal;

    Argument(const char& chArg, const std::string& stArg) {
        m_name.first = "-" + std::string(1, chArg) + "=";
        m_name.second = "--" + stArg + "=";
    };
    explicit Argument(const std::string& stArg) {
        m_name.second = "--" + stArg + "=";
    };
    explicit Argument(const char& chArg) {
        m_name.first = "-" + std::string(1, chArg) + "=";
    };
    Argument(const std::string& stArg, std::string help) {
        m_name.second = "--" + stArg + "=";
        m_help = std::move(help);
    };
    Argument(char chArg, const std::string& stArg, std::string  help) {
        m_help = std::move(help);
        m_name.first = "-" + std::string(1, chArg) + "=";
        m_name.second = "--" + stArg + "=";
    };
    virtual Argument& StoreValue(std::string& val) {};
    virtual Argument& StoreValues(std::vector<std::string>& val) {};
    virtual Argument& Default(const std::string& value){};
    virtual void AddValue(const std::string& val) {};
    virtual Argument& StoreValue(int& val) {};
    virtual Argument& StoreValues(std::vector<int>& val) {};
    virtual Argument& Default(const int& value) {};
    virtual void AddValue(const int& val) {};
    virtual Argument& StoreValue(bool& val) {};
    virtual Argument& StoreValues(std::vector<bool>& val) {};
    virtual Argument& Default(const bool& value) {};
    virtual void AddValue(const bool& val) {};
    Argument& Positional() {
        m_isPositional = true;
        return *this;
    };
    Argument& MultiValue() {
        m_isMulti = true;
        m_multiSize = -1;
        return *this;
    };
    virtual Argument& MultiValue(size_t value) {
        m_isMulti = true;
        m_multiSize = value;
        return *this;
    };
};

class StringArg : public Argument {
private:
public:
    std::string type;
    StringArg(const char& chArg, const std::string& stArg) : Argument(chArg, stArg) {};
    explicit StringArg(const std::string& stArg) : Argument(stArg) {};
    explicit StringArg(const char& chArg) : Argument(chArg) {};
    StringArg(const std::string& stArg, std::string help) : Argument(stArg, std::move(help)){};
    StringArg(char chArg, const std::string& stArg, std::string  help) : Argument(chArg, stArg, std::move(help)) {};

    Argument& StoreValue(std::string& val) override {
        m_haveStore = true;
        m_storeStringValuePtr = &val;
        return *this;
    };
    Argument& StoreValues(std::vector<std::string>& val) override {
        m_haveStore = true;
        m_storeStringValuesPtr = &val;
        return *this;
    };
    void AddValue(const std::string& val) override {
        if (!m_isMulti && !m_stringValue.empty()) {
            m_stringValue.clear();
        }
        if (m_haveStore) {
            if (m_isMulti) {
                m_storeStringValuesPtr->push_back(val);
            } else {
                *m_storeStringValuePtr = val;
            }
        }
        m_stringValue.push_back(val);
    };
    Argument& Default(const std::string& value) override {
        m_isDefault = true;
        std::cout << "!" << '\n';
        m_stringDefaultVal = value;
        m_stringValue.push_back(value);
        return *this;
    };
    std::vector<std::string> GetValue() const {
        return m_stringValue;
    };
};

class IntArg : public Argument {
public:

    IntArg(const char& chArg, const std::string& stArg) : Argument(chArg, stArg) {};
    explicit IntArg(const std::string& stArg) : Argument(stArg) {};
    explicit IntArg(const char& chArg) : Argument(chArg) {};
    IntArg(const std::string& stArg, std::string help) : Argument(stArg, std::move(help)){};
    IntArg(char chArg, const std::string& stArg, std::string  help) : Argument(chArg, stArg, std::move(help)) {};

    Argument& StoreValue(int& val) override {
        m_haveStore = true;
        m_storeIntValuePtr = &val;
        return *this;
    };
    Argument& StoreValues(std::vector<int>& val) override {
        m_haveStore = true;
        m_storeIntValuesPtr = &val;
        return *this;
    };
    void AddValue(const int& val) override {
        if (!m_isMulti && !m_intValue.empty()) {
            m_intValue.clear();
        }
        if (m_haveStore) {
            if (m_isMulti) {
                m_storeIntValuesPtr->push_back(val);
            } else {
                *m_storeIntValuePtr = val;
            }
        }
        m_intValue.push_back(val);
    };
    Argument& Default(const int& value) override {
        m_isDefault = true;
        m_intDefaultVal = value;
        m_intValue.push_back(value);
        return *this;
    };
    std::vector<int> GetValue() const {
        return m_intValue;
    };
};

class BoolArg : public Argument {
public:

    BoolArg(const char& chArg, const std::string& stArg) : Argument(chArg, stArg) {};
    explicit BoolArg(const std::string& stArg) : Argument(stArg) {};
    explicit BoolArg(const char& chArg) : Argument(chArg) {};
    BoolArg(const std::string& stArg, std::string help) : Argument(stArg, std::move(help)){};
    BoolArg(char chArg, const std::string& stArg, std::string  help) : Argument(chArg, stArg, std::move(help)) {};

    Argument& StoreValue(bool& val) override {
        m_haveStore = true;
        m_storeBoolValuePtr = &val;
        return *this;
    };
    void AddValue(const bool& val) override {

        if (!m_boolValue.empty()) {
            m_boolValue.clear();
        }
        if (m_haveStore) {
            *m_storeBoolValuePtr = val;
        }
        m_boolValue.push_back(val);
    };
    Argument& Default(const bool& value) override {
        m_isDefault = true;
        m_boolDefaultVal = value;
        m_boolValue.push_back(value);
        return *this;
    };
    std::vector<bool> GetValue() const {
        return m_boolValue;
    };
};

class HelpArg {
private:
    bool m_haveStore = false;
public:
    std::pair<std::string, std::string> m_name;
    std::vector<std::string> m_value;
    std::string m_help;

    HelpArg() = default;
    HelpArg(char chArg, std::string help);
    HelpArg(const std::string& stArg, std::string help);
    HelpArg(char chArg, const std::string& stArg, std::string help);
};

namespace ArgumentParser {

    class ArgParser {
    private:
        std::vector<Argument*> arguments;
        HelpArg HelpArgV;
    public:
        std::string m_name;
        explicit ArgParser(std::string name): m_name(std::move(name)){};

        Argument& AddStringArgument(const char& chArg);
        Argument& AddStringArgument(const std::string& stArg, const std::string& help = "");
        Argument& AddStringArgument(const char& chArg, const std::string& stArg, const std::string& help = "");
        std::string GetStringValue(const std::string& name, unsigned num = 0);

        Argument& AddIntArgument(const char& chArg);
        Argument& AddIntArgument(const std::string& stArg, const std::string& help = "");
        Argument& AddIntArgument(const char& chArg, const std::string& stArg, const std::string& help = "");
        int GetIntValue(const std::string& name, unsigned num = 0);

        Argument& AddFlag(const char& chArg);
        Argument& AddFlag(const std::string& stArg, const std::string& help = "");
        Argument& AddFlag(const char& chArg, const std::string& stArg, const std::string& help = "");
        bool GetFlag(const std::string& name, unsigned num = 0);

        HelpArg& AddHelp(const char& chArg, const std::string& help);
        HelpArg& AddHelp(const std::string& stArg, const std::string& help);
        HelpArg& AddHelp(char chArg, const std::string& stArg, const std::string& help);
        bool Help() const;
        std::string StringArgHelp();
        std::string IntArgHelp();
        std::string FlagArgHelp();
        std::string HelpArgHelp();
        void HelpDescription();

        bool Parse(const std::vector<std::string>& val);
    };

} // namespace ArgumentParser
