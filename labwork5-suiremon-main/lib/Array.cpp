#include <iostream>
#include <utility>
#include "ArgParser.h"

Argument& ArgumentParser::ArgParser::AddStringArgument(const char& chArg) {
    arguments.emplace_back(new StringArg(chArg));
    arguments[arguments.size() - 1]->type = "string";
    return *arguments[arguments.size() - 1];
}

Argument& ArgumentParser::ArgParser::AddStringArgument(const std::string& stArg, const std::string& help) {
    arguments.emplace_back(new StringArg(stArg, help));
    arguments[arguments.size() - 1]->type = "string";
    return *arguments[arguments.size() - 1];
}

Argument&
ArgumentParser::ArgParser::AddStringArgument(const char& chArg, const std::string& stArg, const std::string& help) {
    arguments.emplace_back(new StringArg(chArg, stArg, help));
    arguments[arguments.size() - 1]->type = "string";
    return *arguments[arguments.size() - 1];
}

std::string ArgumentParser::ArgParser::GetStringValue(const std::string& name, unsigned int num) {
    for (auto& Arg: arguments) {
        if ((!Arg->m_name.first.empty() && Arg->m_name.first.substr(1, Arg->m_name.first.length() - 3) == name) ||
            Arg->m_name.second.substr(2, Arg->m_name.second.length() - 3) == name) {
            return Arg->m_stringValue[num];
        }
    }
    return "";
}


Argument& ArgumentParser::ArgParser::AddIntArgument(const char& chArg) {
    arguments.emplace_back(new IntArg(chArg));
    arguments[arguments.size() - 1]->type = "int";
    return *arguments[arguments.size() - 1];
}

Argument& ArgumentParser::ArgParser::AddIntArgument(const std::string& stArg, const std::string& help) {
    arguments.emplace_back(new IntArg(stArg, help));
    arguments[arguments.size() - 1]->type = "int";
    return *arguments[arguments.size() - 1];
}

Argument&
ArgumentParser::ArgParser::AddIntArgument(const char& chArg, const std::string& stArg, const std::string& help) {
    arguments.emplace_back(new IntArg(chArg, stArg, help));
    arguments[arguments.size() - 1]->type = "int";
    return *arguments[arguments.size() - 1];
}

int ArgumentParser::ArgParser::GetIntValue(const std::string& name, unsigned int num) {
    for (auto& Arg: arguments) {
        if ((!Arg->m_name.first.empty() && Arg->m_name.first.substr(1, Arg->m_name.first.length() - 3) == name) ||
            Arg->m_name.second.substr(2, Arg->m_name.second.length() - 3) == name) {
            return Arg->m_intValue[num];
        }
    }
    return -1;
}


Argument& ArgumentParser::ArgParser::AddFlag(const char& chArg) {
    arguments.emplace_back(new BoolArg(chArg));
    arguments[arguments.size() - 1]->type = "bool";
    return *arguments[arguments.size() - 1];
}

Argument& ArgumentParser::ArgParser::AddFlag(const std::string& stArg, const std::string& help) {
    arguments.emplace_back(new BoolArg(stArg, help));
    arguments[arguments.size() - 1]->type = "bool";
    return *arguments[arguments.size() - 1];
}

Argument& ArgumentParser::ArgParser::AddFlag(const char& chArg, const std::string& stArg, const std::string& help) {
    arguments.emplace_back(new BoolArg(chArg, stArg, help));
    arguments[arguments.size() - 1]->type = "bool";
    return *arguments[arguments.size() - 1];
}

bool ArgumentParser::ArgParser::GetFlag(const std::string& name, unsigned int num) {
    for (auto& Arg: arguments) {
        if ((!Arg->m_name.first.empty() && Arg->m_name.first.substr(1, Arg->m_name.first.length() - 3) == name) ||
            Arg->m_name.second.substr(2, Arg->m_name.second.length() - 3) == name) {
            return (Arg->m_boolValue[num]);
        }
    }
    return -1;
}

bool isNumeric(std::string const& str) {
    auto it = str.begin();
    while (it != str.end() && std::isdigit(*it)) {
        it++;
    }
    return !str.empty() && it == str.end();
}

void addIn(std::string const& str, Argument*& arg) {
    for (int i = 0; i < str.length(); ++i) {
        if (arg->m_name.first[1] == str[i]) {
            arg->AddValue(true);
        };
    }
}

