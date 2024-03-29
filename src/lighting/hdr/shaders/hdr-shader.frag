#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform float exposure;
uniform bool hdr;

const float gamma = 2.2;

void main()
{
	vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
	if(hdr)
	{
		// reinhard tone mapping
		vec3 result = hdrColor / (hdrColor + vec3(1.0));
		// gamma correction
		result = pow(result, vec3(1.0 / gamma));
		FragColor = vec4(result, 1.0);
	}
	else
	{
		vec3 result = pow(hdrColor, vec3(1.0 / gamma));
		FragColor = vec4(result, 1.0);
	}
}
