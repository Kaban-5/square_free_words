/**
 * This code verifies the main Lemma for strings of length at most 200.
 */

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

bool has_tandems_smaller_than(const vector<int32_t> &v, const int32_t what)
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

/**
 * Basic disjoint set union data structure.
 * Asymptotic complexity is suboptimal (rank heuristic is not used), but good enough.
 */
struct dsu
{
	vector<int32_t> par;

	dsu(const int32_t n)
	{
		par.resize(n);
		iota(par.begin(), par.end(), 0);
	}

	int32_t group(const int32_t x)
	{
		if (x == par[x])
			return x;
		return par[x] = group(par[x]);
	}

	void merge(const int32_t x, const int32_t y)
	{
		par[group(x)] = group(y);
	}
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	/**
	 * For each len from 2 to 70, we find the smallest target_len, such that a string of length target_len
	 * can have either two square substrings of different lengths, with the shorter being of length 2*len.
	 * If target_len is always not less than 3 * len + 2, then main lemma holds for all lengths from 1 to
	 * target_len. Hence, this is a proof of main lemma holding up to lengths 3 * 70 + 2 = 212.
	*/
	for (int32_t len = 2; len <= 70; len++)
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
				if (target_len < 3 * len + 2)
					throw runtime_error("Main lemma is incorrect.");
				cerr << len << " -> " << target_len << endl;
				break;
			}
		}
	}

	return 0;
}
