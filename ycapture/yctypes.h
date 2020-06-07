#pragma once

/**
* 共有バッファの形式です。
*/
typedef struct __tagSHARED_BUF {

	/**
	* 最大サイズを保持します。
	*/
	size_t maxSize;

	/**
	* 実際のサイズを保持します。
	*/
	size_t actualSize;

	/**
	* データを保持します。
	*/
	unsigned char data[1];

}SHARED_BUF;

/**
* バッファ情報です。
*/
typedef struct __tagBUFFER_DESC {
	/**
	* 幅を保持します。
	*/
	int width;

	/**
	* 高さを保持します。
	*/
	int height;

	/**
	* 時刻を保持します。
	*/
	LONGLONG time;

}BUFFER_DESC;