#pragma once

// ycapture.h : フィルタの情報定義

// ここは自分の製品に合わせて変更すること

// フィルタ名定義
#define NAME_CAPTURESOURCE L"ARgil Capture Source"

// {5C2CD55C-92AD-4999-8666-912BD3E70001}
DEFINE_GUID(CLSID_CaptureSource, 
	0x5c2cd55c, 0x92ad, 0x4999, 0x86, 0x66, 0x91, 0x2b, 0xd3, 0xe7, 0x0, 0x1);

// イベント・メモリマップドファイル名定義
// フィルタ側・送信側で同一の名前を使用すること

// 書き込みを示すイベント名
#define CS_EVENT_WRITE	L"ARgil_CaptureSource_Write"
// データ読み込みを示すミューテックス名
#define CS_EVENT_READ	L"ARgil_CaptureSource_Read"
// 共有データファイル名(メモリマップドファイル)
#define CS_SHARED_PATH	L"ARgil_CaptureSource_Data"
