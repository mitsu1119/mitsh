#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/*
 * read_line
 * 入力を一行まるまる読み込み
 * 文字列を返す関数
 */
char *read_line() {
	char *line = NULL;
	size_t size=0;
	getline(&line, &size, stdin);
	
	return line;
}

/*
 * lplit_line
 * 行を読み込んで" "で文字列を区切る
 */
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

/*
 * launch
 * args から受け取ったコマンドで子プロセスを作成
 */
int launch(char **args) {
	pid_t pid;
	int status;
	
	pid = fork();

	if(pid == 0) {		// 子プロセス
		// コマンドの実行
		if(execvp(args[0], args) == -1) {
			perror("error");
		}
		exit(EXIT_FAILURE);
	} else if(pid < 0) {	// fork のエラー
		perror("error");
	} else {		// 親プロセス
		do {
			// 子プロセス終了待機
			waitpid(pid, &status, WUNTRACED);
		} while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	
	return 1;
}

/*
 * 組み込みのシェルコマンドの宣言
 */
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

/*
 * execute
 * args で指定されたコマンドを実行する関数
 */
int execute(char **args) {
	// コマンドが指定されていない
	if(args[0] == NULL) return 1;
	
	for(int i=0; i<builtin_command_size(); i++) {
		if(strcmp(args[0], builtin_command[i]) == 0) {
			// args[0] の組み込みコマンドで実行
			return (*builtin_func[i])(args);
		}
	}
	
	// 組み込みコマンドでないときは launch でシステムコール
	return launch(args);
}

/*
 * main_loop
 * メインとなるループ
 */

void main_loop() {
	char *line;
	char **args;
	int status;
	
	do {
		printf("> ");
		line = read_line();
		args = split_line(line);
		status = execute(args);
		
		free(args);
	} while(status);
}

/*
 * main
 * メイン関数
 */
int main(int argc, char **argv) {
	
	main_loop();

	return EXIT_SUCCESS;
}
