/*
privatesrc.cpp: code you want to add to functionexecute.cpp (your own cs engine functions) classic definitions compatible with invocation from script look like this:

static FunctionResult Yourfunction(char* buffer)

where ARGUMENT(1) is a first argument passed in. answers are returned as text in buffer, and success/failure codes as FunctionResult.
*/

/*
Author: Yunchao He

私有代码，为CS引擎增加更多自定义的函数

本文件中的代码相当于直接在functionExecute.cpp中执行，因此，可以直接使用那里的任何变量、函数

可能会使用的变量：

ARGUMENT(1)	传入函数的第一个参数
NOPROBLEM_BIT 函数成功执行后的返回值
FAILRULE_BIT 函数执行出错的返回值

可能会使用的函数：
sprintf(buffer,(char*)"%d",count);	将格式化的数据写入字符串

代码注意：

1. 常用无符号整形	unsigned int i;
*/
#include "cppjieba/Jieba.hpp"

const char* const DICT_PATH = "./JIEBA_DICT/jieba.dict.utf8";
const char* const HMM_PATH = "./JIEBA_DICT/hmm_model.utf8";
const char* const USER_DICT_PATH = "./JIEBA_DICT/user.dict.utf8";
const char* const IDF_PATH = "./JIEBA_DICT/idf.utf8";
const char* const STOP_WORD_PATH = "./JIEBA_DICT/stop_words.utf8";

cppjieba::Jieba jieba(DICT_PATH,
	HMM_PATH,
	USER_DICT_PATH,
	IDF_PATH,
	STOP_WORD_PATH);

// 中文分词
static FunctionResult CNSegmentCode(char* buffer)
{
	vector<string> words;
	vector<cppjieba::Word> jiebawords;
	string s;
	string result;

	s = (string)ARGUMENT(1);
	jieba.Cut(s, words, true);
	result = limonp::Join(words.begin(), words.end(), " ");
	sprintf(buffer, result.c_str());
	return NOPROBLEM_BIT; //FAILRULE_BIT if error
}

/*
// 中文分词 for tesing
static void C_CNSegment(char* input)
{
	input = SkipWhitespace(input);
	char word[MAX_WORD_SIZE];
	*word == '#';
	printf("分词之后的结果：为");
	printf(input);
	printf("\r\n\r\n");
}
*/

void PrivateInit(char* params) {}  // called on startup of CS, passed param : private =
void PrivateRestart() {} // called when CS is restarting
void PrivateShutdown() {}  // called when CS is exiting.