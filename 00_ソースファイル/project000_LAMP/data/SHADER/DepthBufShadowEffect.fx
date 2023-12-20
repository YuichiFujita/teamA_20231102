float4x4 matWorld : world;			// ワールド変換行列
float4x4 matCameraView : view;		// カメラビュー変換行列
float4x4 matCameraProj : projection;	// 射影変換行列
float4x4 matLightView;				// ライトビュー変換行列
float4x4 matLightProj;				// 射影変換行列
float4   m_Ambient = 0.0f;			//色

sampler tex0 : register(s0);		// オブジェクトのテクスチャー
texture texShadowMap;				// シャドウマップテクスチャ
sampler DefSampler = sampler_state	// サンプラーステート
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
   float4 Pos : POSITION;  			 // 射影変換座標
   float4 ZCalcTex : TEXCOORD1;   	// Z値算出用テクスチャ
   float2 Tex       : TEXCOORD0;
   float4 Col : COLOR0;				// ディフューズ色
};


// 頂点シェーダ
VS_OUTPUT DepthBufShadow_VS( float4 Pos : POSITION , float3 Norm : NORMAL, float2 Tex : TEXCOORD0)
{
   VS_OUTPUT Out = (VS_OUTPUT)0;

   // 普通にカメラの目線によるワールドビュー射影変換をする
   float4x4 mat;
   mat  = mul( matWorld, matCameraView );
   mat  = mul( mat, matCameraProj );
   Out.Pos = mul( Pos, mat );
   Out.Tex = Tex;
   // ライトの目線によるワールドビュー射影変換をする
   mat  = mul( matWorld, matLightView );
   mat  = mul( mat, matLightProj );
   Out.ZCalcTex = mul( Pos, mat );
   
   // 法線とライトの方向から頂点の色を決定
   //  濃くなりすぎないように調節しています
   float3 N = normalize( mul(Norm, matWorld) );
   float3 LightDirect = normalize( float3(matLightView._13,matLightView._23,matLightView._33) );
 /*  float3 L = -LightDirect.xyz;
   float3 N = normalize(Norm.xyz);*/
   Out.Col = max(m_Ambient, (dot(N, -LightDirect)*0.5f));
  /* Out.Col = float4(1.0, 1.0,1.0,1.0) * (0.3 + dot(N, -LightDirect)*(1-0.3f));
   Out.Col.a = 1.0f;*/


   return Out;
}


// ピクセルシェーダ
float4 DepthBufShadow_PS( float4 Col : COLOR, float4 ZCalcTex : TEXCOORD1 , VS_OUTPUT In) : COLOR
{
   // ライト目線によるZ値の再算出
   float ZValue = ZCalcTex.z / ZCalcTex.w;

	if ((tex2D(tex0, In.Tex).r + tex2D(tex0, In.Tex).g + tex2D(tex0, In.Tex).b) != 0.0f)
	{
	Col = (In.Col * tex2D(tex0, In.Tex));
	}

	
   // テクスチャ座標に変換
   float2 TransTexCoord;
   TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
   TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;
   if (TransTexCoord.x >1.0f || TransTexCoord.x < 0.0f || TransTexCoord.y >1.0f || TransTexCoord.y < 0.0f)
   {
	   return Col;
   }
   float4 TexCol = tex2D(DefSampler, TransTexCoord);
   // 同じ座標のZ値を抽出
   float SM_Z = (TexCol.x + (TexCol.y +(TexCol.z /256.0f)/256.0f) /256.0f);
  // color.r + (color.g + (color.b + color.a / 256.0f) / 256.0f) / 256.0f;
   // 算出点がシャドウマップのZ値よりも大きければ影と判断
   if( ZValue > SM_Z+0.00005f ){
     Col.rgb = Col.rgb * 0.5f; 
    }

   
   return Col;
}


// テクニック
technique DepthBufShadowTec
{
   pass P0
   {
      VertexShader = compile vs_2_0 DepthBufShadow_VS();
      PixelShader = compile ps_2_0 DepthBufShadow_PS();
   }
}
