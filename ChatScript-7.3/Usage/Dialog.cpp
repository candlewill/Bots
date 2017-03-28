// https://www.chatbots.org/ai_zone/viewthread/1552/P15/

main(int argc, char** argv)
{
	int i;

	printf("argc = %d\n", argc);

	for (i = 0; i < argc; i++)
		printf("argv[%d] = \"%s\"\n", i, argv);
}