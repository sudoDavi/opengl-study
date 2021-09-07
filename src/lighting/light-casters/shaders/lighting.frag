#version 330 core
struct Material{
	sampler2D diffuse;
	sampler2D specular;
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
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform	SpotLight spotLight;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

void main() {
	// Useful variables
	vec3 FragmentToLight = spotLight.position - FragPos;

	// Light Attenuation
	float distance = length(FragmentToLight);
	float attenuation = 1.0 / 
		(spotLight.constantAtt + spotLight.linearAtt * distance + spotLight.quadraticAtt * (distance * distance));

	// Ambient Lighting
	vec3 ambient = attenuation * spotLight.ambient * vec3(texture(material.diffuse, TexCoords));

	// Diffuse Lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(FragmentToLight);
	// Spotlight calc
	float theta = dot(lightDir, normalize(-spotLight.direction));
	float epsilon = spotLight.cutOff - spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = attenuation * spotLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// Specular Lighting
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = attenuation * spotLight.specular * spec *  vec3(texture(material.specular, TexCoords));

	diffuse *= intensity;
	specular *= intensity;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
