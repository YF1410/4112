//ModelData�Ȃ��ƃq�[�v�p�ӂł��Ȃ�����A
//�}�e���A���Ƃ��̃R�s�[�R���X�g���N�^������ق�����������





//#pragma once
//#include<vector>
//#include<d3d12.h>
//#include<d3dx12.h>
//
//#include"DirectXStruct.h"
//
//namespace MelLib 
//{
//	//���f���̒��_�A�C���f�b�N�X���܂Ƃ߂��N���X
//	class ModelVertexData
//	{
//	private:
//		//���_
//		std::vector<std::vector<FbxVertex>>vertices;
//		std::vector<VertexBufferSet> vertexBufferSet;
//
//		//�C���f�b�N�X
//		std::vector <IndexBufferSet> indexBufferSet;
//		std::vector<std::vector<USHORT>>indices;
//
//
//		void MapVertices(const std::vector<FbxVertex>& vertices, const int bufferNum);
//		void MapVertices(void** data, const int bufferNum);
//		void UnmapVertices(const int bufferNum);
//
//		void MapIndices(const std::vector<USHORT>& indices, const int bufferNum);
//		void UnmapIndices(const int bufferNum);
//	public:
//		ModelVertexData(const std::vector<std::vector<FbxVertex>>& vertices, const std::vector<std::vector<USHORT>>& indices);
//
//		std::vector<std::vector<FbxVertex>> GetVertices()const { return vertices; }
//		std::vector<std::vector<USHORT>> GetIndices()const { return indices; }
//
//
//	};
//}
//
