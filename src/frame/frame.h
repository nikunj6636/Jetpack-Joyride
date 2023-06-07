#ifndef FRAME_H
#define FRAME_H

#include "../src/headers.h"

#include "../include/shader.h"
#include "../include/texture.h"

class Frame{    // public data
public: 
    float vertices[30] = {
        // positions          // colors           // texture coords         
         1.0f,  1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f,  -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f,  -1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[6] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO, VBO, EBO;

    Frame(){
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    void drawBg(){
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void clear(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};

class Level{
private:

    float x1 = 0, x2 = 2, speed;
    int level_no;

    Shader ourShader{"../src/vertex.shader", "../src/fragment.shader"};

    string path = "../src/level_images/level" + to_string(level_no) + ".jpg";
    Texture ourTexture{path.c_str()};

    Frame block; // default constructor with empty parentheses works here

public:
    Level(float frame_speed, int level_n): level_no(level_n){
        speed = frame_speed; // global speed of the frame/game
    }

    void render(){
        ourTexture.use(); // use the shader
        ourShader.use();
        
        // 1st block
        x1 -= speed;
        if (x1 <= -2) x1 += 4;

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(x1, 0.0f, 0.0f));
        ourShader.setMat4("trans", trans);

        block.drawBg();

        // 2nd block
        x2 -= speed;
        if (x2 <= -2) x2 += 4;

        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(x2, 0.0f, 0.0f));
        ourShader.setMat4("trans", trans);
        
        block.drawBg();
    }  

    void clear(){
        block.clear();
    }
};

#endif