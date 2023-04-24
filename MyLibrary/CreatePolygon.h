//#pragma once
//#include<vector>
//#include<DirectXMath.h>
//#include"Vector.h"
//#include"DirectXStruct.h"
////�|���S���̒��_����A�C���f�b�N�X���擾���邽�߂̃N���X
//class CreatePolygon final
//{
//	using XMFLOAT3 = DirectX::XMFLOAT3;
//	using XMFLOAT2 = DirectX::XMFLOAT2;
//
//
//private:
//	int windowWidth;
//	int windowHeight;
//
//	CreatePolygon();
//	~CreatePolygon();
//public:
//	CreatePolygon(CreatePolygon& c) = delete;
//	CreatePolygon operator= (CreatePolygon& c) = delete;
//	static CreatePolygon* GetInstance();
//
//	void Initialize(const int& winWidth, const int& winHeight);
//
//#pragma region �����g�������p�`�쐬
//	/// <summary>
//	/// �����g���Ē��_��ݒ肵�܂�(���p�`�����)
//	/// </summary>
//	/// <param name="r"></param>
//	/// <param name="vertexNum">���S�����������_��</param>
//	/// <param name="tyusinX">���S���WX</param>
//	/// <param name="tyusinY">���S���WY</param>
//	/// <returns></returns>
//	std::vector<Vertex> SetPolygonVertex( float r,  int vertexNum, XMFLOAT2 tyusin);
//	
//	/// /// <summary>
//	/// �C���f�b�N�X��ݒ肵�܂�
//	/// </summary>
//	/// <param name="vertexNum">���S�����������_��</param>
//	/// <returns></returns>
//	std::vector<unsigned short> SetPolygonIndex(int vertexNum);
//
//#pragma region �O�p��
//
//	/// <summary>
//	/// �����g���Ē��_��ݒ肵�܂�
//	/// </summary>
//	/// <param name="r"></param>
//	/// <param name="vertexNum">���S�����������_��</param>
//	/// <param name="tyusinX">���S���WX</param>
//	/// <param name="tyusinY">���S���WY</param>*	/// <returns></returns>
//	std::vector<Vertex> GetTriangularPyramidVertex(float r, int vertexNum, XMFLOAT3 tyusin,float up);
//
//	/// /// <summary>
//	/// �C���f�b�N�X��ݒ肵�܂�
//	/// </summary>
//	/// <param name="vertexNum">���S�����������_��</param>
//	/// <returns></returns>
//	std::vector<unsigned short> SetTriangularPyramidIndex(int vertexNum);
//#pragma endregion
//
//#pragma region �����p��
//	std::vector<Vertex> SetPolygonalPrismVertex(float r, int vertexNum, XMFLOAT3 tyusin, float up);
//
//	std::vector<unsigned short> SetPolygonalPrismIndex(int vertexNum);
//
//
//#pragma endregion
//
//#pragma region ��
//
//#pragma endregion
//
//
//#pragma endregion
//
//
//#pragma region �X���b�V���|���S��
//
//	std::vector<Vertex> SetSlushPolygonVertex(XMFLOAT2 tyusin, XMFLOAT2 pos,int type);
//	std::vector<unsigned short> SetSlushPolygonIndex(int type);
//
//#pragma endregion
//
//#pragma region �l�p�`
//	//2D3D�Ƃ���Board�̂ق����g���Ă܂�
//
//#pragma region �l�p�`2D�|���S��
//	/// <summary>
//	/// �l�p�`�̒��_��ݒ肵�܂�
//	/// </summary>
//	/// <param name="startPosX">������WX</param>
//	/// <param name="startPosY">������WY</param>
//	/// <param name="endPosX">�E�����WX</param>
//	/// <param name="endPosY">�E�����WY</param>
//	/// <returns></returns>
//	std::vector<Vertex> SetBoxPolygonVertex(XMFLOAT2 startPos, XMFLOAT2 endPos);
//	
//	/// <summary>
//	/// �C���f�b�N�X��ݒ肵�܂�
//	/// </summary>
//	/// <returns></returns>
//	std::vector<unsigned short> SetBoxPolygonIndex();
//
//
//
//#pragma endregion
//
//#pragma region 4���_�|���S��
//	std::vector<Vertex> SetBoardPolygonVertex(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3, XMFLOAT3 pos4);
//	std::vector<unsigned short> SetBoardPolygonIndex();
//
//
//
//#pragma endregion
//
//#pragma region ��
//	std::vector<Vertex> Get3DBox(DirectX::XMFLOAT3 size);
//	std::vector<unsigned short>  Get3DBoxIndex();
//
//	//���_�����������Auv�������ƁA���R�ɐݒ�ł���
//	std::vector<Vertex> GetVertexMany3DBox(DirectX::XMFLOAT3 size);
//	std::vector<unsigned short>  GetVertexMany3DBoxIndex();
//#pragma endregion
//
//
//#pragma endregion
//
//
//
//};
//
