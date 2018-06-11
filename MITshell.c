/* MITshell.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// プロトタイプ宣言
char *read_line();
char **split_line(char *line);

// extern 宣言
extern char *Mitsh_looks;

extern int execute(char **args);

// メインループ
void main_loop(char *looks) {
	char *line;
	char **args;
	int status;
	char buf[256];
	
	do {
		strcpy(buf, looks);
		for(int i=0; i<256; i++) {
			if(buf[i] == '\0') break;
			// 特殊文字の処理
			if(buf[i] == '\\' && buf[i+1] == 'n') {
				// 改行
				printf("\n");
				i++;
				continue;
			}
			printf("%c", buf[i]);
		}
		
		line = read_line();
		args = split_line(line);
		status = execute(args);
		free(args);
	} while(status);
}

// 入力を読み込んで返す
char *read_line() {
	char *line = NULL;
	size_t size=0;
	getline(&line, &size, stdin);
	
	return line;
}

// 文字列を読み込んで分割する
const int SPLIT_BUF=64;	// 区切りの最大バッファ数
const char *SPLIT_DELIM=" \t\r\n\a";	// 区切り文字
char **split_line(char *line) {
	int bufsize = SPLIT_BUF;
	int position = 0;
	char **splits = (char**)malloc(bufsize*sizeof(char*));
	char *split;
	
	if(!splits) {
		// splits のメモリ確保ができなかったとき
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}
	
	// 分割
	split = strtok(line, SPLIT_DELIM);
	while(split != NULL) {
		splits[position] = split;
		position++;
		
		// split の数が今確保しているバッファ数を上回ったとき
		if(position >= bufsize) {
			// 更にメモリを確保する
			bufsize += SPLIT_BUF;
			splits = (char**)realloc(splits, bufsize*sizeof(char*));
			
			if(!splits) {
				fprintf(stderr, "allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		
		split = strtok(NULL, SPLIT_DELIM);
	}
	splits[position] = NULL;
	return splits;
}
