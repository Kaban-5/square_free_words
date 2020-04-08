#include <bits/stdc++.h>
using namespace std;

bool has_tandems_smaller_than (const vector<int> &v, int what)
{
    for (int len = 1; len < what; len++)
    {
        for (int i = 0; i + 2 * len <= int(v.size()); i++)
        {
            if (equal(v.begin() + i, v.begin() + i + len, v.begin() + i + len))
                return true;
        }
    }

    return false;
}

struct dsu
{
    vector<int> par;

    dsu (int n)
    {
        par.resize(n);
        iota(par.begin(), par.end(), 0);
    }

    int group (int x)
    {
        if (x == par[x])
            return x;
        return par[x] = group(par[x]);
    }

    void merge (int x, int y)
    {
        par[group(x)] = group(y);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    for (int len = 2; len <= 30; len++)
    {
        for (int target_len = 2 * len + 2; ; target_len++)
        {
            bool found_loss = false;
            {
                for (int second_len = len + 1; !found_loss; second_len++)
                {
                    const int start = target_len - 2 * second_len;
                    if (start < 0)
                        break;

                    dsu dsu(target_len);

                    for (int i = 0; i < len; i++)
                        dsu.merge(i, i + len);
                    for (int i = 1; i <= second_len; i++)
                        dsu.merge(target_len - i - second_len, target_len - i);

                    vector<int> v(target_len);
                    for (int i = 0; i < target_len; i++)
                        v[i] = dsu.group(i);

                    if (!has_tandems_smaller_than(v, len))
                        found_loss = true;
                }
            }

            if (found_loss) {
                cerr << len << " -> " << target_len << endl;
                break;
            }
        }
    }

    return 0;
}
