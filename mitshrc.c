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
			// エスケープシーケンスの処理
			for(int i=0; i<256; i++) {
				if(buf[1][i] == '\\') {
					if(i+1 < 256 && buf[1][i+1] != '\0') {
						switch(buf[1][i+1]) {
						case 'n':
							buf[1][i] = '\n';
							for(int j=i+1; j<256-1; j++) {
								// \の後の文字を削除 例)\\n⇒\\;
								buf[1][j] = buf[1][j+1];
							}
							break;
						default :
							buf[1][i] = '\0';
							break;
						};
					}
				}
				if(buf[1][i] == '\0') break;
			}
			strcpy(*looks, buf[1]);
		}
	}
	fclose(fp);
	return 1;
}
