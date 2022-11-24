

#version 330 core
out vec4 FragColor;

in vec3 currentPos;

in vec3 normal;

// inputs the color from vertex shader
// must have same name as in the vertex shader
in vec3 color;

in vec2 texCoord;



// Textures
uniform sampler2D diffuse0;
uniform sampler2D specular0;


uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 PointLight()
{
	vec3 lightVec = lightPos - currentPos;
	float dist = length(lightVec);

	float a = 1.0f;
	float b = 0.04f;
	float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);


	float ambient = 0.2f;

	vec3 normalVec = normalize(normal);
	vec3 lightDirection = normalize(lightVec);

	float diffuse = max(dot(normalVec, lightDirection), 0.0f);
	float specularLight = 0.5f;

	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);

	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 2);
	float specular = specAmount * specularLight;
	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

vec4 DirectionalLight()
{
	vec3 lightVec = lightPos - currentPos;
	float dist = length(lightVec);

	float a = 1.0f;
	float b = 0.04f;
	float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);


	float ambient = 0.2f;

	vec3 normalVec = normalize(normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));

	float diffuse = max(dot(normalVec, lightDirection), 0.0f);
	float specularLight = 0.5f;

	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);

	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 2);
	float specular = specAmount * specularLight;
	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 SpotLight()
{
	// Cosine of angles
	float outerCone = 0.9f;
	float innerCone = 0.95f;

	float ambient = 0.2f;

	vec3 normalVec = normalize(normal);
	vec3 lightDirection = normalize(lightPos - currentPos);

	float diffuse = max(dot(normalVec, lightDirection), 0.0f);
	
	float specularLight = 0.5f;

	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);

	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 2);
	float specular = specAmount * specularLight;
	
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);



	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

vec4 rDirectionalLight()
{
	vec3 lightVec = lightPos - currentPos;
	float dist = length(lightVec);

	float a = 1.0f;
	float b = 0.04f;
	float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);


	// ambient
    float ambientStrength = 0.3;
    vec4 ambient = lightColor * ambientStrength;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - currentPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = lightColor * diff;

	return (diffuse + ambient) * vec4(color,1.0f) ;

}

void main()
{
	//FragColor = vec4(color,1.0);
	FragColor = rDirectionalLight();
	//FragColor = vec4(currentPos, 1.0);
	// FragColor = texture(diffuse0, texCoord) * (diffuse + ambient) * lightColor;
}
