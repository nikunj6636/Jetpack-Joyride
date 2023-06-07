#ifndef COIN_H
#define COIN_H

#include "../src/headers.h"
#include "../include/shader.h"
#include "../include/texture.h"

class Coin_init{    // public data
public: 
    float vertices[30] = { // square coins of width 0.1f
        // positions          // colors          
         0.05f,  -0.9f,   1.0f, 1.0f, 0.0f,     1.0f, 1.0f, // top right
         0.05f,  -1.0f,   1.0f, 1.0f, 0.0f,     1.0f, 0.0f, // bottom right
        -0.05f,  -1.0f,    1.0f, 1.0f, 0.0f,    0.0f, 0.0f, // bottom left
        -0.05f,  -0.9f,      1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // top left 
    };

    unsigned int indices[6] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO, VBO, EBO;

    Coin_init(){
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

class Coins{
private:

    float frame_speed, shift = 0.0f;
    // speed of the frame

    Shader ourShader{"../src/vertex.shader", "../src/fragment.shader"};
    Texture ourTexture{"../src/coins/img.png"};

    Coin_init block; // default constructor with empty parentheses works here

    // number of coins and their postion
    int n_coins;
    pair<float, float>* pos;
    bool *show_coins;
    
public:

    Coins(pair<float, float> *pos_coins, float f_speed, int ncoins, bool *show){    // init the variables
        pos = pos_coins;
        frame_speed = f_speed;
        n_coins = ncoins;
        show_coins = show;
    };

    void render(){
        ourShader.use();
        ourTexture.use();
        
        shift -= frame_speed;   // zappers move towars left, hence relatively player move towards right

        for(int i=0; i<n_coins; i++) 
        {    
            if (show_coins[i])
            {    
                glm:: mat4 trans = glm::mat4(1.0f);
                trans = glm:: translate(trans, glm::vec3(pos[i].first + shift, pos[i].second + 1.0f, 0.0f)); 
                // displacement is finalposition - initialposition

                ourShader.setMat4("trans", trans);
                block.draw();
            }
        }
    }  

    void clear(){
        block.clear();
    }
};

#endif