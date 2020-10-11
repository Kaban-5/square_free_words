bool avoids_bad_states(const string &s, int state, const trie &words_to_avoid)
{
	for (const char ch : s)
	{
		state = words_to_avoid.transitions[state][ch - 'a'];
		if (words_to_avoid.is_bad_state[state])
			return false;
	}
	return true;
}

void generate_and_output(string &s, ostream &out, int64_t &cnt, const trie &words_to_avoid, const int state,
                         const int dictionary_word_length)
{
	if (words_to_avoid.is_bad_state[state])
		return;
	if (!s.empty() && avoids_bad_states(s, state, words_to_avoid))
	{
		out << s << "\n";
		cnt++;
	}
	if (int(s.size()) == dictionary_word_length)
		return;

	for (char ch = 'a'; ch <= 'c'; ch++)
	{
		s += ch;
		generate_and_output(s, out, cnt, words_to_avoid, words_to_avoid.transitions[state][ch - 'a'],
		                    dictionary_word_length);
		s.pop_back();
	}
}

int64_t generate_dictionary(const int dictionary_word_length, ostream &out)
{
	out << dictionary_word_length << endl;
	if (dictionary_word_length == 0)
		return 0;

	stringstream halfs_of_smaller_minimal_squares;
	const int64_t auxiliry_cnt = generate_dictionary(dictionary_word_length / 2,
	                                                 halfs_of_smaller_minimal_squares);
	cerr << "Generated auxiliary dictionary with lengths up to " << dictionary_word_length / 2 << endl;
	cerr << auxiliry_cnt << " strings in auxiliary dictionary." << endl;

	string s;
	trie words_to_avoid;
	int64_t cnt = 0;

	ignore_int(halfs_of_smaller_minimal_squares);
	while (halfs_of_smaller_minimal_squares >> s)
	{
		words_to_avoid.add_word(s + s);
		out << s << "\n";
		cnt++;
	}
	words_to_avoid.build();

	string temp;
	generate_and_output(temp, out, cnt, words_to_avoid, 0, dictionary_word_length);
	return cnt;
}
