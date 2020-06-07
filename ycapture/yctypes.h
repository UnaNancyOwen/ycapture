#pragma once

/**
* ���L�o�b�t�@�̌`���ł��B
*/
typedef struct __tagSHARED_BUF {

	/**
	* �ő�T�C�Y��ێ����܂��B
	*/
	size_t maxSize;

	/**
	* ���ۂ̃T�C�Y��ێ����܂��B
	*/
	size_t actualSize;

	/**
	* �f�[�^��ێ����܂��B
	*/
	unsigned char data[1];

}SHARED_BUF;

/**
* �o�b�t�@���ł��B
*/
typedef struct __tagBUFFER_DESC {
	/**
	* ����ێ����܂��B
	*/
	int width;

	/**
	* ������ێ����܂��B
	*/
	int height;

	/**
	* ������ێ����܂��B
	*/
	LONGLONG time;

}BUFFER_DESC;