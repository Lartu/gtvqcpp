#include "gtvq.h"

#define gtvqWordType uint8_t
#define GTVQWORD 1
#define GTVQSUBLINE 2

class gtvqWord
{
private:
    gtvqString _value = "";
    gtvqWordType _type = GTVQWORD;

public:
    gtvqWord(gtvqString value, uint8_t type)
    {
        _value = value;
        _type = type;
    }

    gtvqString &getValue()
    {
        return _value;
    }

    gtvqWordType getType()
    {
        return _type;
    }
};

class gtvqLine
{
private:
    vector<gtvqWord> words;

public:
    void addWord(gtvqWord word)
    {
        words.push_back(word);
    }

    vector<gtvqWord> &getWords()
    {
        return words;
    }
};