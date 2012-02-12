//********************************************************************************************
/**
 * @file		VoiceText Japanese Engine API サンプル(Windows C/C++)
 * @brief		
 *	vt_jpn.dllを使用。
 *  VoiceText Japanese Engine APIのサンプルです。
 *  個人で楽しむのみに留めましょう。
 *  ライセンスは各自で準備して下さい。
 *
 * @author		@loftkun
 * @date		2012/02/12(日) 12:40:02
 */
//********************************************************************************************
#include <windows.h>
#include <stdio.h>
#include "vt_jpn.h" //VoiceText Japanese Engine API(このサンプルでは定数定義参照に使用。特に必須ではない)

//APIに設定する定数
#define DB_PATH			"C:\\hoge" //DBへのパス
#define LICENSE_PATH	"C:\\hoge" //ライセンスファイルへのパス
#define SPEAKER_ID		(1)	//話者のID(0=miyu, 1=show, 2=misaki)

//ファイルパス
#define INPUT_TEXT			"もやもやさまあず"		//音声に変換したい文字列
#define OUTPUT_TO_FILE		"OutputTextToFile.wav"	//音声ファイル合成APIで出力するファイル名
#define OUTPUT_TO_BUFFER	"OutputTextToBuffer.pcm"//音声バッファ合成APIで出力されたバッファを出力するファイル名

//グローバル変数
HMODULE	hModule = NULL; //DLLのハンドル、API関数アドレスの取り出しに使う

//プロトタイプ宣言
int load();
int unload();
int text_to_file();
int text_to_buf();
int text_play();

//********************************************************************************************
/**
 * @brief		main関数
 * @author		@loftkun
 * @date		2012/02/12(日) 12:45:13
 *
 * @param[in]		int		argc	
 * @param[in,out]	char**	argv	
 *
 * @return		int	
 */
//********************************************************************************************
int main(int argc, char *argv[])
{
	load();

	text_to_file();

	text_to_buf();

	text_play();

	unload();

	return 0;
}

//********************************************************************************************
/**
 * @brief		load関数
 *
 * vt_jpn.dllをロード
 * loadAPIをコール
 *
 * @author		@loftkun
 * @date		2012/02/12(日) 12:45:37
 *
 * @return		int	
 */
//********************************************************************************************
int load()
{
	//DLLのハンドルを取得
	hModule = LoadLibrary("vt_jpn.dll");
	if(hModule == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		exit(1);
	}
	//DLLから初期化APIアドレスを取得
	typedef short	(*pFunc_VT_LOADTTS_JPN)(HWND hWnd, int nSpeakerID, char *db_path, char *licensefile);
	pFunc_VT_LOADTTS_JPN Func_VT_LOADTTS_JPN = 
		Func_VT_LOADTTS_JPN=(pFunc_VT_LOADTTS_JPN)GetProcAddress(hModule,"VT_LOADTTS_JPN");
	if(Func_VT_LOADTTS_JPN == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		exit(1);
	}
	//初期化APIコール
	short ret = (*Func_VT_LOADTTS_JPN)(
		NULL,
		SPEAKER_ID,
		DB_PATH,
		LICENSE_PATH);
	if(ret != VT_LOADTTS_SUCCESS){
		printf("[ERR ][%s] VT_LOADTTS_JPN ret=%d\n", __FUNCTION__, ret);
		exit(1);
	}
	return 0;
}

//********************************************************************************************
/**
 * @brief		unload関数
 *
 * unloadAPIをコール
 *
 * @author		@loftkun
 * @date		2012/02/12(日) 12:45:46
 *
 * @return		int	
 */
//********************************************************************************************
int unload()
{
	if(hModule == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		exit(1);
	}
	//DLLから終了APIアドレスを取得
	typedef void (*pFunc_VT_UNLOADTTS_JPN)(int nSpeakerID);
	pFunc_VT_UNLOADTTS_JPN	Func_VT_UNLOADTTS_JPN = 
		Func_VT_UNLOADTTS_JPN=(pFunc_VT_UNLOADTTS_JPN)GetProcAddress(hModule,"VT_UNLOADTTS_JPN");
	if(Func_VT_UNLOADTTS_JPN == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		exit(1);
	}
	Func_VT_UNLOADTTS_JPN(SPEAKER_ID);//このAPIは戻り値はない
	return 0;
}

//********************************************************************************************
/**
 * @brief		text_to_file関数
 *
 * 音声データをファイルに出力する
 *
 * @author		@loftkun
 * @date		2012/02/12(日) 12:45:49
 *
 * @return		int	
 */
