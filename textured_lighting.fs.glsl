#version 330 core

// object material texture for lighting
struct Material {
    sampler2D diffuse;		// texture for diffuse lighting (also used for ambient lighting)
    sampler2D specular;		// texture for specular lighting
    float     shininess;	// shininess coefficient
};  

// light source properties
struct Light {
    
    // position of light
    vec3 position;

	// ambient component of light
    float ka;		// coefficient  (same for R,G,B)
    vec3 ambient;
    
    float kd;		// reflection coefficient (same for R,G,B)
    vec3 diffuse;
    
    float ks;		// absorption coefficient (same for R,G,B)
    vec3 specular;
};

in vec3 fragPos;  
in vec3 vNormal;  
in vec2 vTexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

out vec4 color;

void main()
{   
	// Phong Lighting Model: ambient + diffuse + specular
	 
    // 1. Ambient
    vec3 ambient = light.ka*light.ambient * vec3(texture(material.diffuse, vTexCoords));
  	
    // 2. Diffuse 
    vec3 normN = normalize(vNormal);	// normalize to unit length (1.0)
    vec3 lightDir = normalize(light.position - fragPos);	// compute light direction I
    float In = max(dot(normN, lightDir), 0.0);	// I*n
    vec3 diffuse = light.kd*light.diffuse * In * vec3(texture(material.diffuse, vTexCoords));
    
    // 3. Specular
    vec3 viewDir = normalize(viewPos - fragPos);	// view/camera direction
    vec3 reflectDir = reflect(-lightDir, normN);		// reflection direction
    float cosPhi = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);		// cos(Phi)^alpha
    //vec3 specular = light.ks*light.specular * cosPhi * vec3(texture(material.specular, vTexCoords));	// normally, we should use specular
    vec3 specular = light.ks*light.specular * cosPhi * vec3(texture(material.diffuse, vTexCoords));
        
    color = vec4(ambient + diffuse + specular, 1.0f);   // output color of the pixel    
}







