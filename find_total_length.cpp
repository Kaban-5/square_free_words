void calc_number_of_states(int32_t l, int32_t r, int32_t depth, const vector<string> &dictionary, int &states_needed)
{
	if (l == r)
		return;

	states_needed++;
	if (int32_t(dictionary[l].size()) == depth)
		l++;

	if (l == r)
		return;

	array<int32_t, 4> where = {l, r, r, r};

	int32_t ptr = 1;
	for (int32_t i = l; i < r; i++)
	{
		while (dictionary[i][depth] > 'a' + (ptr - 1))
			where[ptr++] = i;
	}

	for (int32_t i = 0; i < 3; i++)
		calc_number_of_states(where[i], where[i + 1], depth + 1, dictionary, states_needed);
}

void print_the_number_of_states()
{
	ifstream in(dictionary_address);
	ofstream out(state_count_address);

	vector<string> dictionary;
	string word;
	while (in >> word)
	{
		if (int(word.size()) <= trie_word_length)
			dictionary.push_back(word + word);
	}

	int32_t states_needed = 0;
	sort(dictionary.begin(), dictionary.end());
	calc_number_of_states(0, int(dictionary.size()), 0, dictionary, states_needed);

	cerr << states_needed << " states in the trie." << endl;
	out << states_needed << endl;
}
