float4x4 matWorld : world;		// ���[���h�ϊ��s��
float4x4 matView : view;		// �r���[�ϊ��s��
float4x4 matProj : projection;		// �ˉe�ϊ��s��

struct VS_OUTPUT
{
  float4 Pos : POSITION;   // �ˉe�ϊ����W
   float4 ShadowMapTex : TEXCOORD0;   // Z�o�b�t�@�e�N�X�`��
};

// ���_�V�F�[�_
VS_OUTPUT ZBufferCalc_VS( float4 Pos : POSITION )
{
   VS_OUTPUT Out = (VS_OUTPUT)0;

   // ���ʂɃ��[���h�r���[�ˉe�s�������
   float4x4 mat;
   mat  = mul( matWorld, matView );
   mat  = mul( mat, matProj );
   Out.Pos = mul( Pos, mat );
   
   // �e�N�X�`�����W�𒸓_�ɍ��킹��
   Out.ShadowMapTex = Out.Pos;

   return Out;
}


// �s�N�Z���V�F�[�_
float4 ZBufferPlot_PS( float4 ShadowMapTex : TEXCOORD0 ) : COLOR
{
// �[�x�Z�o
float depth = ShadowMapTex.z / ShadowMapTex.w;
float4 unpacked_depth = float4(0, 0, 256.0f, 256.0f);
unpacked_depth.g = modf(depth*256.0f, unpacked_depth.r);
unpacked_depth.b *= modf(unpacked_depth.g*256.0f, unpacked_depth.g);
unpacked_depth /= 256.0f;
unpacked_depth.w = depth;

return unpacked_depth;  // �W����
}

technique ZValuePlotTec
{
   pass P0
   {
      VertexShader = compile vs_2_0 ZBufferCalc_VS();
      PixelShader = compile ps_2_0 ZBufferPlot_PS();
   }
}
