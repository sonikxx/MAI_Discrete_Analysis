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

    struct TNode
    {
        string value;
        int errorLink; // связь неудач
        int exitLink;  // связь выхода
        vector<int> patternNumber;
        bool isTerminator;
        unsigned int patternSize;
        map<string, unsigned int> children;

        TNode(string s)
        {
            isTerminator = false;
            errorLink = 0;
            exitLink = -1;
            patternSize = 0;
            value = s;
        }

        bool haveChild(const string &letter)
        {
            return children.find(letter) != children.end();
        }

        int GoToChild(const string &letter)
        {
            return children[letter];
        }

        void AddChild(const string &letter, unsigned int ind)
        {
            children[letter] = ind;
        }
    };

    vector<TNode> storage;
    vector<TLetter> text;
    vector<unsigned int> matches;
    bool caseSensitive = true;
    unsigned int trieIter = 0;
    unsigned int patternsCount = 0;
    unsigned int pattetnTotalSize = 0;

    int NewNode(const string &value)
    {
        storage.push_back(TNode(value));
        return storage.size() - 1;
    }

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
        int cur = 0;
        for (const string &letter : s)
        {
            if (storage[cur].haveChild(letter) == false)
            {
                unsigned int newNode = NewNode(letter);
                storage[cur].AddChild(letter, newNode);
            }
            cur = storage[cur].GoToChild(letter);
        }
        storage[cur].isTerminator = true;
        storage[cur].patternNumber.push_back(id);
        storage[cur].patternSize = s.size();
    }

    void Linkate(string letter, unsigned int parent, unsigned int v)
    {
        if (parent == 0)
        {
            storage[v].errorLink = 0;
            return;
        }

        int cur = storage[parent].errorLink;
        while (cur > 0 && storage[cur].haveChild(letter) == false)
        {
            cur = storage[cur].errorLink;
        }
        if (cur == 0 && storage[0].haveChild(letter) == false)
        {
            storage[v].errorLink = 0;
        }
        else
        {
            storage[v].errorLink = storage[cur].GoToChild(letter);
            if (storage[storage[v].errorLink].isTerminator)
            {
                storage[v].exitLink = storage[v].errorLink;
            }
            else if (storage[storage[v].errorLink].exitLink != -1)
            {
                storage[v].exitLink = storage[storage[v].errorLink].exitLink;
            }
        }
    }

    void Preprocessing()
    {
        /* BFS in patterns trie */
        queue<unsigned int> q;
        q.push(0);

        while (!q.empty())
        {
            unsigned int cur = q.front();
            q.pop();

            for (const auto &pairKeyVal : storage[cur].children)
            {
                string letter = pairKeyVal.first;
                unsigned int v = pairKeyVal.second;
                Linkate(letter, cur, v);
                q.push(v);
            }
        }
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
        for (unsigned int i = 0; i < text.size(); i++)
        {
            string letter = text[i].value;
            while (trieIter != 0 && storage[trieIter].haveChild(letter) == false)
            {
                trieIter = storage[trieIter].errorLink;
            }
            if (storage[trieIter].haveChild(letter) == false)
            {
                continue;
            }
            trieIter = storage[trieIter].GoToChild(letter);
            int cur = trieIter;
            while (storage[cur].exitLink != -1)
            {
                cur = storage[cur].exitLink;
                int occurrenceInd = (i + 1) - storage[cur].patternSize;
                for (int i = 0; i < storage[cur].patternNumber.size(); ++i)
                {
                    if (occurrenceInd - storage[cur].patternNumber[i] + 1 >= 0)
                    {
                        ++matches[occurrenceInd - storage[cur].patternNumber[i] + 1];
                    }
                }
            }

            if (storage[trieIter].isTerminator)
            {
                int occurrenceInd = (i + 1) - storage[trieIter].patternSize;
                for (int i = 0; i < storage[trieIter].patternNumber.size(); ++i)
                {
                    if (occurrenceInd - storage[trieIter].patternNumber[i] + 1 >= 0)
                    {
                        ++matches[occurrenceInd - storage[trieIter].patternNumber[i] + 1];
                    }
                }
            }
        }
    }

public:
    TAhoCorasickAlgo()
    {
        NewNode("");
    }

    TAhoCorasickAlgo(bool caseS)
    {
        caseSensitive = caseS;
        NewNode("");
    }

    void RunAlgo()
    {
        ReadPattern();
        Preprocessing();
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
