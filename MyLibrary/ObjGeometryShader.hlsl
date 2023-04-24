#include"ObjShaderHeader.hlsli"


[maxvertexcount(3)]//�ő咸�_��
void GSmain
(
	//triangle�̓v���~�e�B�u�̂���
	//[3]�́A�g���C�A���O�����X�g�A�X�g���b�v�Ƃ�������
	triangle VSOutput input[3],
	inout TriangleStream< GSOutput > output
)
{

	GSOutput element;

	for (uint i = 0; i < 3; i++)
	{
		element.worldPos = input[i].worldPos;
		element.svpos = input[i].svpos + float4(input[i].normal  * ex, 0);
		element.normal = input[i].normal;
		element.uv = input[i].uv;
		output.Append(element);
	}



	output.RestartStrip();

}