#pragma once
namespace MelLib 
{
	//�萔�o�b�t�@�̏��
	struct ConstBufferData
	{
		enum class BufferType
		{
			BUFFER_TYPE_NONE,//�������Ȃ�
			BUFFER_TYPE_COMMON,//�S���f������
			BUFFER_TYPE_EACH_MODEL,//�e���f������
			BUFFER_TYPE_EACH_MODEL_OBJECT,//�e���f���e�I�u�W�F�N�g����
		};

		BufferType bufferType = BufferType::BUFFER_TYPE_NONE;//�o�b�t�@�̐�
		size_t bufferDataSize = 0;//�o�b�t�@1�̃T�C�Y
	};
}