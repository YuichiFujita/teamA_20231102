float4x4 matWorld : world;			// ���[���h�ϊ��s��
float4x4 matCameraView : view;		// �J�����r���[�ϊ��s��
float4x4 matCameraProj : projection;	// �ˉe�ϊ��s��
float4x4 matLightView;				// ���C�g�r���[�ϊ��s��
float4x4 matLightProj;				// �ˉe�ϊ��s��
float4   m_Ambient = 0.0f;			//�F

sampler tex0 : register(s0);		// �I�u�W�F�N�g�̃e�N�X�`���[
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
   float4 ZCalcTex : TEXCOORD1;   	// Z�l�Z�o�p�e�N�X�`��
   float2 Tex       : TEXCOORD0;
   float4 Col : COLOR0;				// �f�B�t���[�Y�F
};


// ���_�V�F�[�_
VS_OUTPUT DepthBufShadow_VS( float4 Pos : POSITION , float3 Norm : NORMAL, float2 Tex : TEXCOORD0)
{
   VS_OUTPUT Out = (VS_OUTPUT)0;

   // ���ʂɃJ�����̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
   float4x4 mat;
   mat  = mul( matWorld, matCameraView );
   mat  = mul( mat, matCameraProj );
   Out.Pos = mul( Pos, mat );
   Out.Tex = Tex;
   // ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
   mat  = mul( matWorld, matLightView );
   mat  = mul( mat, matLightProj );
   Out.ZCalcTex = mul( Pos, mat );
   
   // �@���ƃ��C�g�̕������璸�_�̐F������
   //  �Z���Ȃ肷���Ȃ��悤�ɒ��߂��Ă��܂�
   float3 N = normalize( mul(Norm, matWorld) );
   float3 LightDirect = normalize( float3(matLightView._13,matLightView._23,matLightView._33) );
 /*  float3 L = -LightDirect.xyz;
   float3 N = normalize(Norm.xyz);*/
   Out.Col = max(m_Ambient, (dot(N, -LightDirect)*0.5f));
  /* Out.Col = float4(1.0, 1.0,1.0,1.0) * (0.3 + dot(N, -LightDirect)*(1-0.3f));
   Out.Col.a = 1.0f;*/


   return Out;
}


// �s�N�Z���V�F�[�_
float4 DepthBufShadow_PS( float4 Col : COLOR, float4 ZCalcTex : TEXCOORD1 , VS_OUTPUT In) : COLOR
{
   // ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
   float ZValue = ZCalcTex.z / ZCalcTex.w;

	if ((tex2D(tex0, In.Tex).r + tex2D(tex0, In.Tex).g + tex2D(tex0, In.Tex).b) != 0.0f)
	{
	Col = (In.Col * tex2D(tex0, In.Tex));
	}

	
   // �e�N�X�`�����W�ɕϊ�
   float2 TransTexCoord;
   TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
   TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;
   if (TransTexCoord.x >1.0f || TransTexCoord.x < 0.0f || TransTexCoord.y >1.0f || TransTexCoord.y < 0.0f)
   {
	   return Col;
   }
   float4 TexCol = tex2D(DefSampler, TransTexCoord);
   // �������W��Z�l�𒊏o
   float SM_Z = (TexCol.x + (TexCol.y +(TexCol.z /256.0f)/256.0f) /256.0f);
  // color.r + (color.g + (color.b + color.a / 256.0f) / 256.0f) / 256.0f;
   // �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
   if( ZValue > SM_Z+0.00005f ){
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
