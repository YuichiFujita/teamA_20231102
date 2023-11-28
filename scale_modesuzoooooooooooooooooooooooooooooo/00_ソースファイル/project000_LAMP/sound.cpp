//============================================================
//
//	サウンド処理 [sound.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sound.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const CSound::SSoundInfo CSound::mc_aSoundInfo[LABEL_MAX] =	// サウンド情報
{
	{ "data\\BGM\\bgm_general.wav", -1 },	// BGM (汎用)
	{ "data\\BGM\\bgm_tutorial.wav", -1 },	// BGM (チュートリアル)
	{ "data\\BGM\\bgm_game.wav", -1 },		// BGM (ゲーム)

	{ "data\\SE\\select000.wav",  0 },		// 選択操作音00
	{ "data\\SE\\select001.wav",  0 },		// 選択操作音01
	{ "data\\SE\\decision000.wav",  0 },	// 決定音00
	{ "data\\SE\\decision001.wav",  0 },	// 決定音01
	{ "data\\SE\\decision002.wav",  0 },	// 決定音02
	{ "data\\SE\\vibration000.wav",  0 },	// スマホバイブ音
	{ "data\\SE\\message000.wav",  0 },		// メッセージ音
	{ "data\\SE\\spawn000.wav",  0 },		// 生成音
	{ "data\\SE\\save000.wav",  0 },		// セーブ音
	{ "data\\SE\\walk000.wav",  0 },		// 歩行音（ビル）
	{ "data\\SE\\walk001.wav",  0 },		// 歩行音（障害物）
	{ "data\\SE\\jump000.wav",  0 },		// ジャンプ
	{ "data\\SE\\slideing000.wav",  0 },	// スライディング
	{ "data\\SE\\jumppad000.wav",  0 },		// ジャンプパッド
	{ "data\\SE\\stick000.wav",  0 },		// くっつき音
	{ "data\\SE\\hit000.wav",  0 },			// ぶつかり音
};

//************************************************************
//	親クラス [CSound] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSound::CSound()
{
	// メンバ変数をクリア
	m_pXAudio2			= NULL;	// XAudio2オブジェクトへのインターフェイス
	m_pMasteringVoice	= NULL;	// マスターボイス
	memset(&m_apSourceVoice[0],	0, sizeof(m_apSourceVoice));	// ソースボイス
	memset(&m_apDataAudio[0],	0, sizeof(m_apDataAudio));		// オーディオデータ
	memset(&m_aSizeAudio[0],	0, sizeof(m_aSizeAudio));		// オーディオデータサイズ
}

//============================================================
//	デストラクタ
//============================================================
CSound::~CSound()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSound::Init(HWND hWnd)
{
	// メンバ変数を初期化
	m_pXAudio2			= NULL;	// XAudio2オブジェクトへのインターフェイス
	m_pMasteringVoice	= NULL;	// マスターボイス
	memset(&m_apSourceVoice[0],	0, sizeof(m_apSourceVoice));	// ソースボイス
	memset(&m_apDataAudio[0],	0, sizeof(m_apDataAudio));		// オーディオデータ
	memset(&m_aSizeAudio[0],	0, sizeof(m_aSizeAudio));		// オーディオデータサイズ

	// 変数を宣言
	HRESULT hr;		// 終了確認用

	// COMライブラリの初期化
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "COMライブラリの初期化に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(m_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(mc_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{ // ファイルポインタを先頭に移動

			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// ソースボイスの生成
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = mc_aSoundInfo[nCntSound].nCntLoop;

		// オーディオバッファの登録
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSound::Uninit(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = NULL;
	
			// オーディオデータの開放
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;
	
	if (m_pXAudio2 != NULL)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//============================================================
//	セグメント再生 (再生中なら停止)
//============================================================
HRESULT CSound::Play(ELabel label)
{
	// 変数を宣言
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = mc_aSoundInfo[label].nCntLoop;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // 再生中

		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_apSourceVoice[label]->Start(0);

	return S_OK;
}

//============================================================
//	セグメント停止 (ラベル指定)
//============================================================
void CSound::Stop(ELabel label)
{
	// 変数を宣言
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // 再生中

		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//============================================================
//	セグメント停止 (全て)
//============================================================
void CSound::Stop(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//============================================================
//	生成処理
//============================================================
CSound *CSound::Create(HWND hWnd)
{
	// ポインタを宣言
	CSound *pSound = NULL;	// サウンド生成用

	if (pSound == NULL)
	{ // 使用されていない場合

		// メモリを確保
		pSound = new CSound;	// サウンド
	}
	else { assert(false); return NULL; }	// 使用中

	if (pSound != NULL)
	{ // 確保に成功している場合

		// サウンドの初期化
		if (FAILED(pSound->Init(hWnd)))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pSound;
			pSound = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pSound;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CSound::Release(CSound *&prSound)
{
	if (prSound != NULL)
	{ // 使用中の場合

		// サウンドの終了
		prSound->Uninit();

		// メモリ開放
		delete prSound;
		prSound = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	チャンクのチェック
//============================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	// 変数を宣言
	HRESULT hr = S_OK;	// 終了確認用
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{ // ファイルポインタを先頭に移動

		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{ // チャンクの読み込み

			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{ // チャンクデータの読み込み

			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':

			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;

			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{ // ファイルタイプの読み込み

				hr = HRESULT_FROM_WIN32(GetLastError());
			}

			break;

		default:

			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{ // ファイルポインタをチャンクデータ分移動

				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//============================================================
//	チャンクデータの読み込み
//============================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	// 変数を宣言
	DWORD dwRead;
	
	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{ // ファイルポインタを指定位置まで移動

		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{ // データの読み込み

		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}
