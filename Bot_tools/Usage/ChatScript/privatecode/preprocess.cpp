#include "cppjieba/Jieba.hpp"

const char* const DICT_PATH = "./privatecode/Jieba/DICT/jieba.dict.utf8";
const char* const HMM_PATH = "./privatecode/Jieba/DICT/hmm_model.utf8";
const char* const USER_DICT_PATH = "./privatecode/Jieba/DICT/user.dict.utf8";
const char* const IDF_PATH = "./privatecode/Jieba/DICT/idf.utf8";
const char* const STOP_WORD_PATH = "./privatecode/Jieba/DICT/stop_words.utf8";

cppjieba::Jieba cpp_jieba(DICT_PATH,
	HMM_PATH,
	USER_DICT_PATH,
	IDF_PATH,
	STOP_WORD_PATH);

char * CNPreprocess(char * incoming)
{
	char * segmented_result;
	if (strlen(incoming) == 0 || !strncmp(incoming, " :", 2) || !strncmp(incoming, ":", 1))
		segmented_result = incoming;
	else
	{
		vector<string> words;
		vector<cppjieba::Word> jiebawords;
		string s(incoming);
		string result;

		cpp_jieba.Cut(s, words, true);
		result = limonp::Join(words.begin(), words.end(), " ");
		char *pw = new char(strlen(incoming) + 1);

		// Method #2: Allocate memory on stack and copy the contents of the
		// original string. Keep in mind that once a current function returns,
		// the memory is invalidated.
		segmented_result = (char *)alloca(result.size() + 1);
		memcpy(segmented_result, result.c_str(), result.size() + 1);
	}
	return segmented_result;
}
