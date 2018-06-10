#include <stdio.h>
#include <string.h>

/*
 * 設定ファイルの読み込み
 * path : 設定ファイルのパス
 * looks : 設定ファイルから読み込んだ Looks のデータが格納される変数
 */
int LoadSetting(char *path, char **looks) {
	FILE *fp;
	char buf[2][256];
	fp = fopen(path, "r");
	
	while(fgets(buf[0], 256, fp)) {
		if(buf[0][0] == '#') continue;	// コメント文
		if(strncmp(buf[0], "Looks=", 6) == 0) {	// looks の設定文
			sscanf(buf[0], "Looks='%[^']'", buf[1]);
			strcpy(*looks, buf[1]);
		}
	}
	fclose(fp);
	return 1;
}
