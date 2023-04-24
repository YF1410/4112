//#include "Network.h"
//#include"ErrorProcess.h"
//
//#include<Windows.h>
//
//MelLib::Network* MelLib::Network::GetInstance()
//{
//	static Network n;
//	return &n;
//}
//
//bool MelLib::Network::Initialize()
//{
//	int result = 0;
//
//	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if(result != 0)
//	{
//		ErrorProcess::GetInstance()->StartErroeProcess(L"Network�N���X�̏������Ɏ��s���܂����B", true);
//#ifdef _DEBUG
//		OutputDebugString(L"Network�N���X�̏������Ɏ��s���܂����B\n");
//#endif // _DEBUG
//
//		return false;
//	}
//}
//
//void MelLib::Network::Finalize()
//{
//	WSACleanup();
//}
//
//MelLib::Network::NetWorkError MelLib::Network::StartServer(const USHORT portNum)
//{
//	//�ҋ@�p�\�P�b�g
//	SOCKET serverPreAcceptSocket = {};
//
//	// IP�A�h���X��|�[�g�ԍ���ێ�����\����
//	SOCKADDR_IN serverDDR = {};
//	SOCKADDR_IN clientDDR = {};
//
//	// ���X���\�P�b�g�I�[�v��
//	serverPreAcceptSocket = socket(AF_INET, SOCK_STREAM, 0);
//	if(serverPreAcceptSocket == INVALID_SOCKET)
//	{
//#ifdef _DEBUG
//		OutputDebugString(L"�\�P�b�g�̃I�[�v���Ɏ��s���܂����B\n");
//#endif // _DEBUG
//		return NetWorkError::FAILED_SOCKET_OPEN;
//	}
//
//	// �\�P�b�g�ɖ��O��t����
//	memset(&serverDDR, 0, sizeof(serverDDR));
//	serverDDR.sin_family = AF_INET;
//
//	//htons �z�X�g�o�C�g�I�[�_�[(HBO)����l�b�g���[�N�o�C�g�I�[�_�[(NBO)�ɕϊ�
//	serverDDR.sin_port = htons(portNum);
//	serverDDR.sin_addr.s_addr = INADDR_ANY;
//
//	//bind ���[�J���A�h���X(�T�[�o�[����SOCKADDR_IN)�ƃ\�P�b�g�̊֘A�t��
//	if(bind(serverPreAcceptSocket,(struct sockaddr*)&serverDDR,sizeof(serverDDR)) == SOCKET_ERROR)
//	{
//		closesocket(serverPreAcceptSocket);
//#ifdef _DEBUG
//		OutputDebugString(L"�o�C���h�Ɏ��s���܂����B\n");
//#endif // _DEBUG
//		return NetWorkError::FAILED_BIND;
//	}
//
//	//listen �\�P�b�g�̐ڑ��ҋ@�J�n
//	if(listen(serverPreAcceptSocket,0) == SOCKET_ERROR)
//	{
//		closesocket(serverPreAcceptSocket);
//#ifdef _DEBUG
//		OutputDebugString(L"�\�P�b�g�̐ڑ��҂��J�n�Ɏ��s���܂����B\n");
//#endif // _DEBUG
//		return NetWorkError::FAILED_START_SOCKET_CONNECT_STANDBY;
//	}
//
//	//accept �ڑ���������
//	int clientDDRSize = sizeof(clientDDR);
//	serverAcceptSocket = accept(serverPreAcceptSocket, (SOCKADDR*)&clientDDR, &clientDDRSize);
//	if(serverAcceptSocket == INVALID_SOCKET)
//	{
//		closesocket(serverPreAcceptSocket);
//#ifdef _DEBUG
//		OutputDebugString(L"�ڑ��Ɏ��s���܂����B\n");
//#endif // _DEBUG
//		return NetWorkError::FAILED_CONNECT;
//	}
//
//	//�ҋ@�p�̃\�P�b�g�����
//	closesocket(serverPreAcceptSocket);
//
//	return NetWorkError::ERROR_NONE;
//}
//
//MelLib::Network::NetWorkError MelLib::Network::StartClient(const std::string& address, const USHORT portNum)
//{
//	SOCKADDR_IN clientDDR = {};
//
//	//�\�P�b�g�I�[�v��
//	clientAcceptSocket = socket(AF_INET, SOCK_STREAM, 0);
//	if(clientAcceptSocket == INVALID_SOCKET)
//	{
//#ifdef _DEBUG
//		OutputDebugString(L"�\�P�b�g�̃I�[�v���Ɏ��s���܂����B\n");
//#endif // _DEBUG
//		return NetWorkError::FAILED_SOCKET_OPEN;
//	}
//
//	//HOSTENT �z�X�g�̏����܂Ƃ߂�����
//	HOSTENT* hostent = nullptr;
//
//	//gethostbyname �T�[�o�[���ŃT�[�o�[���擾����֐�?
//	hostent = gethostbyname(address.c_str());
//	if(!hostent)
//	{
//		UINT addr = inet_addr(address.c_str());
//		
//		//gethostbyaddr IP�A�h���X�ŃT�[�o�[���擾����֐�?
//		hostent = gethostbyaddr(address.c_str(),4,AF_INET);
//		if(!hostent)
//		{
//#ifdef _DEBUG
//			OutputDebugString(L"�z�X�g��������܂���ł����B\n");
//#endif // _DEBUG
//			return NetWorkError::FAILED_FIND_HOST;
//		}
//	}
//
//	//�\�P�b�g���T�[�o�[�ɐڑ�
//	//clientDDR�̐擪��0��sizeof(SOCKADDR_IN)�̒l���ǉ�
//	memset(&clientDDR, 0, sizeof(SOCKADDR_IN));
//	clientDDR.sin_family = hostent->h_addrtype;
//	clientDDR.sin_port = htons(portNum);
//	clientDDR.sin_addr.s_addr = *((ULONG*)hostent->h_addr);
//	if(connect(clientAcceptSocket,(SOCKADDR*)&clientDDR,sizeof(clientDDR)) == SOCKET_ERROR)
//	{
//#ifdef _DEBUG
//		OutputDebugString(L"�T�[�o�[�ɐڑ��ł��܂���ł����B\n");
//#endif // _DEBUG
//		return NetWorkError::FAILED_CONNECT;
//	}
//
//	return NetWorkError::ERROR_NONE;
//}
