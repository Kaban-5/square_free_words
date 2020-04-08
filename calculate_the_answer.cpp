#include <bits/stdc++.h>

using namespace std;
using ll = long long;

const int max_states = 6449335;

int trans[max_states][3];
bitset<max_states> bad;

struct trie
{
    int free_node;

    void create_node ()
    {
        assert(free_node < max_states);
        for (int i = 0; i < 3; i++)
            trans[free_node][i] = -1;
        free_node++;
    }

    trie ()
    {
        free_node = 0;
        create_node();
    }

    void add_word (const string &s)
    {
        int v = 0;
        for (char ch : s)
        {
            if (trans[v][ch - 'a'] == -1)
            {
                trans[v][ch - 'a'] = free_node;
                create_node();
            }

            v = trans[v][ch - 'a'];
        }

        bad[v] = true;
    }

    void build ()
    {
        assert(free_node == max_states);
        vector<int> link(max_states, -1);

        vector<int> q;
        q.reserve(max_states);
        q.push_back(0);
        link[0] = 0;

        for (int ptr = 0; ptr < int(q.size()); ptr++)
        {
            const int cur = q[ptr];

            for (int c = 0; c < 3; c++)
            {
                int &dest = trans[cur][c];
                if (dest == -1)
                    dest = (cur == 0 ? 0 : trans[link[cur]][c]);

                if (link[dest] == -1)
                {
                    q.push_back(dest);
                    link[dest] = (cur == 0 ? 0 : trans[link[cur]][c]);
                }
            }
        }
    }
};

int simulate (const string &s)
{
    int state = 0;
    for (char ch : s)
        state = trans[state][ch - 'a'];
    return state;
}

void trie_construction (istream &cin = std::cin, ostream &cout = std::cout) {
    trie trie;

    string s;

    while (cin >> s) {
        const int len = int(s.size());
        if (len <= trie_word_length)
            trie.add_word(s + s);
    }

    trie.build();
}

void dp_calculation (istream &cin = std::cin, ostream &cout = std::cout)
{
    const int states = max_states;

    const int dp_memoization_length = max(0, correct_answer_length - 2 * (trie_word_length + 1));

    vector<vector<ll>> dp(dp_memoization_length + 1, vector<ll>(states));
    for (int i = 0; i < states; i++)
        dp[0][i] = (bad[i] ? 0 : 1);

    vector<ll> cur_layer = dp[0], next_layer(states);
    vector<ll> answer(correct_answer_length + 1);

    for (int iter = 0; iter <= correct_answer_length; iter++) {
        if (iter <= dp_memoization_length)
            copy(cur_layer.begin(), cur_layer.end(), dp[iter].begin());

        answer[iter] = cur_layer[0];
        if (iter == correct_answer_length)
            break;

        fill(next_layer.begin(), next_layer.end(), 0);

        for (int i = 0; i < states; i++) {
            if (bad[i])
                continue;

            for (int j = 0; j < 3; j++) {
                const int to = trans[i][j];
                next_layer[i] += cur_layer[to];
            }
        }

        swap(next_layer, cur_layer);
    }

    string s;

    while (cin >> s)
    {
        const int s_length = int(s.size());
        if (s_length <= trie_word_length || s_length > tandem_word_length)
            continue;

        const int finish_state = simulate(s);
        reverse(s.begin(), s.end());
        const int start_state = simulate(s);
        reverse(s.begin(), s.end());
        assert(!bad[finish_state] && !bad[start_state]);

        for (int length = 2 * int(s.size()); length <= correct_answer_length; length++) {
            for (int start = 0; start + 2 * int(s.size()) <= length; start++) {
                const int forbidden = s.back() - 'a';
                ll left_multiplier = (start == 0 ? 1 : 0);
                if (start > 0)
                {
                    for (int previous = 0; previous < 3; previous++)
                    {
                        if (previous != forbidden)
                            left_multiplier += dp[start - 1][trans[start_state][previous]];
                    }
                }

                const ll right_multiplier = dp[length - start - 2 * int(s.size())][finish_state];
                answer[length] -= left_multiplier * right_multiplier;
            }
        }
    }

    for (int i = 0; i <= correct_answer_length; i++)
        cout << i << ' ' << answer[i] << endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    for (int iter = 0; iter < 2; iter++)
    {
        ifstream fin("../dictionary.txt");
        ofstream fout("../answers_for_lengths.txt");
        (iter == 0 ? trie_construction : dp_calculation)(fin, fout);
    }

    cout << fixed;
    cout << double(clock()) / CLOCKS_PER_SEC << endl;

    return 0;
}

