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
   // Z�l�Z�o
   return ShadowMapTex.z / ShadowMapTex.w;
}

technique ZValuePlotTec
{
   pass P0
   {
      VertexShader = compile vs_2_0 ZBufferCalc_VS();
      PixelShader = compile ps_2_0 ZBufferPlot_PS();
   }
}
