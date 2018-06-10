/* Builtin.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// プロトタイプ宣言
int mit_cd(char **args);
int mit_help(char **args);
int mit_exit(char **args);

// 組み込みコマンドのリスト
char *builtin_command[] = {
	"cd",
	"help",
	"exit"
};

// 組み込みコマンドに対応する関数のリスト
int (*builtin_func[])(char**) = {
	&mit_cd,
	&mit_help,
	&mit_exit
};

// 組み込みコマンドの数
int builtin_command_size() {
	return sizeof(builtin_command)/sizeof(char*);
}

/*
 * 組み込みコマンドの処理
 */
// cd コマンド
int mit_cd(char **args) {
	if(args[1] == NULL) {
		// 移動するディレクトリが指定されていないとき
		fprintf(stderr, "expected argument to \"cd\"\n");
	} else {
		if(chdir(args[1]) != 0) {
			// 移動できなかったときエラー
			perror("error");
		}
	}
	return 1;
}
// help コマンド
int mit_help(char **args) {
	printf("MITshell\n");
	printf("Built in:\n");
	for(int i=0; i<builtin_command_size(); i++) {
		printf(" %s\n", builtin_command[i]);
	}
	
	printf("Use the man command for information other programs\n\n");
	return 1;
}
// exit コマンド
int mit_exit(char **args) {
	return 0;
}
