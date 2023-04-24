#include"FbxShaderHeader.hlsli"
#include"TriTessellateHeader.hlsli"

//�e�b�Z���[�V�����R����
//https://game.watch.impress.co.jp/docs/series/3dcg/590707.html

//�o�͂��ꂽ�_(�R���g���[���|�C���g)��������V�F�[�_�[
//�����Ő܂�Ȃ��鏈�����������邱�Ƃɂ���āA���[�|�����f�����ׂ������炩�ȃ��f���ɂ��邱�Ƃ��ł���


//�e�b�Z���[�^�X�e�[�W����o�͂��ꂽ���_���ƂɌĂяo�����
[domain(DOMEIN)]//�h���C������
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,//UV���W�̂���? try����float3(UVW) ���̍��W�́A�|���S����̂ǂ̕ӂɂ��邩������
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	//���̕���
	static const float3 PLANE_POS = float3(0, 1, 0);


	//��肭���ΎO�p�`�ł��l�p�`�Ɠ����悤�Ȃ��ꂢ�ȕ������ł���͂�

	DS_OUTPUT Output;

	//p�́A�e�b�Z���[�^�ɓn�������_(�R���g���[���|�C���g)�ƁA�h���C��(�|���S����UV(W)���W)���狁�߂����_���W

	//�O�p�`�̏ꍇ�́Auvw�S����0�ɂȂ邱�Ƃ͂Ȃ�?
	//�l�p�`���Ƃ���?���邩�� patch[0].pos * domain.x + patch[0].pos * domain.y�̎��Ō��_�ɒ��_�z�u�����?
	float4 p = patch[0].svpos * domain.x + patch[1].svpos * domain.y + patch[2].svpos * domain.z;
	
	//p.y = p.y >= PLANE_POS.y ? PLANE_POS.y : p.y;
	
	Output.svpos = p;
	
	//partitioning�Œ��_���ǂ̒��_�ƂȂ��邩�����܂邩�牺�̎��ł͖���
	/*float3 p0ToP1 = patch[1].pos.xyz - patch[0].pos.xyz;
	float3 p1ToP2 = patch[2].pos.xyz - patch[1].pos.xyz;
	float3 p2ToP0 = patch[0].pos.xyz - patch[2].pos.xyz;
	float4 p = 
		float4(patch[0].pos.xyz + p0ToP1 * domain.x
	     + (p1ToP2 * domain.y + p0ToP1 * domain.y)
	     + (p2ToP0 * domain.z + -p2ToP0 * domain.z)
			, 1);*/

	//�������quad�ł����@
	//���܂������Ȃ�����
	//float3 p0ToP1 = patch[1].pos.xyz - patch[0].pos.xyz;
	//float3 p1ToP2 = patch[2].pos.xyz - patch[1].pos.xyz;
	//float4 p = float4(patch[0].pos.xyz + p0ToP1 * domain.y
	//	+ (p1ToP2 * domain.x + p0ToP1 * (1 - domain.y) * domain.x)
	//	, 1);


	//Output.worldPos = mul(worldMat, p);
	//Output.svpos = mul(mat, p);

	Output.uv = patch[0].uv * domain.x + patch[1].uv * domain.y + patch[2].uv * domain.z;


	p = patch[0].worldPos * domain.x + patch[1].worldPos * domain.y + patch[2].worldPos * domain.z;
	
	Output.worldPos = p;

	//�܂�Ȃ��Ȃ�����@���͂��̂܂�
	Output.normal = patch[0].normal;

	

	return Output;
}






