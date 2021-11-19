#version 330 core
struct Material{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
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
	mat3 TBN;
} fs_in;

out vec4 FragColor;

uniform vec3 viewPos;

/*
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}
*/

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);


	float diff = max(dot(normal, lightDir), 0.0);

	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constantAtt + light.linearAtt * distance + light.quadraticAtt * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
	vec3 specular = light.specular * spec; // Disabled specular mapping since I want to see the full effect
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

/*

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - FragPos);

	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = (light.cutOff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	diffuse *= intensity;
	specular *= intensity;

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constantAtt + light.linearAtt * distance + light.quadraticAtt * (distance * distance));
	ambient *= attenuation;
	specular *= attenuation;
	diffuse *= attenuation;

	return (ambient + diffuse + specular);
}
*/

void main() {
	vec3 norm = texture(material.normal, fs_in.TexCoords).rgb;
	norm = norm * 2.0 - 1.0;
	norm = normalize(fs_in.TBN * norm);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	// phase 1 : Point Lights
	vec3 result = CalcPointLight(light, norm, fs_in.FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}
