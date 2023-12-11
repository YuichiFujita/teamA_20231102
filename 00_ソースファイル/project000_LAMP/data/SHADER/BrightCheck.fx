sampler2D inputTexture : register(s0);

float threshold = 0.75; // しきい値

float4 main(float2 texCoord : TEXCOORD0) : COLOR
{
	// テクスチャからピクセルの色をサンプリング
	float4 color = tex2D(inputTexture, texCoord);

	// ピクセルの輝度を計算
	float luminance = dot(color.rgb, float3(0.299, 0.587, 0.114));

	// しきい値を超える場合は元の色を保持し、それ以外の場合は色を塗りつぶす
	if (luminance >= threshold)
	{
		// 色の明るさを半分にして新しい色を生成
		float4 newColor = color;
		return newColor;
	}
	else
	{
		return float4(0.0, 0.0, 0.0, 1.0); // ピクセルを塗りつぶす (黒色)
	}
}

technique brightcheck
{
	pass Pass1
	{
		PixelShader = compile ps_2_0 main();
	}
}