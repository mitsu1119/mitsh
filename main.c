#include <stdio.h>
#include <stdlib.h>
#include "MITshell.h"

int main(int argc, char **argv) {
	char *looks;
	LoadSetting("setting.mitshrc", &looks);	

	main_loop(looks);
	return EXIT_SUCCESS;
}
