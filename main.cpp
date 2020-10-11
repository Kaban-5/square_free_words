/**
 * Read README.md for general tutorial on using this code.
 *
 * Warning: little-to-nothing is guaranteed in the case the code crashes or is forcefully terminated.
 * Of course, it won't break your system, but the workflow of the code is pretty delicate.
 * Manually modifying dictionary.txt is not recommended. Backing it up verbatim is encouraged, though.
 *
 * Warning: dictionary.txt takes several GBs of your disk space.
 * For example, for correct_answer_length = 131, the size of dictionary.txt is approximately 7.6 GB.
 * Later, I will explain a possible way to deal with this problem (not implemented here, though).
 *
 *
 *
 *
 * Warning! Warning! Warning! Very important.
 *
 * The last part of this program (trie_construction_and_dp_calculation()) uses a lot of memory.
 * Running out of RAM is a quite likely event. Depending on your system setup, that would lead
 * to some processes (possibly, this very one) being killed or swapping (honesly, I am not an OS
 * expert, so my understanding of this could be wrong). The latter probably will be even worse
 * than former, because, at best, this program will slow down to a crawl, and, at worst, your
 * whole system will slow down to a crawl and will become sluggish and unresponsive.
 * I "memnuked" my laptop that way several times while running this code.
 *
 * As a corollary of the above, it is highly recommended to close all applications with high
 * memory consumption before running this code: browsers, IDEs, e.t.c. Also, I recommend to
 * keep eye on the memory consumption of the program and kill it before it consumes all
 * available RAM. Running in a safe setup, where the amount of RAM avaiable to the program
 * is limited is a very good idea. If in doubt, start with small correct_answer_length (say, 101),
 * and work up from there, keeping in mind that the memory usage grows exponentially with
 * correct_answer_length.
 *
 * There are several ways to decrease RAM consumption at the cost of slower and/or more
 * complicated code. I will discuss them later (they are not implemented here, though).
 */
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * Up to where we want to compute the answers. Modify this constant if you want to compute
 * more or less answers. Don't change the definition of other "constexpr int32_t" constants,
 * unless you have a good reason to.
 */
constexpr int32_t correct_answer_length = 131;
static_assert(5 <= correct_answer_length, "For 5 the code finishes instantly anyway.");

/// How large can be squares in string of length <= correct_answer_length?
constexpr int32_t tandem_word_length = correct_answer_length / 2;
/// All minimal squares with length <= trie_word_length will be in the trie.
constexpr int32_t trie_word_length = (correct_answer_length + 1) / 3;
/// Minimal required length of a word in the dictionary.
constexpr int32_t necessary_dictionary_word_length = tandem_word_length;

static_assert(correct_answer_length <= 3 * trie_word_length + 1, "Trie strings are too short.");
static_assert(correct_answer_length <= 2 * tandem_word_length + 1, "Medium strings are too short.");
static_assert(trie_word_length <= tandem_word_length, "Trie strings are longer than medium strings.");
static_assert(tandem_word_length <= necessary_dictionary_word_length, "Possible tandems are not in the dictionary.");

/**
 * Change to where you want dictionary and answers to be. I use "../filename" and not "filename",
 * because CLion, by default, stores executables in "project/clang-build-debug" and
 * "project/clang-build-release", not just "project".
 */
const string dictionary_address = "../dictionary.txt";
const string answers_address = "../answers_for_lengths.txt";


/**
 * The dictionary is either an empty file, or a number len in a single line, followed by
 * multiple lines, each being a such string w of length <= len, that ww is a minimal squares.
 * Moreover, all such minimal squares with half-length from 1 to len are present in the
 * dictionary. Right now, the dictionary is not compressed in any way, hence it is very large.
 *
 * There are two obvious ways to compress the dictionary:
 * 1) Use ASCII symbols to encode several consecutive letters at the same time (there are
 * only three letters used, but more than 90 printable ASCII characters). Moreover, after the
 * first letter, there is only two possibilities for each letter in a word from the dictionary.
 * 2) Because the dictionary is sorted alphabetically, consecutive strings often have large LCP.
 * (longest common prefix). Encode a string as [the length of its LCP with previous string
 * (one byte is enough)] and [the remaining suffix of the string].
 *
 * I think (I did not actually implement them) that, with both above optimizations, the dictionary
 * file size should be smaller than the RAM usage of the last part of the program, meaning that
 * we can actually store the dictionary in the RAM with those optimizations.
 *
 * More complicated encoding schemes can be used, but we need to be able to encode and decode
 * strings "on the fly", otherwise there is no point in encoding them at all.
 */

/**
 * Consumes and ignores an integer from a stream. Recall that dictionary starts with the maximal
 * length of words in it. Often, we don't need this information and want to proceed reading the
 * actual dictionary itself.
 */
void ignore_int (istream &in)
{
	int32_t tmp;
	if (!(in >> tmp))
		throw std::runtime_error("Expected real dictionary word length at the start of dictionary file.");
}

/**
 * Keep these files in the same directory with main.cpp.
 */
#include "aho-corasick.cpp"
#include "faster_gen_strings.cpp"
#include "find_total_length.cpp"
#include "faster_calculate_the_answer.cpp"

bool dictionary_has_large_strings()
{
	ifstream in(dictionary_address); /// if there is no dictionary, an empty one is created
	int32_t real_dictionary_word_length;
	if (in >> real_dictionary_word_length) /// works correctly for an empty dictionary
		return real_dictionary_word_length >= necessary_dictionary_word_length;
	return false;
}

void timestamp(const string &message)
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
			const int64_t number_of_strings = generate_dictionary(necessary_dictionary_word_length, out);
			cerr << number_of_strings << " strings in the dictionary" << endl;
		}
		timestamp("Successfully generated the dictionary");
	}

	int32_t max_states = -1;
	{
		/// Find the number of states in the trie.
		cerr << "Computing the number of states in the trie..." << endl;
		max_states = print_the_number_of_states();
		timestamp("Successfully computed the number of states in the trie");
	}
	if (max_states == -1)
		throw std::runtime_error("Did not find the number of states somehow.");

	cerr << "Computing the answers..." << endl;
	trie_construction_and_dp_calculation(max_states);
	cerr << "Successfully computed the answers." << endl;
	timestamp("Terminated");
}