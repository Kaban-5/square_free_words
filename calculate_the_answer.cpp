void dp_calculation(const trie &trie)
{
	const int32_t states = trie.final_number_of_states;
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

		const int32_t finish_state = trie.simulate<1>(word);
		reverse(word.begin(), word.end());
		const int32_t start_state = trie.simulate<1>(word);
		reverse(word.begin(), word.end());
		if (bad[finish_state] || bad[start_state])
			throw std::runtime_error("The word is not good, but somehow is in the dictionary.");

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

				const int64_t right_multiplier = dp[length - start - 2 * int32_t(word.size())][finish_state];
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
