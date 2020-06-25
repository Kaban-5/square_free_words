#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

bool has_tandems_smaller_than(const vector<int32_t> &v, int32_t what)
{
	for (int32_t len = 1; len < what; len++)
	{
		for (int32_t i = 0; i + 2 * len <= int32_t(v.size()); i++)
		{
			if (equal(v.begin() + i, v.begin() + i + len, v.begin() + i + len))
				return true;
		}
	}

	return false;
}

struct dsu
{
	vector<int32_t> par;

	dsu(int32_t n)
	{
		par.resize(n);
		iota(par.begin(), par.end(), 0);
	}

	int32_t group(int32_t x)
	{
		if (x == par[x])
			return x;
		return par[x] = group(par[x]);
	}

	void merge(int32_t x, int32_t y)
	{
		par[group(x)] = group(y);
	}
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	for (int32_t len = 2; len <= 30; len++)
	{
		for (int32_t target_len = 2 * len + 2;; target_len++)
		{
			bool found_loss = false;
			{
				for (int32_t second_len = len + 1; !found_loss; second_len++)
				{
					const int32_t start = target_len - 2 * second_len;
					if (start < 0)
						break;

					dsu dsu(target_len);

					for (int32_t i = 0; i < len; i++)
						dsu.merge(i, i + len);
					for (int32_t i = 1; i <= second_len; i++)
						dsu.merge(target_len - i - second_len, target_len - i);

					vector<int32_t> v(target_len);
					for (int32_t i = 0; i < target_len; i++)
						v[i] = dsu.group(i);

					if (!has_tandems_smaller_than(v, len))
						found_loss = true;
				}
			}

			if (found_loss)
			{
				cerr << len << " -> " << target_len << endl;
				break;
			}
		}
	}

	return 0;
}
