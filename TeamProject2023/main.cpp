#include<Windows.h>
#include"Game.h"
//#pragma comment�œǂݍ��߂΁A�X�^�e�B�b�N���C�u�����ł�.lib�ǂݍ��߂�?
//libfbxsdk-md.pdb' ��������܂���ł����BLibraryTest.exe �́A�f�o�b�O�ɃI�u�W�F�N�g �t�@�C����K�v�Ƃ��� /DEBUG:FASTLINK ���g���ăr���h����܂����B
//�́A�u�r���h�v�́A�u�\�����[�V�����̊��S�ȃv���O�����f�[�^�x�[�X�t�@�C�����\�z����v�����s����ƒ���

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Game::GetInstance()->Run();
	return 0;
}