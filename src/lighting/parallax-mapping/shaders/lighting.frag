#version 330 core
struct Material{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	sampler2D parallax;
	float shininess;
};

struct DirectionalLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float linearAtt;
	float constantAtt;
	float quadraticAtt;
};

struct SpotLight{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constantAtt;
	float linearAtt;
	float quadraticAtt;
};

uniform Material material;
//uniform DirectionalLight directionalLight;
uniform PointLight light;
//uniform SpotLight spotLight;

in VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} fs_in;

out vec4 FragColor;

uniform vec3 viewPos;

uniform float height_scale;


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTexture) {
	vec3 lightDir = normalize(light.position - fragPos);


	float diff = max(dot(normal, lightDir), 0.0);

	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constantAtt + light.linearAtt * distance + light.quadraticAtt * (distance * distance));

	vec3 ambient = light.ambient * diffuseTexture;
	vec3 diffuse = light.diffuse * diff * diffuseTexture;
	vec3 specular = light.specular * spec; // Disabled specular mapping since I want to see the full effect
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}



vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

void main() {
	// offset texture coordinates with Parallax Mapping
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec2 texCoords = ParallaxMapping(fs_in.TexCoords, viewDir);

	// then sample textures with new texture coords
	vec3 diffuseTexture = texture(material.diffuse, texCoords).rgb;
	vec3 norm = texture(material.normal, texCoords).rgb;
	norm = normalize(norm * 2.0 - 1.0);

	// phase 1 : Point Lights
	vec3 result = CalcPointLight(light, norm, fs_in.FragPos, viewDir, diffuseTexture);

	FragColor = vec4(result, 1.0);
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
	float height = texture(material.parallax, texCoords).r;
	vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
	return texCoords - p;
}
