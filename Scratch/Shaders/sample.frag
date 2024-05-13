#version 330 core //Version
//USING POINT LIGHT

//texture to be passed
uniform sampler2D tex0;

uniform vec3 lightPos;

uniform vec3 lightColor;

uniform float ambientStr;

uniform vec3 ambientColor;

uniform vec3 cameraPos;

uniform float specStr;

uniform float specPhong;

//should receive the texCoord from the vertex shader
in vec2 texCoord;

in vec3 normCoord;

in vec3 fragPos;

out vec4 FragColor; //Returns a Color

//Simple shader that colors the model Red

void main()
{
	//				  r    g   b   a   Ranges from 0->1
	//FragColor = vec4(0.7f, 0f, 0f, 1f); //Sets the color of the Fragment

	vec3 normal = normalize(normCoord);

	vec3 lightDir = normalize(lightPos - fragPos);

	float distance = length(lightPos - fragPos);
	//distance is the length between the position of the light to the fragment 

	float constant = 1.0f; //constant

	float linear = 0.007f; // linear

	float quadratic = 0.0002f; //quadratic

	float attenuation = 1.0f / (constant + linear * distance + quadratic * (distance * distance));
	//attenuation is the computation of the decrease in intensity of light in certain distances

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 ambientCol = ambientColor * ambientStr;

	vec3 diffuse = diff * lightColor;	

	vec3 viewDir = normalize(cameraPos - fragPos);

	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);

	vec3 specColor = spec * specStr * lightColor;

	ambientCol *= attenuation;

	diffuse *= attenuation;

	specColor *= attenuation;  //multiplying the calculated decrease in intensity to the calculation of the lighting 

	//Assigning the texture color using the function
	FragColor = vec4((specColor + diffuse + ambientCol), 1.0) * texture(tex0, texCoord);
}