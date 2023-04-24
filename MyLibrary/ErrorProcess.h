#pragma once
#include<string>
#include<Windows.h>
//�G���[���ɃE�B���h�E���o���A��������N���X
namespace MelLib 
{
	class ErrorProcess
	{
	private:
		ErrorProcess() {}
		~ErrorProcess() {}

		HWND errowMessageHWND = nullptr;
	public:
		ErrorProcess(ErrorProcess& e) = delete;
		ErrorProcess operator=(ErrorProcess& e) = delete;
		static ErrorProcess* GetInstance();

		/// <summary>
		/// �Ăяo���ƃG���[���N�������̏������s���܂��B
		/// </summary>
		/// <param name="text">�G���[���b�Z�[�W</param>
		/// <param name="assertFlag"></param>
		void StartErroeProcess(const std::wstring& text,const bool assertFlag);
	};
}

