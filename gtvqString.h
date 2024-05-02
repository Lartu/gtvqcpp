#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class gtvqString
{
private:
    vector<string> buffer;
    string stringRep;
    size_t gtvqString_get_str_utf8length(const string cstr);
    void createFromString(const string &cstr);
    void createFromChar(const char *cstr);
    void createFromMem(const char *mem, size_t len);

public:
    size_t size() const;
    bool empty() const;
    size_t length() const;
    string str_rep();
    gtvqString();
    gtvqString(const string &x);
    gtvqString(const double &f);
    gtvqString &operator=(const string &x);
    gtvqString(const char *x);
    gtvqString &operator=(const char *x);
    gtvqString(const char *x, size_t len);
    gtvqString(char x);
    gtvqString &operator=(char x);
    gtvqString operator[](size_t i) const;
    string &operator[](int i);
    friend ostream &operator<<(ostream &out, const gtvqString &c);
    friend gtvqString operator+(const gtvqString &c1, const gtvqString &c2);
    friend gtvqString operator+(const string &c1, const gtvqString &c2);
    friend gtvqString operator+(const gtvqString &c1, const string &c2);
    friend gtvqString operator+(const char *c1, const gtvqString &c2);
    friend gtvqString operator+(const gtvqString &c1, const char *c2);
    friend bool operator<(const gtvqString &c1, const gtvqString &c2);
    friend bool operator>(const gtvqString &c1, const gtvqString &c2);
    friend bool operator==(const gtvqString &ch1, const gtvqString &ch2);
    friend bool operator==(const gtvqString &ch1, const string &ch2);
    friend bool operator==(const gtvqString &ch1, const char *ch2);
    friend bool operator==(const string &ch2, const gtvqString &ch1);
    friend bool operator==(const char *ch2, const gtvqString &ch1);
    friend bool operator==(const gtvqString &ch1, const char ch2);
    friend bool operator==(const char ch2, const gtvqString &ch1);
    friend bool operator!=(const gtvqString &ch1, const gtvqString &ch2);
    bool loadFile(const string &fileName);
    gtvqString &operator+=(const gtvqString &txt);
    gtvqString &operator+=(const string &txt);
    gtvqString &operator+=(const char *txt);
    bool isAlphanumeric();
    bool isAlphanumeric(size_t from);
    bool isNumber();
    double getNumber();
    gtvqString substr(size_t from, size_t count);
    gtvqString &erase(size_t from, size_t count);
    gtvqString substr(size_t from);
    int compare(size_t from, size_t count, const gtvqString &other);
    int compare(const gtvqString &other);
};

ostream &operator<<(ostream &out, const gtvqString &c);
gtvqString operator+(const gtvqString &c1, const gtvqString &c2);
gtvqString operator+(const string &c1, const gtvqString &c2);
gtvqString operator+(const gtvqString &c1, const string &str);
gtvqString operator+(const char *c1, const gtvqString &c2);
gtvqString operator+(const gtvqString &c1, const char *str);
bool operator==(const gtvqString &ch1, const gtvqString &ch2);
bool operator==(const string &c1, const gtvqString &ch2);
bool operator==(const gtvqString &ch1, const string &c2);
bool operator==(const char *c1, const gtvqString &ch2);
bool operator==(const gtvqString &ch1, const char *c2);
bool operator==(const char c1, const gtvqString &ch2);
bool operator==(const gtvqString &ch1, const char c2);
bool operator<(const gtvqString &c1, const gtvqString &c2);
bool operator>(const gtvqString &c1, const gtvqString &c2);
bool operator!=(const gtvqString &ch1, const gtvqString &ch2);
gtvqString to_ldpl_string(double x);

