//****************************************************************************************************************
//
// クライアントのTCP送受信処理(tcp_client.cpp)
// Auther : 唐﨑結斗
// 概要 : クライアントTCP送受信クラスの管理
//
//****************************************************************************************************************

//***************************************************************************
// インクルード
//***************************************************************************
#include "tcp_client.h"
#include <stdio.h>

//===========================================================
// コンストラクタ
// Auther : 唐﨑結斗
// 概要 : メンバ変数のクリアをする
//===========================================================
CTCPClient::CTCPClient()
{
	// ソケットのクリア
	m_sock = INVALID_SOCKET;

	// アドレスの初期化	
	memset(&m_addr, 0, sizeof(m_addr));
}

//===========================================================
// デストラクタ
// Auther : 唐﨑結斗
// 概要 : 
//===========================================================
CTCPClient::~CTCPClient()
{

}

//===========================================================
// 初期化
// Auther : 唐﨑結斗
// 概要 : ソケットを作成し、サーバーと接続したかを返す
//===========================================================
bool CTCPClient::Init(const char * pIpAddres, int nPortNum)
{
	// ソケットの作成
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sock == INVALID_SOCKET)
	{// ソケット作成失敗
		printf("ソケット作成失敗\n");
		return false;
	}
	else
	{// ソケット作成成功
		printf("ソケット作成成功\n");
	}

	// アドレスの設定
	m_addr.sin_family = AF_INET;													// IPV4の設定
	m_addr.sin_port = htons(nPortNum);												// ポート番号の設定
	m_addr.sin_addr.S_un.S_addr = inet_addr(pIpAddres);								// IPアドレスの設定

	// 接続する
	if (connect(m_sock, (struct sockaddr*)&m_addr, sizeof(m_addr)) != 0)
	{// 接続失敗
		printf("接続失敗\n");
		return false;
	}
	else
	{// 接続成功
		printf("接続成功\n");
		return true;
	}
}

//===========================================================
// オーバーロードされた初期化
// Auther : 唐﨑結斗
// 概要 : 引数のソケットをメンバ変数に代入し、NULLチェック
//===========================================================
bool CTCPClient::Init(SOCKET sock)
{
	// ソケットの作成
	m_sock = sock;

	return true;
}

//===========================================================
// 送信
// Auther : 唐﨑結斗
// 概要 : send関数を使ってサーバーにデーターを送信
//===========================================================
int CTCPClient::Send(char *pSendData, int nSendDataSize)
{
	if (m_sock == INVALID_SOCKET)
	{// ソケットが使用されてない場合
		return 0;
	}
	else
	{// ソケットが使用されてる
		// 返り値用の変数宣言
		int nSendByte = 0;

		// データの送信
		printf("%sを送信\n", pSendData);

		// データを送信する
		nSendByte =  send(m_sock, pSendData, nSendDataSize, 0);

		return nSendByte;
	}
}

//===========================================================
// 受信
// Auther : 唐﨑結斗
// 概要 : recv関数を使ってサーバーにデーターを受信し、受信量を返す
//===========================================================
int CTCPClient::Recv(char *pRecvData, int nRecvDataSize)
{
	if (m_sock == INVALID_SOCKET)
	{// ソケットが使用されてない場合
		return 0;
	}
	else
	{// ソケットが使用されてる
		// 返り値用の変数宣言
		int nRecvByte = 0;

		// データを受信する
		nRecvByte =  recv(m_sock, pRecvData, nRecvDataSize, 0);

		if (nRecvByte <= 0)
		{// 0以下の数値を受信した場合
			Uninit();
		}
		else
		{// 受信データの出力
			printf("%sを受信\n", pRecvData);
		}

		return nRecvByte;
	}
}

//===========================================================
// 終了
// Auther : 唐﨑結斗
// 概要 : ソケットの解放を行う
//===========================================================
void CTCPClient::Uninit(void)
{
	if (m_sock == INVALID_SOCKET)
	{
		return;
	}
	else
	{
		// ソケットを解放する
		closesocket(m_sock);
		m_sock = INVALID_SOCKET;
		printf("接続が切断されました\n");
	}
}
