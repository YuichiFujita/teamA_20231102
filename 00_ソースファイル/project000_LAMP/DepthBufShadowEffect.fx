float4x4 matWorld : world;			// ���[���h�ϊ��s��
float4x4 matCameraView : view;		// �J�����r���[�ϊ��s��
float4x4 matCameraProj : projection;	// �ˉe�ϊ��s��
float4x4 matLightView;				// ���C�g�r���[�ϊ��s��
float4x4 matLightProj;				// �ˉe�ϊ��s��

texture texShadowMap;				// �V���h�E�}�b�v�e�N�X�`��
sampler DefSampler = sampler_state	// �T���v���[�X�e�[�g
{
    texture = (texShadowMap);
    AddressU  = CLAMP;        
    AddressV  = CLAMP;
    AddressW = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};


struct VS_OUTPUT
{
   float4 Pos : POSITION;  			 // �ˉe�ϊ����W
   float4 ZCalcTex : TEXCOORD0;   	// Z�l�Z�o�p�e�N�X�`��
   float4 Col : COLOR0;				// �f�B�t���[�Y�F
};


// ���_�V�F�[�_
VS_OUTPUT DepthBufShadow_VS( float4 Pos : POSITION , float3 Norm : NORMAL)
{
   VS_OUTPUT Out = (VS_OUTPUT)0;

   // ���ʂɃJ�����̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
   float4x4 mat;
   mat  = mul( matWorld, matCameraView );
   mat  = mul( mat, matCameraProj );
   Out.Pos = mul( Pos, mat );
   
   // ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
   mat  = mul( matWorld, matLightView );
   mat  = mul( mat, matLightProj );
   Out.ZCalcTex = mul( Pos, mat );
   
   // �@���ƃ��C�g�̕������璸�_�̐F������
   //  �Z���Ȃ肷���Ȃ��悤�ɒ��߂��Ă��܂�
   float3 N = normalize( mul(Norm, matWorld) );
   float3 LightDirect = normalize( float3(matLightView._13,matLightView._23,matLightView._33) );
   Out.Col = float4(0.0,0.6,1.0,1.0) * (0.3 + dot(N, -LightDirect)*(1-0.3f));
   
   return Out;
}


// �s�N�Z���V�F�[�_
float4 DepthBufShadow_PS( float4 Col : COLOR, float4 ZCalcTex : TEXCOORD0 ) : COLOR
{
   // ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
   float ZValue = ZCalcTex.z / ZCalcTex.w;
   
   // �e�N�X�`�����W�ɕϊ�
   float2 TransTexCoord;
   TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
   TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;
   
   // �������W��Z�l�𒊏o
   float SM_Z = tex2D( DefSampler, TransTexCoord ).x;
   
   // �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
   if( ZValue > SM_Z+0.005f ){
     Col.rgb = Col.rgb * 0.5f; 
    }
   
   return Col;
}


// �e�N�j�b�N
technique DepthBufShadowTec
{
   pass P0
   {
      VertexShader = compile vs_2_0 DepthBufShadow_VS();
      PixelShader = compile ps_2_0 DepthBufShadow_PS();
   }
}
