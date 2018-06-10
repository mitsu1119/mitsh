/* execute.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// extern 宣言
extern char *builtin_command[];
extern int (*builtin_func[])(char**);

extern int builtin_command_size();

// 受け取ったコマンドで子プロセスの作成
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

// 読み込んだコマンドを適切に実行
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
