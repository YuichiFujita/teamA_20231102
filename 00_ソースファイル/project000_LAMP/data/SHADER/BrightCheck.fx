sampler2D inputTexture : register(s0);

float threshold = 0.75; // �������l

float4 main(float2 texCoord : TEXCOORD0) : COLOR
{
	// �e�N�X�`������s�N�Z���̐F���T���v�����O
	float4 color = tex2D(inputTexture, texCoord);

	// �s�N�Z���̋P�x���v�Z
	float luminance = dot(color.rgb, float3(0.299, 0.587, 0.114));

	// �������l�𒴂���ꍇ�͌��̐F��ێ����A����ȊO�̏ꍇ�͐F��h��Ԃ�
	if (luminance >= threshold)
	{
		// �F�̖��邳�𔼕��ɂ��ĐV�����F�𐶐�
		float4 newColor = color;
		return newColor;
	}
	else
	{
		return float4(0.0, 0.0, 0.0, 1.0); // �s�N�Z����h��Ԃ� (���F)
	}
}

technique brightcheck
{
	pass Pass1
	{
		PixelShader = compile ps_2_0 main();
	}
}