//============================================================
//
//	サウンドヘッダー [sound.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	クラス定義
//************************************************************
// サウンドクラス
class CSound
{
public:
	// コンストラクタ
	CSound();

	// デストラクタ
	~CSound();

	// サウンドラベル列挙
	enum ELabel
	{
		LABEL_BGM_GENERAL = 0,	// BGM (汎用)
		LABEL_BGM_TUTORIAL,		// BGM (チュートリアル)
		LABEL_BGM_GAME,			// BGM (ゲーム)

		LABEL_SE_SELECT_000,	// 選択操作音00
		LABEL_SE_SELECT_001,	// 選択操作音01
		LABEL_SE_DECISION_000,	// 決定音00
		LABEL_SE_DECISION_001,	// 決定音01
		LABEL_SE_DECISION_002,	// 決定音02
		LABEL_SE_VIBRATION,		// スマホバイブ音
		LABEL_SE_MESSAGE,		// メッセージ音
		LABEL_SE_SPAWN,			// 生成音
		LABEL_SE_SAVE,			// セーブ音
		LABEL_SE_WALK_BUILD,	// 歩行音（ビル）
		LABEL_SE_WALK_OBS,		// 歩行音（障害物）
		LABEL_SE_JUMP,			// ジャンプ
		LABEL_SE_SLIDE,			// スライディング
		LABEL_SE_JUMPPAD,		// ジャンプパッド
		LABEL_SE_STICK,			// くっつき音
		LABEL_SE_HIT,			// ぶつかり音
		LABEL_MAX,				// この列挙型の総数
	};

	// サウンド情報構造体
	struct SSoundInfo
	{
		char *pFilename;	// ファイル名
		int   nCntLoop;		// ループカウント
	};

	// メンバ関数
	HRESULT Init(HWND hWnd);	// 初期化
	void Uninit(void);			// 終了
	HRESULT Play(ELabel label);	// 再生
	void Stop(ELabel label);	// 停止
	void Stop(void);			// 全停止

	// 静的メンバ関数
	static CSound *Create(HWND hWnd);			// 生成
	static HRESULT Release(CSound *&pSound);	// 破棄

private:
	// メンバ関数
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);	// チャンクチェック
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// チャンクデータ読込

	// メンバ変数
	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice    *m_apSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[LABEL_MAX];						// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX];						// オーディオデータサイズ
	static const SSoundInfo mc_aSoundInfo[LABEL_MAX];	// サウンド情報定数
};

#endif	// _SOUND_H_