// Gets length of utf8-encoded c++ string
size_t gtvqString::gtvqString_get_str_utf8length(const string cstr)
{
    size_t len = cstr.size();
    size_t utf8len = 0;
    unsigned int c;
    for (size_t i = 0; i < len; i++)
    {
        size_t l = 0;
        c = (unsigned char)cstr[i];
        if (c >= 0 && c <= 127)
            l = 0;
        else if ((c & 0xE0) == 0xC0)
            l = 1;
        else if ((c & 0xF0) == 0xE0)
            l = 2;
        else if ((c & 0xF8) == 0xF0)
            l = 3;
        else if (c >= 248 && c <= 255)
            return 0; // invalid utf8
        i += l;
        utf8len++;
    }
    return utf8len;
}
// Fills buffer with utf8-encoded c++ string
void gtvqString::createFromString(const string &cstr)
{
    createFromMem(cstr.c_str(), cstr.size());
}
// Fills buffer with utf8-encoded c++ string
void gtvqString::createFromChar(const char *cstr)
{
    // If we copy the implementation we can do without the `strlen` call.
    createFromMem(cstr, strlen(cstr));
}
// Fills buffer with utf8-encoded c++ string
void gtvqString::createFromMem(const char *cstr, size_t cstrlen)
{
    buffer.clear();
    size_t chPos = 0;
    for (size_t i = 0; i < cstrlen; ++i)
    {
        string ch = "";
        char c = cstr[i];
        if (c >= 0 && c <= 127)
        {
            ch += c;
        }
        else if ((c & 0xE0) == 0xC0)
        {
            ch += c;
            ch += cstr[++i];
        }
        else if ((c & 0xF0) == 0xE0)
        {
            ch += c;
            ch += cstr[++i];
            ch += cstr[++i];
        }
        else if ((c & 0xF8) == 0xF0)
        {
            ch += c;
            ch += cstr[++i];
            ch += cstr[++i];
            ch += cstr[++i];
        }
        buffer.push_back(ch);
        chPos++;
    }
}
size_t gtvqString::size() const { return buffer.size(); }
bool gtvqString::empty() const { return buffer.empty(); }
size_t gtvqString::length() const { return size(); }
string gtvqString::str_rep()
{
    // TODO this can be optimized to make it more performant and not having to
    // recreate the string representation every time!
    stringRep = "";
    for (size_t i = 0; i < size(); ++i)
    {
        stringRep += buffer[i];
    }
    return stringRep;
}
// default constructor
gtvqString::gtvqString() {}
// conversion from string (constructor):
gtvqString::gtvqString(const string &x) { createFromString(x); }

// conversion from double (constructor):
gtvqString::gtvqString(const double &f)
{
    std::string str = to_string(f);
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    str.erase(str.find_last_not_of('.') + 1, std::string::npos);
    createFromString(str);
}

// conversion from string (assignment):
gtvqString &gtvqString::operator=(const string &x)
{
    createFromString(x);
    return *this;
}
// conversion from char * (constructor):
gtvqString::gtvqString(const char *x) { createFromChar(x); }
// conversion from char * (assignment):
gtvqString &gtvqString::operator=(const char *x)
{
    createFromChar(x);
    return *this;
}
// conversion from char * and size (constructor):
gtvqString::gtvqString(const char *x, size_t len) { createFromMem(x, len); }
// conversion from char (constructor):
gtvqString::gtvqString(char x) { createFromMem(&x, 1); }
// conversion from char (assignment):
gtvqString &gtvqString::operator=(char x)
{
    createFromMem(&x, 1);
    return *this;
}
// [] for reading
gtvqString gtvqString::operator[](size_t i) const
{
    if (i >= buffer.size())
    {
        cout << "Out-of-bounds index access." << endl;
        exit(1);
    }
    gtvqString c = buffer[i];
    return c;
}
// [] for setting
string &gtvqString::operator[](int i)
{
    if (i >= buffer.size())
    {
        cout << "Out-of-bounds index access." << endl;
        exit(1);
    }
    return buffer[i];
}
// += operator
gtvqString &gtvqString::operator+=(const gtvqString &txt)
{
    buffer.insert(buffer.end(), txt.buffer.begin(), txt.buffer.end());
    return *this;
}
// += operator
gtvqString &gtvqString::operator+=(const string &txt)
{
    gtvqString c2 = txt;
    buffer.insert(buffer.end(), c2.buffer.begin(), c2.buffer.end());
    return *this;
}
// += operator
gtvqString &gtvqString::operator+=(const char *txt)
{
    gtvqString c2 = txt;
    buffer.insert(buffer.end(), c2.buffer.begin(), c2.buffer.end());
    return *this;
}

