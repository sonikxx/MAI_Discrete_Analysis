#include <map>
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class TAhoCorasickAlgo
{
private:
    struct TLetter
    {
        unsigned int lineId;
        unsigned int letterId;
        string value;
    };

    vector<pair<int, vector<string>>> storage;
    vector<TLetter> text;
    vector<unsigned int> matches;
    bool caseSensitive = true;
    unsigned int trieIter = 0;
    unsigned int patternsCount = 0;
    unsigned int pattetnTotalSize = 0;

    void PrintOccurrence()
    {
        for (unsigned int ind = 0; ind < matches.size(); ++ind)
        {
            if (patternsCount == matches[ind] && ind + pattetnTotalSize <= text.size())
            {
                cout << text[ind].lineId << ", " << text[ind].letterId << "\n";
            }
        }
    }

    void StringToLower(string &s)
    {
        for (char &c : s)
        {
            c = tolower(c);
        }
    }

    void AddPattern(unsigned int id, const vector<string> &s)
    {
        storage.push_back({id, s});
    }

    void ReadPattern()
    {
        string inputLine;
        getline(cin, inputLine);
        string tmp;
        string prev = "?";
        vector<string> pattern;
        istringstream input(inputLine);
        int patternId = 1;
        while (input >> tmp)
        {
            ++pattetnTotalSize;
            if (tmp == "?")
            {
                if (prev == "?")
                {
                    patternId = pattetnTotalSize + 1;
                    continue;
                }
                AddPattern(patternId, pattern);
                ++patternsCount;
                pattern.clear();
                patternId = pattetnTotalSize + 1;
                prev = "?";
                continue;
            }
            if (!caseSensitive)
            {
                StringToLower(tmp);
            }
            prev = tmp;
            pattern.push_back(tmp);
        }
        if (tmp != "?")
        {
            AddPattern(patternId, pattern);
            ++patternsCount;
            pattern.clear();
        }
    }

    void ReadText()
    {
        string inputLine;
        string tmp;
        for (unsigned int lineId = 1; getline(cin, inputLine); ++lineId)
        {
            istringstream input(inputLine);
            unsigned int letterId = 0;
            while (input >> tmp)
            {
                if (!caseSensitive)
                {
                    StringToLower(tmp);
                }
                text.push_back({lineId, ++letterId, tmp});
            }
        }
    }

    void Search()
    {
        matches.resize(text.size(), 0);
        for (int i = 0; i < storage.size(); ++i)
        {
            for (int k = 0; k < text.size(); ++k)
            {
                int good = 0;
                for (int n = 0; n < storage[i].second.size() && k + n < text.size(); ++n)
                {
                    if (text[k + n].value != storage[i].second[n])
                    {
                        break;
                    }
                    ++good;
                }
                if (good == storage[i].second.size())
                {
                    if (k - storage[i].first + 1 >= 0)
                    {
                        ++matches[k - storage[i].first + 1];
                    }
                }
            }
        }
    }

public:
    TAhoCorasickAlgo(bool caseS)
    {
        caseSensitive = caseS;
    }

    void RunAlgo()
    {
        ReadPattern();
        ReadText();
        Search();
        PrintOccurrence();
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    TAhoCorasickAlgo algo(false); /* caseSensitive = false */
    algo.RunAlgo();
    return 0;
}
