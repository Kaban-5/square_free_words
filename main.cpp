#ifdef LOCAL
//#define _GLIBCXX_DEBUG
#endif

/// Warning: little-to-nothing is guaranteed in the case the code crashes or is forcefully terminated.
/// Of course, it won't break your system, but the workflow of the code is pretty delicate.
/// I will try to make the problem less extreme.
/// Manually modifying .txt files is not recommended (but you can do this if you know what are you doing).
/// It is recommended to have a backup of dictionary file in case that something goes awry.
/// Generating the dictionary takes most of the program runtime.
/// Calculating the answers uses the most memory among all three blocks.

#include <bits/stdc++.h>
using namespace std;

constexpr int32_t correct_answer_length = 121;
static_assert(5 <= correct_answer_length, "For 5 the code finishes instantly anyway.");

constexpr int32_t tandem_word_length = correct_answer_length / 2;
constexpr int32_t trie_word_length = (correct_answer_length + 1) / 3;
constexpr int32_t necessary_dictionary_word_length = tandem_word_length;

static_assert(correct_answer_length <= 3 * trie_word_length + 1, "Trie strings are too short.");
static_assert(correct_answer_length <= 2 * tandem_word_length + 1, "Medium strings are too short.");
static_assert(trie_word_length <= tandem_word_length, "Trie strings are longer than medium strings.");
static_assert(tandem_word_length <= necessary_dictionary_word_length, "Possible tandems are not in the dictionary.");

const string dictionary_address = "../dictionary.txt";
const string word_length_address = "../real_dictionary_word_length.txt";
const string state_count_address = "../state_count.txt";
const string answers_address = "../answers_for_lengths.txt";

#include "gen_strings.cpp"
#include "find_total_length.cpp"
#include "calculate_the_answer.cpp"

bool dictionary_has_large_strings ()
{
	ifstream in(word_length_address);
	int32_t real_dictionary_word_length;
	if (in >> real_dictionary_word_length)
		return real_dictionary_word_length >= necessary_dictionary_word_length;
	return false;
}

void timestamp (const string &message)
{
	cerr << fixed << setprecision(3);
	cerr << message << "; time = " << double(clock()) / CLOCKS_PER_SEC << " seconds." << endl;
}

int main()
{
	if (!dictionary_has_large_strings())
	{
		cerr << "Generating the dictionary..." << endl;
		{
			ofstream out(dictionary_address);
			int64_t number_of_strings = 0;
			generate_and_output(string(""), out, number_of_strings);
			cerr << number_of_strings << " strings in the dictionary" << endl;
		}
		{
			ofstream out(word_length_address);
			out << necessary_dictionary_word_length << endl;
		}
		timestamp("Succesfully generated the dictionary");

		/// Find the number of states in the trie.
		cerr << "Computing the number of states in the trie..." << endl;
		print_the_number_of_states();
		timestamp("Succesfully computed the number of states in the trie");
	}

	cerr << "Computing the answers..." << endl;
	trie_construction_and_dp_calculation();
	cerr << "Succesfully computed the answers." << endl;
	timestamp("Terminated");
}