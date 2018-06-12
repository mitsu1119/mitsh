#include <stdio.h>
#include <stdlib.h>
#include "MITshell.h"

int main(int argc, char **argv) {
	char *looks;
	looks = (char *)malloc(256*sizeof(char));
	LoadSetting("setting.mitshrc", &looks);	

	main_loop(looks);
	free(looks);
	return EXIT_SUCCESS;
}
