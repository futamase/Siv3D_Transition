struct VS_OUTPUT {
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
  float4 color : COLOR0;
};

cbuffer psConstants1 : register(b0) {
  float4 _Range;
};

cbuffer psConstants2 : register(b1)
{
	float4 _Color;
}

SamplerState sampler0 : register(s0);
Texture2D tex : register(t0);
Texture2D mask : register(t1);

float4 PS(VS_OUTPUT input) : SV_Target{
  float4 color = mask.Sample(sampler0, input.tex);
  float4 mainColor = _Color;

  color.r -= _Range.x;

  mainColor.a = color.r;// *color.r *color.r;

  return mainColor;
}
