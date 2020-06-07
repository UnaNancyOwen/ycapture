// testclient.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <CaptureSender.h>
#include <ycapture.h>

#define WIDTH	640
#define HEIGHT	200

// テスト処理を実行します。
int _tmain(int argc, _TCHAR* argv[])
{
	// 各種オブジェクトの用意
	CaptureSender sender(CS_SHARED_PATH, CS_EVENT_WRITE, CS_EVENT_READ);
	unsigned char* imageBuf = new unsigned char[WIDTH * HEIGHT * 3];
	DWORD avgTimePF = 1000 / 15;		// 15fps

	// 1000回サンプルを更新
	int count = 0;
	for(int i = 0; i < 1000; i ++) {
		// 送信ごとにR成分を変える
		count += 10;
		if(count > 255) {
			count = 0;
		}
		unsigned char r = count;
		unsigned char g = 0;
		unsigned char b = 0;

		for(int y = 0; y < HEIGHT; y ++) {
			for(int x = 0; x < WIDTH; x ++) {
				int offs = (x + y * WIDTH) * 3;
				imageBuf[offs + 0] = r;
				imageBuf[offs + 1] = g;
				imageBuf[offs + 2] = b;
			}
		}

		// 送信する
		HRESULT hr = sender.Send(i * avgTimePF, WIDTH, HEIGHT, imageBuf);
		if(FAILED(hr)) {
			// エラー: 続行しない
			fprintf(stderr, "Error: 0x%08x\n", hr);
			break;
		}else if(hr == S_OK) {
			// 送信成功
			printf("Sent: %d\n", i);
		}else{
			// フィルタ未起動。無視
			printf("Ignored: %d\n", i);
		}

		// 次のサンプル送信まで待機
		Sleep(avgTimePF);
	}

	// 後始末
	delete[] imageBuf;
	imageBuf = NULL;
	return 0;
}

