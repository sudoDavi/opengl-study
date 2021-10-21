#version 330 core

uniform bool blinn;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform sampler2D floorTexture;

void main() {
	vec3 color = texture(floorTexture, TexCoords).rgb;
	// ambient
	vec3 ambient = 0.05 * color;
	// diffuse
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 normal = normalize(Normal);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	if(blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);  
		spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
	}
	vec3 specular = vec3(0.3) * spec; // assuming bright white light color
	FragColor = vec4(ambient + diffuse + specular, 1.0);
}
