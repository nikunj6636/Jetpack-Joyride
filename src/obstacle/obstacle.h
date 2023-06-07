#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "../src/headers.h"

#include "../include/shader.h"
#include "../include/texture.h"

class Obstacle_init{    // public data
public: 

    float vertices[30] = {
        // positions          // colors          
         0.05f,  0.0f,   1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
         0.05f,  -1.0f,  1.0f, 1.0f, 0.0f,   1.0, 0.0f,
        -0.05f,  -1.0f,   1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
        -0.05f, 0.0f,    1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    };

    unsigned int indices[6] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO, VBO, EBO;

    Obstacle_init(){
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

    void draw(){
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void clear(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};

class Obstacle{
private:

    float frame_speed, shift = 0.0f;
    // speed of the frame

    Shader ourShader{"../src/obstacle/vertex.shader", "../src/obstacle/fragment.shader"};

    Obstacle_init block; // default constructor with empty parentheses works here

    // number of zappers and their postion
    int n_zappers;
    float *x;

    // movement along y;
    bool *is_moving;
    float speed_zapper_y;  
    float y = 0; // displacement along y direction
    int d = 1; // direction
    
public:

    Obstacle(float *posx, float f_speed, int n_zap, bool *isMoving, float speed_zappery){    // init the variables
        x = posx;
        frame_speed = f_speed;
        n_zappers = n_zap;
        is_moving = isMoving;
        speed_zapper_y = speed_zappery; 
    };

    void render(){
        ourShader.use();
        
        shift -= frame_speed;   // zappers move towars left, hence relatively player move towards right

        if ( (y >= 1.0f && d == 1) || (y <= 0.0f && d == -1) ) d = -d;
        y += d * speed_zapper_y;

        for(int i=0; i<n_zappers; i++) // draw n rectangles, beautiful rendering of the objects
        {    
            glm:: mat4 trans = glm::mat4(1.0f);
            trans = glm:: translate(trans, glm::vec3(x[i] + shift, {is_moving[i] ? y : 0} , 0.0f)); // y is the displacement
            ourShader.setMat4("trans", trans);
            block.draw();
        }
    }  

    void clear(){
        block.clear();
    }
};

#endif