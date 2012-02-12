//********************************************************************************************
/**
 * @file		VoiceText Japanese Engine API �T���v��(Windows C/C++)
 * @brief		
 *	vt_jpn.dll���g�p�B
 *  VoiceText Japanese Engine API�̃T���v���ł��B
 *  �l�Ŋy���ނ݂̂ɗ��߂܂��傤�B
 *  ���C�Z���X�͊e���ŏ������ĉ������B
 *
 * @author		@loftkun
 * @date		2012/02/12(��) 12:40:02
 */
//********************************************************************************************
#include <windows.h>
#include <stdio.h>
#include "vt_jpn.h" //VoiceText Japanese Engine API(���̃T���v���ł͒萔��`�Q�ƂɎg�p�B���ɕK�{�ł͂Ȃ�)

//API�ɐݒ肷��萔
#define DB_PATH			"C:\\hoge" //DB�ւ̃p�X
#define LICENSE_PATH	"C:\\hoge" //���C�Z���X�t�@�C���ւ̃p�X
#define SPEAKER_ID		(1)	//�b�҂�ID(0=miyu, 1=show, 2=misaki)

//�t�@�C���p�X
#define INPUT_TEXT			"������₳�܂���"		//�����ɕϊ�������������
#define OUTPUT_TO_FILE		"OutputTextToFile.wav"	//�����t�@�C������API�ŏo�͂���t�@�C����
#define OUTPUT_TO_BUFFER	"OutputTextToBuffer.pcm"//�����o�b�t�@����API�ŏo�͂��ꂽ�o�b�t�@���o�͂���t�@�C����

//�O���[�o���ϐ�
HMODULE	hModule = NULL; //DLL�̃n���h���AAPI�֐��A�h���X�̎��o���Ɏg��

//�v���g�^�C�v�錾
int load();
int unload();
int text_to_file();
int text_to_buf();
int text_play();

//********************************************************************************************
/**
 * @brief		main�֐�
 * @author		@loftkun
 * @date		2012/02/12(��) 12:45:13
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
 * @brief		load�֐�
 *
 * vt_jpn.dll�����[�h
 * loadAPI���R�[��
 *
 * @author		@loftkun
 * @date		2012/02/12(��) 12:45:37
 *
 * @return		int	
 */
