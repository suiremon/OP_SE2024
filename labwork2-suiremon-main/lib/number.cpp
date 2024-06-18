#include "number.h"
#include <cstring>
#include <cmath>
#include <cstdlib>

int2023_t ToNeg(int2023_t num) {
    num.data[int2023_t::INDEX_MAX] += int2023_t::HALF_BASE;
    return num;
}

bool IsNeg(const int2023_t num) {
    return num.data[int2023_t::INDEX_MAX] >= int2023_t::HALF_BASE;
}

int2023_t GetAbs(int2023_t num) {
    if (IsNeg(num)) {
        return ToNeg(num);
    }
    return num;
}

int2023_t SumAbs(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    uint8_t carry = 0;

    for (int i = 0; i < int2023_t::SIZE; i++) {
        uint16_t sum = lhs.data[i] + rhs.data[i] + carry;
        result.data[i] = sum & 0xFF;
        carry = sum >> int2023_t::BYTE_SIZE;
    }
    return result;
}

int2023_t SubstAbs(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    uint8_t carry = 0;

    for (int i = 0; i < int2023_t::SIZE; i++) {
        int diff = lhs.data[i] - rhs.data[i] - carry;
        if (diff < 0) {
            diff += int2023_t::HALF_BASE * 2;
            carry = 1;
        } else {
            carry = 0;
        }
        result.data[i] = diff;
    }
    return result;
}

int2023_t DivTwo(const int2023_t& lhs) {
    int2023_t result = lhs;
    int carry = 0;

    for (int i = int2023_t::INDEX_MAX; i >= 0; i--) {
        int value = (result.data[i] + (carry << int2023_t::BYTE_SIZE)) / 2;
        carry = (result.data[i] + (carry << int2023_t::BYTE_SIZE)) % 2;
        result.data[i] = value;
    }
    return result;
}

int2023_t from_int(int32_t i) {
    int2023_t result;
    int usedCount = 0;

    if (i < 0) {
        i *= -1;
        result = ToNeg(result);
    }
    while (i > 0) {
        result.data[usedCount] = i & 0xFF;
        i >>= int2023_t::BYTE_SIZE;
        usedCount++;
    }
    return result;
}

int2023_t from_string(const char *buff) {
    int2023_t result = 0;
    int usedCount = 0;
    int step = 18;
    char tmp[step];

    for (int i = 0; i < strlen(buff); i += step) {
        if (usedCount + step <= strlen(buff)) {
            result *= pow(10, step);
            strncpy(tmp, buff + usedCount, step);
            usedCount += step;
            if (tmp[0] == '-') {
                tmp[0] = '0';
            }
            result += atoll(tmp);
        } else {
            result *= pow(10, strlen(buff) - usedCount);
            result += std::abs(atoll(buff + usedCount));
        }
    }
    if (buff[0] == '-') {
        result = ToNeg(result);
    }
    return result;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    int2023_t lhsAbs = GetAbs(lhs);
    int2023_t rhsAbs = GetAbs(rhs);

    if ((IsNeg(lhs) && IsNeg(rhs)) || ((!IsNeg(lhs) && !IsNeg(rhs)))) { //-- || ++
        result = SumAbs(lhsAbs, rhsAbs);
        if (IsNeg(lhs)) {
            result = ToNeg(result);
        }
    } else if (!IsNeg(lhs) && IsNeg(rhs)) { //+-
        result = lhs - rhsAbs;
    } else if (IsNeg(lhs) && !IsNeg(rhs)) { //-+
        result = rhs - lhsAbs;
    }
    return result;
}

int2023_t operator+=(int2023_t& lhs, const int2023_t& rhs) {
    lhs = lhs + rhs;
    return lhs;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    int2023_t lhsAbs = GetAbs(lhs);
    int2023_t rhsAbs = GetAbs(rhs);

    if ((!IsNeg(lhs) && !IsNeg(rhs))) { //++
        result = SubstAbs(lhsAbs, rhsAbs);
        if (lhs < rhs) {
            result = SubstAbs(rhsAbs, lhsAbs);
            result = ToNeg(result);
        }
    } else if (IsNeg(lhs) && IsNeg(rhs)) { //--
        result = SubstAbs(rhsAbs, lhsAbs);
        if (lhs < rhs) {
            result = SubstAbs(lhsAbs, rhsAbs);
            result = ToNeg(result);
        }
    } else if (IsNeg(lhs) && !IsNeg(rhs)) { //-+
        result = lhsAbs + rhs;
        result = ToNeg(result);
    } else if (!IsNeg(lhs) && IsNeg(rhs)) { //+-
        result = lhs + rhsAbs;
    }
    return result;
}