//********************************************************************************************
int text_to_file()
{
	if(hModule == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		exit(1);
	}
	//DLLから音声ファイル出力APIアドレスを取得
	typedef short	(*pFunc_VT_TextToFile_JPN)(int fmt, char *tts_text, char *filename, int nSpeakerID, int pitch, int speed, int volume, int pause, int dictidx, int texttype);
	pFunc_VT_TextToFile_JPN	Func_VT_TextToFile_JPN = NULL;
    Func_VT_TextToFile_JPN=(pFunc_VT_TextToFile_JPN)GetProcAddress(hModule,"VT_TextToFile_JPN");
	if(Func_VT_TextToFile_JPN == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		unload();
		exit(0);
	}
	short ret = (*Func_VT_TextToFile_JPN)(
		VT_FILE_API_FMT_S16PCM_WAVE ,
		INPUT_TEXT,
		OUTPUT_TO_FILE,
		1,
		-1, -1, -1, -1, -1, -1);
	if(ret != 1){
		printf("[ERR ][%s] VT_TextToFile_JPN ret=%d\n", __FUNCTION__, ret);
		unload();
		exit(0);
	}

	return 0;
}

//********************************************************************************************
/**
 * @brief		text_to_buf関数
 *
 * 変換された音声データバッファを取得する
 * 本関数ではそのままバッファをファイルに出力している。
 *
 * @author		@loftkun
 * @date		2012/02/12(日) 12:45:53
 *
 * @return		int	
 */
//********************************************************************************************
int text_to_buf()
{
	if(hModule == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		exit(1);
	}

	//DLLから音声バッファ出力APIアドレスを取得
	typedef int		(*pFunc_VT_TextToBuffer_JPN)(int fmt, char *tts_text, char *output_buff, int *output_len, int flag, int nThreadID, int nSpeakerID, int pitch, int speed, int volume, int pause, int dictidx, int texttype);
	pFunc_VT_TextToBuffer_JPN	Func_VT_TextToBuffer_JPN = NULL;
	Func_VT_TextToBuffer_JPN=(pFunc_VT_TextToBuffer_JPN)GetProcAddress(hModule,"VT_TextToBuffer_JPN");
	if(Func_VT_TextToBuffer_JPN == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		unload();
		exit(0);
	}

	//frameflag=-1でコールして、PCMフレームバッファサイズを取得する
	//frameflagについてはAPI仕様書を参照(-1=サイズ取得, 0=先頭バッファ取得, 1=2番目以降バッファ取得, 2=終了指定)
	int ret = 0, frameflag = -1;
	int nFramesize=0;
	ret = Func_VT_TextToBuffer_JPN (
		VT_BUFFER_API_FMT_S16PCM,	//合成した合成音のフォーマット
		(char *)NULL,				//合成する文字列
		(char *)NULL,				//合成した合成音を保存するフレームバッファのpointer
		&nFramesize,				//フレームバッファに保存された合成音のサイズ
		frameflag,					//APIの動作を指定するflag
		0,							//スレッド使う場合に区別するためのスレッド番号
		SPEAKER_ID,					//話者ID
		-1, -1, -1, -1, -1, -1);	//pitch, speed, volume, pause, dictidx, texttype
	if(ret < 0){
		printf("[ERR ][%s] VT_TextToBuffer_JPN ret=%d\n", __FUNCTION__, ret);
		unload();
		exit(0);
	}
	printf("[INFO][%s] VT_TextToBuffer_JPN get nFramesize=%d\n", __FUNCTION__, nFramesize);

	//取得したフレームバッファサイズで、
	//音声バッファ受け取りバッファ確保
	char * framebuf = (char *) malloc (nFramesize);
	if(framebuf == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		unload();
		exit(0);
	}

	//出力ファイルOpen
	FILE * fp = NULL;
	errno_t errNo = fopen_s(&fp, OUTPUT_TO_BUFFER,"wb");
	if(errNo != 0){
		printf("[ERR ][%s]\n", __FUNCTION__);
		unload();
		exit(0);
	}

	//音声バッファをファイルに出力
	ret = 0;
	frameflag = 0;
	while(ret == 0){//戻り値が0なら継続、1(正常終了)でループを抜ける。負(異常終了)で終了。
		//音声バッファ取得
		ret = Func_VT_TextToBuffer_JPN (
			VT_BUFFER_API_FMT_S16PCM ,		//合成した合成音のフォーマット
			INPUT_TEXT,						//合成する文字列
			framebuf,						//合成した合成音を保存するフレームバッファのpointer
			&nFramesize,					//フレームバッファに保存された合成音のサイズ
			frameflag,						//APIの動作を指定するflag
			0,								//スレッド使う場合に区別するためのスレッド番号
			SPEAKER_ID,						//話者ID
			-1, -1, -1, -1, -1, -1);		//pitch, speed, volume, pause, dictidx, texttype
		printf("[INFO][%s] VT_TextToBuffer_JPN ret=%d nFramesize=%d\n", __FUNCTION__, ret, nFramesize);
		if(ret < 0){
			printf("[ERR ][%s] VT_TextToBuffer_JPN ret=%d\n", __FUNCTION__, ret);
			unload();
			fclose(fp);
			exit(0);
		}
		//frameflag更新
		if (frameflag == 0){
			frameflag = 1;//2番目以降のバッファ
		}
		//ファイルに出力
		int cnt = fwrite(framebuf,nFramesize,1,fp);
		if(cnt != 1){
			printf("[ERR ][%s] VT_TextToBuffer_JPN ret=%d\n", __FUNCTION__, ret);
			unload();
			fclose(fp);
			exit(0);
		}
	}
	fclose(fp);
	free (framebuf);
	return 0;
}

