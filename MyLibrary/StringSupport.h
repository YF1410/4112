#pragma once
#include<string>

namespace MelLib 
{
	// String�Ɋ֌W����֗��֐����������N���X
	class StringSupport
	{
	public:
		/// <summary>
		/// �n�����p�X����t�@�C�����𔲂��o���܂��B
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static std::string ExtractionFileName(const std::string& path);

		static std::wstring ExtractionFileName(const std::wstring& path);

	};
}