bool ArgumentParser::ArgParser::Parse(const std::vector<std::string>& val) {
    if (val[0] != "app") {
        return false;
    }
    if (val.size() > 1) {
        if (val[1] == HelpArgV.m_name.first || val[1] == HelpArgV.m_name.second) {
            return true;
        }
    }
    bool add = false;
    for (const auto& value: val) {
        if (value != "app") {
            for (auto& Arg: arguments) {
                if (Arg->type == "bool") {
                    if (!Arg->m_name.first.empty() && Arg->m_name.first == value.substr(0, Arg->m_name.first.length()) + "=") {
                        addIn(value, Arg);
                        add = true;
                    } else if (!Arg->m_name.second.empty() && Arg->m_name.second == value.substr(0, Arg->m_name.second.length()) + "=") {
                        Arg->AddValue(true);
                        add = true;
                    } else {
                        addIn(value, Arg);
                    }
                } else if (Arg->type == "string") {
                    if (!Arg->m_name.first.empty() && Arg->m_name.first == value.substr(0, Arg->m_name.first.length())) {
                        Arg->AddValue(value.substr(Arg->m_name.first.length(), value.length()));
                        add = true;
                    } else if (!Arg->m_name.second.empty() && Arg->m_name.second == value.substr(0, Arg->m_name.second.length())) {
                        Arg->AddValue(value.substr(Arg->m_name.second.length(), value.length()));
                        add = true;
                    }
                } else if (Arg->type == "int") {
                    if (!Arg->m_name.first.empty() && Arg->m_name.first == value.substr(0, Arg->m_name.first.length()) &&
                        isNumeric(value.substr(Arg->m_name.first.length(), value.length()))) {
                        Arg->AddValue(stoi(value.substr(Arg->m_name.first.length(), value.length())));
                        add = true;
                    } else if (!Arg->m_name.second.empty() && Arg->m_name.second == value.substr(0, Arg->m_name.second.length()) &&
                               isNumeric(value.substr(Arg->m_name.second.length(), value.length()))) {
                        Arg->AddValue(stoi(value.substr(Arg->m_name.second.length(), value.length())));
                        add = true;
                    }
                }
            }
        }
    }
    for (const auto& value: val) {
        if (!add) {
            for (auto& Arg: arguments) {
                if (Arg->type == "string" && Arg->m_isPositional && !isNumeric(value)) {
                    Arg->AddValue(value.substr(Arg->m_name.second.length(), value.length()));
                }
            }
        }
    }
    for (const auto& value: val) {
        if (!add) {
            for (auto& Arg: arguments) {
                if (Arg->type == "int" && Arg->m_isPositional && isNumeric(value)) {
                    Arg->AddValue(stoi(value));
                }
            }
        }
    }


    for (auto& Arg: arguments) {
        if (Arg->type == "string") {
            if ((!Arg->m_isDefault && Arg->m_stringValue.empty()) ||
                (Arg->m_isMulti && (Arg->m_stringValue.size() - Arg->m_multiSize) < 0)) {
                return false;
            }
        } else if (Arg->type == "int") {
            if ((!Arg->m_isDefault && Arg->m_intValue.empty()) ||
                (Arg->m_isMulti && Arg->m_multiSize != -1 && Arg->m_intValue.size() < Arg->m_multiSize)) {
                return false;
            }
        } else {
            if (!Arg->m_isDefault && Arg->m_boolValue.empty()) {
                return false;
            }
        }
    }
    return true;
}


HelpArg::HelpArg(char chArg, std::string help) {
    m_name.first = "-" + std::string(1, chArg);
    m_help = std::move(help);
}

HelpArg::HelpArg(const std::string& stArg, std::string help) {
    m_name.second = "--" + stArg;
    m_help = std::move(help);
}

HelpArg::HelpArg(char chArg, const std::string& stArg, std::string help) {
    m_name.first = "-" + std::string(1, chArg);
    m_name.second = "--" + stArg;
    m_help = std::move(help);
}

HelpArg& ArgumentParser::ArgParser::AddHelp(const char& chArg, const std::string& help) {
    HelpArg arg(chArg, help);
    HelpArgV = arg;
    return HelpArgV;
}

HelpArg& ArgumentParser::ArgParser::AddHelp(const std::string& stArg, const std::string& help) {
    HelpArg arg(stArg, help);
    HelpArgV = arg;
    return HelpArgV;
}

HelpArg& ArgumentParser::ArgParser::AddHelp(const char chArg, const std::string& stArg, const std::string& help) {
    HelpArg arg(chArg, stArg, help);
    HelpArgV = arg;
    return HelpArgV;
}