//********************************************************************************************
/**
 * @brief		ウィンドウハンドルを取得する
 *
 * MSDNから持ってきたソース。
 *
 * @author		http://support.microsoft.com/kb/124103/ja
 * @date		2012/02/12(日) 14:43:59
 *
 * @return		HWND	
 */
//********************************************************************************************
HWND GetConsoleHwnd(void)
{
	#define MY_BUFSIZE 1024 // コンソール ウィンドウのタイトル用のバッファサイズ
	HWND hwndFound;		 // 呼び出し側へ返される値
	char pszNewWindowTitle[MY_BUFSIZE];
						   // 作成されるウィンドウのタイトルが入ります
	char pszOldWindowTitle[MY_BUFSIZE]; // 元のウィンドウタイトルが入ります

	// 現在のウィンドウタイトルを取得します

	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

	// 独自に、ウィンドウの新規タイトルをフォーマットします

	wsprintf(pszNewWindowTitle,"%d/%d",
			 GetTickCount(),
			 GetCurrentProcessId());

	// 現在のウィンドウタイトルを変更します

	SetConsoleTitle(pszNewWindowTitle);

	// ウィンドウタイトルのアップデートを確実なものにさせます

	Sleep(40);

	// ウィンドウの新規タイトルを探しにいきます

	hwndFound=FindWindow(NULL, pszNewWindowTitle);

	// 元のウィンドウタイトルへ戻します

	SetConsoleTitle(pszOldWindowTitle);

	return(hwndFound);
}

//********************************************************************************************
/**
 * @brief		text_play関数
 *
 * 音声データを再生する
 *
 * @author		@loftkun
 * @date		2012/02/12(日) 12:45:59
 *
 * @return		int	
 */
//********************************************************************************************
int text_play()
{
	if(hModule == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		exit(1);
	}
	//DLLから合成音再生APIアドレスを取得
	typedef short	(*pFunc_VT_PLAYTTS_JPN)(HWND hcaller, UINT umsg, char *text_buff, int nSpeakerID, int pitch, int speed, int volume, int pause, int dictidx, int texttype);
	pFunc_VT_PLAYTTS_JPN	Func_VT_PLAYTTS_JPN = NULL;
	Func_VT_PLAYTTS_JPN = (pFunc_VT_PLAYTTS_JPN)GetProcAddress(hModule,"VT_PLAYTTS_JPN");
	if(Func_VT_PLAYTTS_JPN == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		unload();
		exit(0);
	}

	//ウィンドウハンドルを取得
	HWND hWnd = GetConsoleHwnd();
	if(hWnd == NULL){
		printf("[ERR ][%s] FindWindow\n", __FUNCTION__);
		unload();
		exit(0);
	}
	short ret = Func_VT_PLAYTTS_JPN(
		hWnd,			//ウィンドウハンドル
		WM_APP,			//ユーザ定義のウィンドウメッセージ
		INPUT_TEXT,
		SPEAKER_ID,
		-1, -1, -1, -1, -1, -1);	//pitch, speed, volume, pause, dictidx, texttype
	if(ret != 1){
		printf("[ERR ][%s] VT_PLAYTTS_JPN ret=%d\n", __FUNCTION__, ret);
		unload();
		exit(0);
	}

	//再生が終わるまで待つ
	//ここではスリープで待っているが、
	//本来は再生が終わった旨をウィンドウメッセージで取得するように実装する
	Sleep(5000);
	return 0;
}
