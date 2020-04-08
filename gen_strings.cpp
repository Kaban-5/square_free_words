#include <bits/stdc++.h>

using namespace std;
using ll = long long;

bool bad (const string &s)
{
    for (int len = 1; 2 * len < int(s.size()); len++)
    {
        for (int i = 0; i + 2 * len <= int(s.size()); i++)
        {
            if (equal(s.begin() + i, s.begin() + i + len, s.begin() + i + len))
                return true;
        }
    }

    return false;
}

bool bad_double (const string &s)
{
    return bad(s + s);
}

void gen (string s, ostream &out, int &cnt) {
    if (!s.empty() && !bad_double(s))
    {
        out << s << endl;
        cnt++;
    }

    if (int(s.size()) == dictionary_word_length)
        return;

    for (char ch = 'a'; ch <= 'c'; ch++)
    {
        s += ch;
        bool ok = true;
        for (int i = 1; ok && 2 * i <= int(s.size()); i++)
            ok &= !equal(s.end() - 2 * i, s.end() - i, s.end() - i);

        if (ok)
            gen(s, out, cnt);
        s.pop_back();
    }
}

void solve(istream &cin = std::cin, ostream &cout = std::cout)
{
    int cnt = 0;
    gen("", cout, cnt);
    cerr << "cnt = " << cnt << endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    ofstream fout("../dictionary.txt");
    solve(cin, fout);

    cout << fixed;
    cout << double(clock()) / CLOCKS_PER_SEC << endl;

    return 0;
}
