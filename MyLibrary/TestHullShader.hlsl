//�n���V�F�[�_�[������
//https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/direct3d-11-advanced-stages-hull-shader-design

//�e�b�Z���[�V�����X�e�[�W���
//https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/direct3d-11-advanced-stages-tessellation#hull-shader-stage

//�n���V�F�[�_�[�́A2��ޏo�͂���(�R���g���[���|�C���g�ƁA�p�b�`�萔�f�[�^)

#include"FbxShaderHeader.hlsli"

#include"TriTessellateHeader.hlsli"




//�p�b�`�萔�֐�
//EdgeTessFactor�Ȃǂ̕ϐ��̉���́A�Z�}���e�B�N�X�̉���ɏ�����Ă���
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	//�o�͂��v�Z����R�[�h(1����32?1�Œʏ�ʂ�)
	Output.EdgeTessFactor[0] = 1;
	Output.EdgeTessFactor[1] = 1;
	Output.EdgeTessFactor[2] = 1;
	Output.InsideTessFactor = 1;


	return Output;
}


//�n���V�F�[�_�[���`���Ă���
//partitioning�Ȃǂ̐����́A
//https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/direct3d-11-advanced-stages-tessellation
//�̃e�b�Z���[�^�X�e�[�W�̕����ɏ�����Ă���

//�h���C��(�n���V�F�[�_�[�Ŏg�p�����C���v���O����)"try"(3)"quad"(4)"isoline"(2)
//����͎O�p�`�P�ʂŕ������邽�߁A"try"(����R�}���h���X�g�ŃZ�b�g�����g�|���W�[�Ɋ֌W�Ȃ��ݒ�ł���?)
[domain(DOMEIN)]
//�e�b�Z���[�V�����X�L�[��(�������@) ��������ɂ���āAdomain�ƒ��_���ǂ̒��_�ƂȂ��邩�����܂�
[partitioning("integer")]
//�o�͂̃v���~�e�B�u(�o�͂����Ƃ��̌`��)
[outputtopology("triangle_cw")]
//�o�̓R���g���[���|�C���g�̐�(main�����s������)
[outputcontrolpoints(NUM_CONTROL_POINTS)]
//�p�b�`�萔�f�[�^���v�Z����֐���
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip, //���̓R���g���[���|�C���g(���_)
	uint i : SV_OutputControlPointID,//���_�ԍ�
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	Output.worldPos = ip[i].worldPos;
	Output.svpos = ip[i].svpos;
	Output.normal = ip[i].normal;
	Output.uv = ip[i].uv;

	return Output;
}



