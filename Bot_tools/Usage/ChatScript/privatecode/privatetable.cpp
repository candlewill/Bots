/*
privatetable.cpp: listing of the functions made visible to CS table entries to connect your functions to script:

{ (char*) ^YourFunction, YourFunction, 1,0, (char*) help text of your function},

1 is the number of evaluated arguments to be passed in where VARIABLE_ARGUMENT_COUNT means args evaled but you have to detect the end and ARGUMENT(n) will be ?

Another possible value is STREAM_ARG which means raw text sent. You have to break it apart and do whatever.
*/

{ (char*) "^cn_segment", CNSegmentCode, 1, 0, (char*) "Chinese Text Segmentation using Jieba" },