int2023_t operator-=(int2023_t& lhs, const int2023_t& rhs) {
    lhs = lhs - rhs;
    return lhs;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    int2023_t lhsAbs = GetAbs(lhs);
    int2023_t rhsAbs = GetAbs(rhs);

    for (int i = 0; i < int2023_t::SIZE; ++i) {
        for (int j = 0; j < int2023_t::SIZE; ++j) {
            int k = i + j;
            if (k < int2023_t::SIZE) {
                int temp = result.data[k] + lhsAbs.data[i] * rhsAbs.data[j];
                result.data[k] = temp & 0xFF;
                int carry = (temp >> int2023_t::BYTE_SIZE) & 0xFF;
                for (int l = k + 1; l < int2023_t::SIZE && carry != 0; ++l) {
                    temp = result.data[l] + carry;
                    result.data[l] = temp & 0xFF;
                    carry = (temp >> int2023_t::BYTE_SIZE) & 0xFF;
                }
            }
        }
    }
    if ((IsNeg(lhs) && !IsNeg(rhs)) || (!IsNeg(lhs) && IsNeg(rhs))) {
        result = ToNeg(result);
    }
    return result;
}

int2023_t operator*=(int2023_t& lhs, const int2023_t& rhs) {
    lhs = lhs * rhs;
    return lhs;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result = 0;
    int2023_t lhsAbs = GetAbs(lhs);
    int2023_t rhsAbs = GetAbs(rhs);
    int2023_t low = 0;
    int2023_t high = lhsAbs;
    int2023_t mid;

    if (rhsAbs == 1) {
        return lhs;
    }

    while (low <= high) {
        mid = low + DivTwo(high - low);
        if (mid * rhsAbs > lhsAbs)
            high = mid - 1;
        else {
            result = mid;
            low = mid + 1;
        }
    }
    if (IsNeg(lhs) != IsNeg(rhs) && result != 0) {
        result = ToNeg(result);
    }
    return result;
}

int2023_t operator/=(int2023_t& lhs, const int2023_t& rhs) {
    lhs = lhs / rhs;
    return lhs;
}

bool operator>(const int2023_t& lhs, const int2023_t& rhs) {
    if (rhs == lhs) {
        return false;
    } else if (IsNeg(lhs) != IsNeg(rhs)) {
        return IsNeg(lhs) < IsNeg(rhs);
    } else {
        for (int i = int2023_t::INDEX_MAX; i >= 0; i--) {
            if (lhs.data[i] > rhs.data[i]) {
                return !IsNeg(lhs);
            } else if (lhs.data[i] < rhs.data[i]) {
                return IsNeg(lhs);
            }
        }
    }
    return false;
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
    if (rhs == lhs) {
        return false;
    } else if (IsNeg(lhs) != IsNeg(rhs)) {
        return IsNeg(lhs) > IsNeg(rhs);
    } else {
        for (int i = int2023_t::INDEX_MAX; i >= 0; i--) {
            if (lhs.data[i] > rhs.data[i]) {
                return IsNeg(rhs);
            } else if (lhs.data[i] < rhs.data[i]) {
                return !IsNeg(rhs);
            }
        }
    }
    return false;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    for (int i = 0; i < int2023_t::SIZE; i++) {
        if (lhs.data[i] != rhs.data[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    for (int i = 0; i < int2023_t::SIZE; i++) {
        if (lhs.data[i] != rhs.data[i]) {
            return true;
        }
    }
    return false;
}

bool operator>=(const int2023_t& lhs, const int2023_t& rhs) {
    return (lhs > rhs || lhs == rhs);
}

bool operator<=(const int2023_t& lhs, const int2023_t& rhs) {
    return (lhs < rhs || lhs == rhs);
}

std::ostream &operator<<(std::ostream& stream, const int2023_t& value) {
    bool neg = false;
    bool fl = false;

    if (IsNeg(value)) {
        neg = true;
        stream << "-";
        if (value.data[int2023_t::INDEX_MAX] != int2023_t::HALF_BASE) {
            stream << (int) value.data[int2023_t::INDEX_MAX] - int2023_t::HALF_BASE << " ";
        }
    }
    for (int i = int2023_t::INDEX_MAX - neg; i >= 0; i--) {
        if (value.data[i] != 0 || fl) {
            stream << (int) value.data[i] << " ";
            fl = true;
        }
    }
    return stream;
}
