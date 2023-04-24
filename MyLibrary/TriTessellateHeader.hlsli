
#define NUM_CONTROL_POINTS 3
#define DOMEIN "tri"


//�n���V�F�[�_�[�L�q

//�R���g���[���|�C���g = ���_?

//���̓R���g���[���|�C���g
struct VS_CONTROL_POINT_OUTPUT
{
	//float3 vPosition : WORLDPOS;


	//float4 worldPos:POSITION;
	//float4 svpos : SV_POSITION;
	float4 worldPos : POSITION;
	float4 svpos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv : TEXCOORD;
};

//Output����R���g���[���|�C���g
struct HS_CONTROL_POINT_OUTPUT
{
	//float3 vPosition : WORLDPOS;

	/*float4 worldPos :POSITION;
	float4 svpos : SV_POSITION;*/
	float4 worldPos : POSITION;
	float4 svpos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv: TEXCOORD;
};


//�o�̓p�b�`�萔�f�[�^
struct HS_CONSTANT_DATA_OUTPUT
{
	//�p�b�`�̊e���_�̃e�b�Z���[�V������(�ǂ̂��炢�������邩?)(�O���̕ӂ��ǂ̂��炢�������邩?)
	//[]���͒��_��?
	//�����ڂ����ׂ����������ꍇ�́A���������l��傫������΂���?
	float EdgeTessFactor[3] : SV_TessFactor;

	//�p�b�`�T�[�t�F�X���̃e�b�Z���[�V�����̗�(�|���S���̓����̕�����?)
	//�X�v���C�g���΂�΂�ɂ���Ƃ���g�����Ƃ��Ȃǂ̕��ׂ���������(�����B��)���ׂ����������Ƃ��͂���̒l��傫������?
	float InsideTessFactor : SV_InsideTessFactor;

};



//�h���C���V�F�[�_�[�L�q
//�h���C���V�F�[�_�[�̃A�E�g�v�b�g
struct DS_OUTPUT
{
	//float4 vPosition  : SV_POSITION;

	float4 worldPos :POSITION;
	float4 svpos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv: TEXCOORD;
};



