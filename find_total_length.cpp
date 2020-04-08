#include <bits/stdc++.h>
using namespace std;

void calc_number_of_states (int l, int r, int depth, const vector<string> &dictionary, int &ans) {
    if (l == r)
        return;

    ans++;
    if (int(dictionary[l].size()) == depth)
        l++;

    if (l == r)
        return;

    int where[4];
    where[0] = l;
    fill(where + 1, where + 4, r);

    int ptr = 1;
    for (int i = l; i < r; i++)
    {
        while (dictionary[i][depth] > 'a' + (ptr - 1))
            where[ptr++] = i;
    }

    for (int i = 0; i < 3; i++)
        calc_number_of_states(where[i], where[i + 1], depth + 1, dictionary, ans);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("../dictionary.txt");
    ofstream fout("../state_count.txt");

    vector<string> dictionary;
    string s;
    while (fin >> s)
    {
        if (int(s.size()) <= trie_word_length)
            dictionary.push_back(s + s);
    }

    int ans = 0;
    sort(dictionary.begin(), dictionary.end());
    calc_number_of_states(0, int(dictionary.size()), 0, dictionary, ans);

    fout << "need states = " << ans << endl;
    cout << ans << endl;

    return 0;
}