bool ArgumentParser::ArgParser::Help() const {
    if (!HelpArgV.m_help.empty()) {
        return true;
    }
    return false;
}

std::string ArgumentParser::ArgParser::StringArgHelp() {
    std::string strDesc;
    for (auto& arg: arguments) {
        if (!arg->m_stringValue.empty()) {
            if (!arg->m_name.first.empty()) {
                strDesc += arg->m_name.first + ", ";
            } else {
                strDesc += "     ";
            }
            if (!arg->m_name.second.empty()) {
                strDesc += arg->m_name.second;
            }
            strDesc += "<string>, ";
            if (!arg->m_help.empty()) {
                strDesc += arg->m_help + " ";
            }
            bool parent = false;
            if (arg->m_isDefault) {
                strDesc += "[default = " + arg->m_stringDefaultVal;
                parent = true;
            }
            if (arg->m_isMulti) {
                if (parent) {
                    strDesc += ", repeated";
                } else {
                    strDesc += "[repeated";
                    parent = true;
                }
                strDesc += ", min args = " + std::to_string(arg->m_multiSize);
            }
            if (arg->m_isPositional) {
                if (parent) {
                    strDesc += ", positional]";
                    parent = false;
                } else {
                    strDesc += "[positional]";
                    parent = false;
                }
            }
            if (parent) {
                strDesc += ']';
            }
            strDesc += '\n';
        }
    }
    return strDesc;
}

std::string ArgumentParser::ArgParser::IntArgHelp() {
    std::string intDesc;
    for (auto& arg: arguments) {
        if (!arg->m_intValue.empty()) {
            if (!arg->m_name.first.empty()) {
                intDesc += arg->m_name.first + ", ";
            } else {
                intDesc += "     ";
            }
            if (!arg->m_name.second.empty()) {
                intDesc += arg->m_name.second;
            }
            intDesc += "<int>, ";
            if (!arg->m_help.empty()) {
                intDesc += arg->m_help + " ";
            }
            bool parent = false;
            if (arg->m_isDefault) {
                intDesc += "[default = " + std::to_string(arg->m_intDefaultVal);
                parent = true;
            }
            if (arg->m_isMulti) {
                if (parent) {
                    intDesc += ", repeated";
                } else {
                    intDesc += "[repeated";
                    parent = true;
                }
                intDesc += ", min args = " + std::to_string(arg->m_multiSize);
            }
            if (arg->m_isPositional) {
                if (parent) {
                    intDesc += ", positional]";
                    parent = false;
                } else {
                    intDesc += "[positional]";
                    parent = false;
                }
            }
            if (parent) {
                intDesc += ']';
            }
            intDesc += '\n';
        }
    }
    return intDesc;
}

std::string ArgumentParser::ArgParser::FlagArgHelp() {
    std::string flagDesc;
    for (auto& arg: arguments) {
        if (!arg->m_boolValue.empty()) {
            if (!arg->m_name.first.empty()) {
                flagDesc += arg->m_name.first + ", ";
            } else {
                flagDesc += "     ";
            }
            if (!arg->m_name.second.empty()) {
                flagDesc += arg->m_name.second + " ";
            }
            if (!arg->m_help.empty()) {
                flagDesc += arg->m_help + " ";
            }
            bool parent = false;
            if (arg->m_isDefault) {
                flagDesc += "[default = " + std::to_string(arg->m_boolDefaultVal);
                parent = true;
            }
            if (parent) {
                flagDesc += ']';
            }
            flagDesc += '\n';
        }
    }
    return flagDesc;
}

std::string ArgumentParser::ArgParser::HelpArgHelp() {
    std::string helpDesc;
    if (!HelpArgV.m_name.first.empty()) {
        helpDesc += HelpArgV.m_name.first + ", ";
    }
    if (!HelpArgV.m_name.second.empty()) {
        helpDesc += HelpArgV.m_name.second + " ";
    }
    helpDesc += "Display this help and exit\n";
    return helpDesc;
}

void ArgumentParser::ArgParser::HelpDescription() {
    std::string help;
    std::string strDesc = StringArgHelp();
    std::string intDesc = IntArgHelp();
    std::string boolDesc = FlagArgHelp();
    std::string helpDesc = HelpArgHelp();
    help = m_name + '\n' + HelpArgV.m_help + "\n\n";
    std::cout << help << strDesc << intDesc << boolDesc << '\n' << helpDesc;
}
