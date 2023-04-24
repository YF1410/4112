#include"FbxShaderHeader.hlsli"
#include"QuadTessellateHeader.hlsli"

//�e�b�Z���[�V�����R����
//https://game.watch.impress.co.jp/docs/series/3dcg/590707.html

//�o�͂��ꂽ�_(�R���g���[���|�C���g)��������V�F�[�_�[
//�����Ő܂�Ȃ��鏈�����������邱�Ƃɂ���āA���[�|�����f�����ׂ������炩�ȃ��f���ɂ��邱�Ƃ��ł���


//�e�b�Z���[�^�X�e�[�W����o�͂��ꂽ���_���ƂɌĂяo�����
[domain(DOMEIN)]//�h���C������
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float2 domain : SV_DomainLocation,//UV���W�̂���? try����float3(UVW) ���̍��W�́A�|���S����̂ǂ̕ӂɂ��邩������
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;


	//���ʂ���(�������|���̖@���̋t�x�N�g���ɂȂ�悤��)���āA�������獶��A��������E���̃x�N�g�������߂�
	float3 posVY = patch[1].pos.xyz - patch[0].pos.xyz;
	float3 posVX = patch[2].pos.xyz - patch[0].pos.xyz;

	//�h���C�����|����(0�ō����A1�Ńx�N�g���̏I����)
	float4 p = float4(patch[0].pos.xyz + posVY * domain.y + posVX * domain.x, 1);

	Output.worldPos = mul(worldMat, p);
	Output.svpos = mul(mat, p);

	float2 uvVY = patch[1].uv.xy - patch[0].uv.xy;
	float2 uvVX = patch[2].uv.xy - patch[0].uv.xy;
	Output.uv = float2(patch[0].uv.xy + uvVY * domain.y + uvVX * domain.x);
	//�܂�Ȃ��Ȃ�����@���͂��̂܂�
	Output.normal = patch[0].normal;

	return Output;
}