bool gtvqString::isAlphanumeric()
{
    for (const string &s : buffer)
    {
        for (const char &c : s)
            if (!isalnum(c))
                return false;
    }
    return true;
}

bool gtvqString::isAlphanumeric(size_t from)
{
    for (size_t i = from; i < size(); ++i)
    {
        for (const char &c : buffer[i])
            if (!isalnum(c))
                return false;
    }
    return true;
}

bool gtvqString::isNumber()
{
    string number = "";
    for (size_t i = 0; i < size(); ++i)
    {
        number += buffer[i];
    }
    unsigned int firstchar = 0;
    if (number[0] == '-')
        firstchar = 1;
    if (number[firstchar] == '.')
        return false; //.12 is not a valid decimal in LDPL, 0.12 is.
    if (number[firstchar] == '+')
        return false; //+5 is not a valid decimal in LDPL, 5 is.
    istringstream iss(number);
    double f;
    iss >> f;
    bool isNumber = iss.eof() && !iss.fail();
    // If it is a number, it might be an octal literal (e.g. 025, 067, 003 etc)
    // so we proceed to fix the original number to make it decimal.
    if (isNumber)
    {
        string f_number = "";
        unsigned int i;
        for (i = 1; i < number.length(); ++i)
        {
            // If prev char not 0
            if (number[i - 1] != '0')
            {
                // if prev char is -, continue check
                if (number[i - 1] == '-')
                    f_number += '-';
                // if prev char is number, break
                else
                    break;
            }
            // If prev number is 0
            else if (number[i] == '.')
            {
                f_number += '0';
            }
        }
        f_number += number.substr(i - 1);
        number = f_number;
        return true;
    }
    else
    {
        return false;
    }
}

double gtvqString::getNumber()
{
    string number = "";
    for (size_t i = 0; i < size(); ++i)
    {
        number += buffer[i];
    }
    return stod(number);
}

gtvqString gtvqString::substr(size_t from, size_t count)
{
    count = from + count > buffer.size() ? buffer.size() - from : count;
    gtvqString newText;
    newText.buffer.insert(newText.buffer.begin(), buffer.begin() + from,
                          buffer.begin() + from + count);
    return newText;
}

gtvqString &gtvqString::erase(size_t from, size_t count)
{
    buffer.erase(buffer.begin() + from, buffer.begin() + from + count);
    return *this;
}

gtvqString gtvqString::substr(size_t from)
{
    gtvqString newText;
    newText.buffer.insert(newText.buffer.begin(), buffer.begin() + from,
                          buffer.end());
    return newText;
}

// NOTE: returns 0 on equality, -1 if the string is shorter, and 1 in any other
// case.
int gtvqString::compare(size_t from, size_t count, const gtvqString &other)
{
    // Fix count to respect the actual end of the buffer.
    count = from + count > buffer.size() ? buffer.size() - from : count;
    // Compare sizes before anything else for efficiency.
    if (count < other.buffer.size())
        return -1;
    if (count > other.buffer.size())
        return 1;
    for (size_t i = from, j = 0; j < count; ++i, ++j)
        if (buffer[i] != other.buffer[j])
            return 1; // We already know it's not shorter, see above.
    return 0;
}

int gtvqString::compare(const gtvqString &other)
{
    if (*this == other)
        return 0;
    if (this->size() < other.size())
        return -1;
    else
        return 1;
}

