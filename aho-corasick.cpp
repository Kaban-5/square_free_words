/**
 * Aho-Corasick automaton.
 *
 * Warning! Don't use is_bad_state, transitions and simulate() externally before calling build().
 * Don't call add_word() after build() either. Don't call build() twice. The automaton will be in
 * the correct state only after you add all strings and call build() once.
 */
struct trie
{
	const int32_t final_number_of_states;
	vector<bool> is_bad_state;
	vector<array<int32_t, 3>> transitions;

	bool known_number_of_states() const
	{
		return final_number_of_states != -1;
	}
	int node_count() const {
		return int(transitions.size());
	}

	void create_node()
	{
		if (known_number_of_states() && int(transitions.size()) >= final_number_of_states)
			throw std::runtime_error("Attempted to create too many states.");

		is_bad_state.push_back(false);
		transitions.push_back(array<int32_t, 3>{-1, -1, -1});
	}

	explicit trie() : final_number_of_states(-1)
	{
		create_node();
	}

	explicit trie(const int32_t state_count) : final_number_of_states(state_count)
	{
		is_bad_state.reserve(final_number_of_states);
		transitions.reserve(final_number_of_states);
		create_node();
	}

	void add_word(const string &s)
	{
		int32_t v = 0;
		for (char ch : s)
		{
			if (transitions[v][ch - 'a'] == -1)
			{
				transitions[v][ch - 'a'] = node_count();
				create_node();
			}

			v = transitions[v][ch - 'a'];
		}

		is_bad_state[v] = true;
	}

	void build()
	{
		if (known_number_of_states() && node_count() != final_number_of_states)
			throw std::runtime_error("Number of states was computed incorrectly.");

		vector<int32_t> link(node_count(), -1);

		vector<int32_t> q;
		q.reserve(node_count());
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
