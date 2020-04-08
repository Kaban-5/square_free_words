constexpr int dictionary_word_length = 60;

constexpr int correct_answer_length = 121;
constexpr int trie_word_length = 40;
constexpr int tandem_word_length = 60;

static_assert(correct_answer_length <= 3 * trie_word_length + 1, "trie strings too small");
static_assert(correct_answer_length <= 2 * tandem_word_length + 1, "medium strings too small");

#include "gen_strings.cpp"
//#include "find_total_length.cpp"
//#include "calculate_the_answer.cpp"
