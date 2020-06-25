struct trie
{
	int32_t free_node, max_states;
	vector<bool> is_bad_state;
	vector<array<int32_t, 3>> transitions;

	void create_node()
	{
		if (free_node >= max_states)
			throw std::runtime_error("Attempted to create too many states.");

		for (int32_t i = 0; i < 3; i++)
			transitions[free_node][i] = -1;
		free_node++;
	}

	trie(const int max_states_parameter)
	{
		max_states = max_states_parameter;
		is_bad_state.assign(max_states, false);
		transitions.resize(max_states);

		free_node = 0;
		create_node();
	}

	void add_word(const string &s)
	{
		int32_t v = 0;
		for (char ch : s)
		{
			if (transitions[v][ch - 'a'] == -1)
			{
				transitions[v][ch - 'a'] = free_node;
				create_node();
			}

			v = transitions[v][ch - 'a'];
		}

		is_bad_state[v] = true;
	}

	void build()
	{
		if (free_node != max_states)
			throw std::runtime_error("Number of states was computed incorrectly.");

		vector<int32_t> link(max_states, -1);

		vector<int32_t> q;
		q.reserve(max_states);
		q.push_back(0);
		link[0] = 0;

		for (int32_t ptr = 0; ptr < int32_t(q.size()); ptr++)
		{
			const int32_t cur = q[ptr];

			for (int32_t c = 0; c < 3; c++)
			{
				int &dest = transitions[cur][c];
				if (dest == -1)
					dest = (cur == 0 ? 0 : transitions[link[cur]][c]);

				if (link[dest] == -1)
				{
					q.push_back(dest);
					link[dest] = (cur == 0 ? 0 : transitions[link[cur]][c]);
				}
			}
		}
	}

	int32_t simulate(const string &s) const
	{
		int32_t state = 0;
		for (char ch : s)
			state = transitions[state][ch - 'a'];
		return state;
	}
};

void dp_calculation(const trie &trie)
{
	const int32_t states = trie.max_states;
	const auto &bad = trie.is_bad_state;
	const auto &transitions = trie.transitions;

	const int32_t dp_memoization_length = max(0, correct_answer_length - 2 * (trie_word_length + 1));

	vector<vector<int64_t>> dp(dp_memoization_length + 1, vector<int64_t>(states));
	for (int32_t i = 0; i < states; i++)
		dp[0][i] = (bad[i] ? 0 : 1);

	vector<int64_t> cur_layer = dp[0], next_layer(states);
	vector<int64_t> answer(correct_answer_length + 1);

	for (int32_t iter = 0; iter <= correct_answer_length; iter++)
	{
		if (iter <= dp_memoization_length)
			copy(cur_layer.begin(), cur_layer.end(), dp[iter].begin());

		answer[iter] = cur_layer[0];
		if (iter == correct_answer_length)
			break;

		fill(next_layer.begin(), next_layer.end(), int64_t(0));

		for (int32_t i = 0; i < states; i++)
		{
			if (bad[i])
				continue;

			for (int j = 0; j < 3; j++)
			{
				const int32_t to = transitions[i][j];
				next_layer[i] += cur_layer[to];
			}
		}

		swap(next_layer, cur_layer);
	}

	string word;
	ifstream in(dictionary_address);

	while (in >> word)
	{
		const int s_length = int32_t(word.size());
		if (s_length <= trie_word_length || s_length > tandem_word_length)
			continue;

		const int32_t finish_state = trie.simulate(word);
		reverse(word.begin(), word.end());
		const int32_t start_state = trie.simulate(word);
		reverse(word.begin(), word.end());
		assert(!bad[finish_state] && !bad[start_state]);

		for (int32_t length = 2 * int32_t(word.size()); length <= correct_answer_length; length++)
		{
			for (int32_t start = 0; start + 2 * int32_t(word.size()) <= length; start++)
			{
				const int32_t forbidden = word.back() - 'a';
				int64_t left_multiplier = (start == 0 ? 1 : 0);
				if (start > 0)
				{
					for (int32_t previous = 0; previous < 3; previous++)
					{
						if (previous != forbidden)
							left_multiplier += dp[start - 1][transitions[start_state][previous]];
					}
				}

				const int32_t right_multiplier = dp[length - start - 2 * int32_t(word.size())][finish_state];
				answer[length] -= left_multiplier * right_multiplier;
			}
		}
	}

	ofstream out(answers_address);
	for (int32_t i = 0; i <= correct_answer_length; i++)
		out << i << ' ' << answer[i] << endl;
}

void trie_construction_and_dp_calculation()
{
	int32_t max_states;
	{
		ifstream in(state_count_address);
		if (!(in >> max_states))
			throw std::runtime_error("Could not read the number of states.");
		cerr << max_states << " states in the trie." << endl;
	}

	trie trie(max_states);

	string word;
	ifstream in(dictionary_address);

	while (in >> word)
	{
		const int32_t len = int32_t(word.size());
		if (len <= trie_word_length)
			trie.add_word(word + word);
	}
	trie.build();

	dp_calculation(trie);
}
