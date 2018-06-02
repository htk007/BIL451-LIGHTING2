#ifndef _LIGHT_H_
#define _LIGHT_H_

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light
{
    public:
        Light()
        {
            this->position = glm::vec3(0.0f, 10.0f, 3.0f);

            this->ka = 0.3f;
            this->ambient = glm::vec3(1.0f, 1.0f, 1.0f);    // white light

            this->kd = 0.9f;
            this->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);    // white light

            this->ks = 0.3f;
            this->specular = glm::vec3(1.0f, 1.0f, 1.0f);    // white light

        }
        Light(glm::vec3 position, float ka, glm::vec3 ambient, float kd, glm::vec3 diffuse, float ks, glm::vec3 specular)
        {
            this->position = position;

            this->ka = ka;
            this->ambient = ambient;    // white light

            this->kd = kd;
            this->diffuse = diffuse;    // white light

            this->ks = ks;
            this->specular = specular;    // white light

        }

        // send light attributes to fragment shader
        void send2shader(GLuint program)
        {
            glUniform3fv(glGetUniformLocation(program, "light.position"), 1, glm::value_ptr(position));

            glUniform1f(glGetUniformLocation(program, "light.ka"), ka);
            glUniform3fv(glGetUniformLocation(program, "light.ambient"), 1, glm::value_ptr(ambient));

            glUniform1f(glGetUniformLocation(program, "light.kd"), kd);
            glUniform3fv(glGetUniformLocation(program, "light.diffuse"), 1, glm::value_ptr(diffuse));

            glUniform1f(glGetUniformLocation(program, "light.ks"), ks);
            glUniform3fv(glGetUniformLocation(program, "light.specular"), 1, glm::value_ptr(specular));
        }

    // position of light
    glm::vec3 position;

    // ambient
    float ka;   // coefficient  (same for R,G,B)
    glm::vec3 ambient; // R,G,B

    // diffuse
    float kd;   // reflection coefficient (same for R,G,B)
    glm::vec3 diffuse; // R,G,B

    // specular
    float ks;    // absorption coefficient (same for R,G,B)
    glm::vec3 specular; // R,G,B
};


#endif // _LIGHT_H_
