#ifndef PLAYER_H
#define PLAYER_H

#include "../src/headers.h"

#include "../include/shader.h"
#include "../include/texture.h"

typedef pair<float, float> ff;

class Player_init{    // public data
public: 
    float vertices[30] = { // mapping of coordinates with the texture
        // positions          // colors           // texture coords
         -0.5f,  -0.8f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         -0.5f,  -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.7f,  -1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.7f, -0.8f,      1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[6] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO, VBO, EBO;

    Player_init(){
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

class Player{
private:

    Shader ourShader{"../src/vertex.shader", "../src/player/fragment.shader"};
    Texture ourTexture{"../src/player/avt.png"};
    Player_init block; // default constructor with empty parentheses works here

    // Transaltion in y direction
    GLFWwindow *window;
    float y = 0.0f, speed_y = 0.0f, a = 0.0009f, y0 = -0.9f, max_y_speed = 0.05f;
    // displacement_y, speed_y, ay, initial postion of player, max speed in y direction

    // Translation in x direction
    float x0 = -0.6f, x = 0, frame_speed;
    // initial position of center of player, displacement_x, v_x

    // x_position of the obstacles
    float * posx;
    int i = 0, n_zappers;  
    // nearest approaching obstacle

    // movement along y of obsatcles
    bool *is_moving;
    float speed_zapper_y;  
    float y_zapper = 0; // displacement along y direction
    int d = 1; // direction  

    // position of coins
    pair<float, float> *pos_coins;
    int j = 0;
    int coins_collided = 0;
    bool *show_coins;

    bool isFlying = false;

    void processInput()
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
        {
            if (speed_y < 0) speed_y += 10*a;
            else if (speed_y < max_y_speed) speed_y += a; // normalized values
            isFlying = true;
        }

        else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) 
        {
            if (speed_y > 0) speed_y -= 10*a;
            if (speed_y > -max_y_speed) speed_y -= a; // speed observed is used
            isFlying = false;
        }
        
        // collision with roof and wall
        if (y + speed_y <= 1.8f && y + speed_y >= -0.02f) y += speed_y;

    }

    bool intersect(ff a, ff b)
    {
        return !(a.second < b.first || b.second < a.first);
    }

    // collision with zappers
    int collision()
    {
        if ( intersect({x+x0-0.1, x+x0+0.1}, {posx[i]-0.05, posx[i]+0.05}) ) // 0.1/2 == 0.05 is the width of zapper and coin
        {
            float dis_y = {is_moving[i] ? y_zapper : 0};
            if ( intersect({y + y0 - 0.1f, y + y0 + 0.1}, {-1.0f + dis_y, 0.0f + dis_y}) )
            {
                return 1;  // coordinates of the zapper and player
            }
        }

        else if (x+x0+0.1 > posx[i]+0.05){
            i++;
            if (i == n_zappers) return -1; // level ends
        }

        return 0;
    }

    void collect_coins(){
        if ( intersect({x+x0-0.1, x+x0+0.1}, {pos_coins[j].first -0.05, pos_coins[j].first + 0.05f}) ) // 0.1/2 == 0.05 is the width of zapper and coin
        {
            if ( intersect({y + y0 - 0.1f, y + y0 + 0.1}, { pos_coins[j].second, pos_coins[j].second + 0.1f}) )
            {
                if (show_coins[j] == true) coins_collided++; // to avoid multiple collisions with the same coin
                show_coins[j] = false;  // changing in the global address
            }
        }

        else if (x + x0 + 0.1 > pos_coins[j].first + 0.05f){
            j++;
        }
    }

public:

    // constructor fxn for the initialization of the variables
    Player(GLFWwindow *input_window, float *zapper_x, float f_speed, int n_zap, bool *isMoving, float speed_zappery, pair<float, float> *poscoins, bool *show){
        posx = zapper_x;
        window = input_window;

        frame_speed = f_speed;

        n_zappers = n_zap;
        is_moving = isMoving;
        speed_zapper_y = speed_zappery; 

        pos_coins = poscoins;
        show_coins = show;
    }

    int render(){
        ourTexture.use(); // use the shader, texture
        ourShader.use();

        ourShader.setInt("isFlying", isFlying);
        
        // input
        // -----
        processInput();

        if ( (y_zapper >= 1.0f && d == 1) || (y_zapper <= 0.0f && d == -1) ) d = -d;
        y_zapper += d * speed_zapper_y;

        x += frame_speed; // relative motion of jack
        
        glm:: mat4 trans = glm::mat4(1.0f);
        trans = glm:: translate(trans, glm::vec3(0.0f, y, 0.0f)); // y is the displacement
        ourShader.setMat4("trans", trans);

        block.draw();
        collect_coins();

        return collision(); 
        // draw character fetch relative postion and collect coins and check collision
    }  

    float distanceTravelled(){
        return x;
    }

    int ncoinsCollected(){
        return coins_collided;
    }

    void clear(){
        block.clear();
    }
};

#endif
