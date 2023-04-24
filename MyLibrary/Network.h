//#pragma once
//#include<winsock2.h>
//#pragma comment(lib,"Ws2_32.lib")
//
//namespace MelLib 
//{
//	//�`���[�g���A��
//	// https://docs.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock
//
//	// WinSock2�𗘗p�����ʐM�N���X
//	class Network
//	{
//	public:
//		enum class NetWorkError
//		{
//			ERROR_NONE,
//			FAILED_SOCKET_OPEN,
//			FAILED_BIND,
//			FAILED_START_SOCKET_CONNECT_STANDBY,
//			FAILED_CONNECT,
//			FAILED_FIND_HOST,
//		};
//
//	private:
//		WSADATA wsaData = {};
//
//		SOCKET serverAcceptSocket = {};
//		SOCKET clientAcceptSocket = {};
//
//	private:
//		Network() {}
//		~Network() {}
//	public:
//		Network(Network& n) = delete;
//		Network operator=(Network& n) = delete;
//		static Network* GetInstance();
//
//		bool Initialize();
//		void Finalize();
//
//#pragma region ��M��(�T�[�o�[)
//		NetWorkError StartServer(const USHORT portNum);
//#pragma endregion
//
//#pragma region ���M��
//		NetWorkError StartClient(const std::string& address, const USHORT portNum);
//#pragma endregion
//
//
//	};
//
//}