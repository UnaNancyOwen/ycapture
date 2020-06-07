// testclient.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <CaptureSender.h>
#include <ycapture.h>

#define WIDTH	640
#define HEIGHT	200

// �e�X�g���������s���܂��B
int _tmain(int argc, _TCHAR* argv[])
{
	// �e��I�u�W�F�N�g�̗p��
	CaptureSender sender(CS_SHARED_PATH, CS_EVENT_WRITE, CS_EVENT_READ);
	unsigned char* imageBuf = new unsigned char[WIDTH * HEIGHT * 3];
	DWORD avgTimePF = 1000 / 15;		// 15fps

	// 1000��T���v�����X�V
	int count = 0;
	for(int i = 0; i < 1000; i ++) {
		// ���M���Ƃ�R������ς���
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

		// ���M����
		HRESULT hr = sender.Send(i * avgTimePF, WIDTH, HEIGHT, imageBuf);
		if(FAILED(hr)) {
			// �G���[: ���s���Ȃ�
			fprintf(stderr, "Error: 0x%08x\n", hr);
			break;
		}else if(hr == S_OK) {
			// ���M����
			printf("Sent: %d\n", i);
		}else{
			// �t�B���^���N���B����
			printf("Ignored: %d\n", i);
		}

		// ���̃T���v�����M�܂őҋ@
		Sleep(avgTimePF);
	}

	// ��n��
	delete[] imageBuf;
	imageBuf = NULL;
	return 0;
}