ostream &operator<<(ostream &out, const gtvqString &c)
{
    for (const string &s : c.buffer)
    {
        out << s;
    }
    return out;
}

gtvqString operator+(const gtvqString &c1, const gtvqString &c2)
{
    gtvqString res = c1;
    res.buffer.insert(res.buffer.end(), c2.buffer.begin(), c2.buffer.end());
    return res;
}

gtvqString operator+(const string &c1, const gtvqString &c2)
{
    gtvqString res = c1;
    res.buffer.insert(res.buffer.end(), c2.buffer.begin(), c2.buffer.end());
    return res;
}

gtvqString operator+(const gtvqString &c1, const string &str)
{
    gtvqString res = c1;
    gtvqString c2 = str;
    res.buffer.insert(res.buffer.end(), c2.buffer.begin(), c2.buffer.end());
    return res;
}

gtvqString operator+(const char *c1, const gtvqString &c2)
{
    gtvqString res = c1;
    res.buffer.insert(res.buffer.end(), c2.buffer.begin(), c2.buffer.end());
    return res;
}

gtvqString operator+(const gtvqString &c1, const char *str)
{
    gtvqString res = c1;
    gtvqString c2 = str;
    res.buffer.insert(res.buffer.end(), c2.buffer.begin(), c2.buffer.end());
    return res;
}

bool operator==(const gtvqString &ch1, const gtvqString &ch2)
{
    return ch1.buffer == ch2.buffer;
}

bool operator==(const string &c1, const gtvqString &ch2)
{
    const gtvqString ch1 = c1;
    return ch1 == ch2;
}

bool operator==(const gtvqString &ch1, const string &c2)
{
    const gtvqString ch2 = c2;
    return ch1 == ch2;
}

bool operator==(const char *c1, const gtvqString &ch2)
{
    const gtvqString ch1 = c1;
    return ch1 == ch2;
}

bool operator==(const gtvqString &ch1, const char *c2)
{
    const gtvqString ch2 = c2;
    return ch1 == ch2;
}

bool operator==(const char c1, const gtvqString &ch2)
{
    const gtvqString ch1 = c1;
    return ch1 == ch2;
}

bool operator==(const gtvqString &ch1, const char c2)
{
    const gtvqString ch2 = c2;
    return ch1 == ch2;
}

bool operator<(const gtvqString &c1, const gtvqString &c2)
{
    size_t max = c1.buffer.size() > c2.buffer.size() ? c2.buffer.size() : c1.buffer.size();
    for (size_t i = 0; i < max; ++i)
    {
        if (c1.buffer[i] < c2.buffer[i])
            return true;
        else if (c1.buffer[i] > c2.buffer[i])
            return false;
    }
    return c1.buffer.size() < c2.buffer.size();
}

bool operator>(const gtvqString &c1, const gtvqString &c2)
{
    size_t max =
        c1.buffer.size() > c2.buffer.size() ? c2.buffer.size() : c1.buffer.size();
    for (size_t i = 0; i < max; ++i)
    {
        if (c1.buffer[i] > c2.buffer[i])
            return true;
        else if (c1.buffer[i] < c2.buffer[i])
            return false;
    }
    return c1.buffer.size() > c2.buffer.size();
}

bool operator!=(const gtvqString &ch1, const gtvqString &ch2)
{
    return ch1.buffer != ch2.buffer;
}

// Converts string to uppercase
void gtvqString_toUpper(gtvqString &str) {
  string out = str.str_rep();
  std::transform(out.begin(), out.end(), out.begin(), ::toupper);
  str = out;
}

// Converts string to lowercase
void gtvqString_toLower(gtvqString &str) {
  string out = str.str_rep();
  std::transform(out.begin(), out.end(), out.begin(), ::tolower);
  str = out;
}

// Removes all trailing and ending whitespace from a string
void gtvqString_trim(gtvqString &_line) {
  string line = _line.str_rep();
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
  _line = line;
}