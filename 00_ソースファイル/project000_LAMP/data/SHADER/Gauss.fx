// Gaussian Blur Shader

texture InputTexture; // ���̓e�N�X�`��
sampler2D InputSampler = sampler_state {
	Texture = <InputTexture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

float2 texelSize = 1.0 / float2(1280.0, 720.0); // ��ʂ̕��ƍ���

#define KERNEL_SIZE 9 // �J�[�l���T�C�Y
float weights[KERNEL_SIZE] = { 0.05, 0.09, 0.12, 0.15, 0.18, 0.15, 0.12, 0.09, 0.05 }; // �J�[�l���̏d��

																					   // �K�E�X�ڂ����̃s�N�Z���V�F�[�_�[
float4 PS_GaussianBlur(float2 texCoord : TEXCOORD0) : COLOR0
{
	float4 finalColor = 0.0;
	for (int i = 0; i < KERNEL_SIZE; ++i)
	{
		for (int j = 0; j < KERNEL_SIZE; ++j)
		{
			float2 offset = float2(i - KERNEL_SIZE / 2, j - KERNEL_SIZE / 2) * texelSize;
			finalColor += tex2D(InputSampler, texCoord + offset) * weights[i] * weights[j];
		}
	}
	return finalColor;
}

technique GaussianBlur
{
	pass P0
	{
		PixelShader = compile ps_3_0 PS_GaussianBlur();
	}
}