//********************************************************************************************
int load()
{
	//DLL�̃n���h�����擾
	hModule = LoadLibrary("vt_jpn.dll");
	if(hModule == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		exit(1);
	}
	//DLL���珉����API�A�h���X���擾
	typedef short	(*pFunc_VT_LOADTTS_JPN)(HWND hWnd, int nSpeakerID, char *db_path, char *licensefile);
	pFunc_VT_LOADTTS_JPN Func_VT_LOADTTS_JPN = 
		Func_VT_LOADTTS_JPN=(pFunc_VT_LOADTTS_JPN)GetProcAddress(hModule,"VT_LOADTTS_JPN");
	if(Func_VT_LOADTTS_JPN == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		exit(1);
	}
	//������API�R�[��
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
 * @brief		unload�֐�
 *
 * unloadAPI���R�[��
 *
 * @author		@loftkun
 * @date		2012/02/12(��) 12:45:46
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
	//DLL����I��API�A�h���X���擾
	typedef void (*pFunc_VT_UNLOADTTS_JPN)(int nSpeakerID);
	pFunc_VT_UNLOADTTS_JPN	Func_VT_UNLOADTTS_JPN = 
		Func_VT_UNLOADTTS_JPN=(pFunc_VT_UNLOADTTS_JPN)GetProcAddress(hModule,"VT_UNLOADTTS_JPN");
	if(Func_VT_UNLOADTTS_JPN == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		exit(1);
	}
	Func_VT_UNLOADTTS_JPN(SPEAKER_ID);//����API�͖߂�l�͂Ȃ�
	return 0;
}

//********************************************************************************************
/**
 * @brief		text_to_file�֐�
 *
 * �����f�[�^���t�@�C���ɏo�͂���
 *
 * @author		@loftkun
 * @date		2012/02/12(��) 12:45:49
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
	//DLL���特���t�@�C���o��API�A�h���X���擾
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
 * @brief		text_to_buf�֐�
 *
 * �ϊ����ꂽ�����f�[�^�o�b�t�@���擾����
 * �{�֐��ł͂��̂܂܃o�b�t�@���t�@�C���ɏo�͂��Ă���B
 *
 * @author		@loftkun
 * @date		2012/02/12(��) 12:45:53
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

	//DLL���特���o�b�t�@�o��API�A�h���X���擾
	typedef int		(*pFunc_VT_TextToBuffer_JPN)(int fmt, char *tts_text, char *output_buff, int *output_len, int flag, int nThreadID, int nSpeakerID, int pitch, int speed, int volume, int pause, int dictidx, int texttype);
	pFunc_VT_TextToBuffer_JPN	Func_VT_TextToBuffer_JPN = NULL;
	Func_VT_TextToBuffer_JPN=(pFunc_VT_TextToBuffer_JPN)GetProcAddress(hModule,"VT_TextToBuffer_JPN");
	if(Func_VT_TextToBuffer_JPN == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		unload();
		exit(0);
	}

	//frameflag=-1�ŃR�[�����āAPCM�t���[���o�b�t�@�T�C�Y���擾����
	//frameflag�ɂ��Ă�API�d�l�����Q��(-1=�T�C�Y�擾, 0=�擪�o�b�t�@�擾, 1=2�Ԗڈȍ~�o�b�t�@�擾, 2=�I���w��)
	int ret = 0, frameflag = -1;
	int nFramesize=0;
	ret = Func_VT_TextToBuffer_JPN (
		VT_BUFFER_API_FMT_S16PCM,	//���������������̃t�H�[�}�b�g
		(char *)NULL,				//�������镶����
		(char *)NULL,				//����������������ۑ�����t���[���o�b�t�@��pointer
		&nFramesize,				//�t���[���o�b�t�@�ɕۑ����ꂽ�������̃T�C�Y
		frameflag,					//API�̓�����w�肷��flag
		0,							//�X���b�h�g���ꍇ�ɋ�ʂ��邽�߂̃X���b�h�ԍ�
		SPEAKER_ID,					//�b��ID
		-1, -1, -1, -1, -1, -1);	//pitch, speed, volume, pause, dictidx, texttype
	if(ret < 0){
		printf("[ERR ][%s] VT_TextToBuffer_JPN ret=%d\n", __FUNCTION__, ret);
		unload();
		exit(0);
	}
	printf("[INFO][%s] VT_TextToBuffer_JPN get nFramesize=%d\n", __FUNCTION__, nFramesize);

	//�擾�����t���[���o�b�t�@�T�C�Y�ŁA
	//�����o�b�t�@�󂯎��o�b�t�@�m��
	char * framebuf = (char *) malloc (nFramesize);
	if(framebuf == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		unload();
		exit(0);
	}

	//�o�̓t�@�C��Open
	FILE * fp = NULL;
	errno_t errNo = fopen_s(&fp, OUTPUT_TO_BUFFER,"wb");
	if(errNo != 0){
		printf("[ERR ][%s]\n", __FUNCTION__);
		unload();
		exit(0);
	}

	//�����o�b�t�@���t�@�C���ɏo��
	ret = 0;
	frameflag = 0;
	while(ret == 0){//�߂�l��0�Ȃ�p���A1(����I��)�Ń��[�v�𔲂���B��(�ُ�I��)�ŏI���B
		//�����o�b�t�@�擾
		ret = Func_VT_TextToBuffer_JPN (
			VT_BUFFER_API_FMT_S16PCM ,		//���������������̃t�H�[�}�b�g
			INPUT_TEXT,						//�������镶����
			framebuf,						//����������������ۑ�����t���[���o�b�t�@��pointer
			&nFramesize,					//�t���[���o�b�t�@�ɕۑ����ꂽ�������̃T�C�Y
			frameflag,						//API�̓�����w�肷��flag
			0,								//�X���b�h�g���ꍇ�ɋ�ʂ��邽�߂̃X���b�h�ԍ�
			SPEAKER_ID,						//�b��ID
			-1, -1, -1, -1, -1, -1);		//pitch, speed, volume, pause, dictidx, texttype
		printf("[INFO][%s] VT_TextToBuffer_JPN ret=%d nFramesize=%d\n", __FUNCTION__, ret, nFramesize);
		if(ret < 0){
			printf("[ERR ][%s] VT_TextToBuffer_JPN ret=%d\n", __FUNCTION__, ret);
			unload();
			fclose(fp);
			exit(0);
		}
		//frameflag�X�V
		if (frameflag == 0){
			frameflag = 1;//2�Ԗڈȍ~�̃o�b�t�@
		}
		//�t�@�C���ɏo��
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
 * @brief		�E�B���h�E�n���h�����擾����
 *
 * MSDN���玝���Ă����\�[�X�B
 *
 * @author		http://support.microsoft.com/kb/124103/ja
 * @date		2012/02/12(��) 14:43:59
 *
 * @return		HWND	
 */
//********************************************************************************************
HWND GetConsoleHwnd(void)
{
	#define MY_BUFSIZE 1024 // �R���\�[�� �E�B���h�E�̃^�C�g���p�̃o�b�t�@�T�C�Y
	HWND hwndFound;		 // �Ăяo�����֕Ԃ����l
	char pszNewWindowTitle[MY_BUFSIZE];
						   // �쐬�����E�B���h�E�̃^�C�g��������܂�
	char pszOldWindowTitle[MY_BUFSIZE]; // ���̃E�B���h�E�^�C�g��������܂�

	// ���݂̃E�B���h�E�^�C�g�����擾���܂�

	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

	// �Ǝ��ɁA�E�B���h�E�̐V�K�^�C�g�����t�H�[�}�b�g���܂�

	wsprintf(pszNewWindowTitle,"%d/%d",
			 GetTickCount(),
			 GetCurrentProcessId());

	// ���݂̃E�B���h�E�^�C�g����ύX���܂�

	SetConsoleTitle(pszNewWindowTitle);

	// �E�B���h�E�^�C�g���̃A�b�v�f�[�g���m���Ȃ��̂ɂ����܂�

	Sleep(40);

	// �E�B���h�E�̐V�K�^�C�g����T���ɂ����܂�

	hwndFound=FindWindow(NULL, pszNewWindowTitle);

	// ���̃E�B���h�E�^�C�g���֖߂��܂�

	SetConsoleTitle(pszOldWindowTitle);

	return(hwndFound);
}

//********************************************************************************************
/**
 * @brief		text_play�֐�
 *
 * �����f�[�^���Đ�����
 *
 * @author		@loftkun
 * @date		2012/02/12(��) 12:45:59
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
	//DLL���獇�����Đ�API�A�h���X���擾
	typedef short	(*pFunc_VT_PLAYTTS_JPN)(HWND hcaller, UINT umsg, char *text_buff, int nSpeakerID, int pitch, int speed, int volume, int pause, int dictidx, int texttype);
	pFunc_VT_PLAYTTS_JPN	Func_VT_PLAYTTS_JPN = NULL;
	Func_VT_PLAYTTS_JPN = (pFunc_VT_PLAYTTS_JPN)GetProcAddress(hModule,"VT_PLAYTTS_JPN");
	if(Func_VT_PLAYTTS_JPN == NULL){
		printf("[ERR ][%s]\n", __FUNCTION__);
		unload();
		exit(0);
	}

	//�E�B���h�E�n���h�����擾
	HWND hWnd = GetConsoleHwnd();
	if(hWnd == NULL){
		printf("[ERR ][%s] FindWindow\n", __FUNCTION__);
		unload();
		exit(0);
	}
	short ret = Func_VT_PLAYTTS_JPN(
		hWnd,			//�E�B���h�E�n���h��
		WM_APP,			//���[�U��`�̃E�B���h�E���b�Z�[�W
		INPUT_TEXT,
		SPEAKER_ID,
		-1, -1, -1, -1, -1, -1);	//pitch, speed, volume, pause, dictidx, texttype
	if(ret != 1){
		printf("[ERR ][%s] VT_PLAYTTS_JPN ret=%d\n", __FUNCTION__, ret);
		unload();
		exit(0);
	}

	//�Đ����I���܂ő҂�
	//�����ł̓X���[�v�ő҂��Ă��邪�A
	//�{���͍Đ����I������|���E�B���h�E���b�Z�[�W�Ŏ擾����悤�Ɏ�������
	Sleep(5000);
	return 0;
}
