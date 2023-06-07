#include "../src/headers.h"
#include "../src/frame/frame.h"
#include "../src/player/player.h"
#include "../src/obstacle/obstacle.h"
#include "../src/coins/coins.h"
#include "../src/text/text.h"

#define PLAY 0
#define LOST -1

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // text: load openGL text
    // ----------------------------------------
    Text text;
    if (text.isError()) 
    {
        std::cout << "Failed to load Text" << endl;
        return -1;
    }

    // render loop starts
    // --------------------------------------------------------------------

    int n = 6, game_state = PLAY; // number of obstacles / zappers, curr state of game

    float frame_speed = 0.01f, speed_zapper_y = 0.01f; // speed of the jack, speed of obstacles

    int total_score = 0;

    for(int level_no = 1; level_no <= 3 && !glfwWindowShouldClose(window); level_no++)
    {
        // init for obstacles, coins, players, frames
        // ----------------------------------------------------------------

        if (level_no != 1) 
        {
            frame_speed += 0.005f;
            speed_zapper_y += 0.005f;
        }

        float posx[n], prefix = 0; // postion of zappers
        for(int i=0; i<n; i++){
            prefix += (float)rand() * 0.5f/(RAND_MAX) + 1.5; // generate a random value [1.5, 2]
            posx[i] = prefix;
        }

        // random movement of zappers
        bool isMoving[n];
        for(int i=0; i<n; i++){
            isMoving[i] = rand() % 2;
        }

        Level frame(frame_speed, level_no); // frames moving towards left

        Obstacle obstacles(posx, frame_speed, n, isMoving, speed_zapper_y);

        // init for coins
        int n_coins = 5 * n;
        pair<float, float> pos_coins[n_coins]; // store the postion of coins
        bool show_coins[n_coins];

        for(int i=0; i<n_coins; i++) show_coins[i] = true;

        for(int i=0; i<n; i++){ // spwan coins randomly after each barrier

            float y_coins = (float)rand() / (RAND_MAX) - 1 ; // generate random [-1, 0]

            for(int j=0; j<5; j++){
                pos_coins[5*i+j] = {posx[i] + 0.5f + 0.15f * j, y_coins}; // put 5 coins simultaneously in a row
            }
        }
        Coins coins(pos_coins, frame_speed, n_coins, show_coins);

        // Player
        Player player(window, posx, frame_speed, n, isMoving, speed_zapper_y, pos_coins, show_coins);

        // init ends
        // ------------------------------------------------------------------------
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // render
            // ------

            // to render the frame, zappers, coins
            frame.render();
            obstacles.render();
            coins.render();

            // Text render
            float x_text = 10.0f, y_text = 580.0f;

            string level_str = "Level: " + to_string(level_no);
            text.render(level_str.c_str(), x_text, y_text); // Level number

            string distance_str = "Distance Travelled: " + to_string(player.distanceTravelled());
            text.render(distance_str.c_str(), x_text, y_text - 20);

            string length_str =  "Length: " + to_string(posx[n-1]);
            text.render(length_str.c_str(), x_text, y_text - 40);

            string score_str = "Score: " + to_string(total_score + player.ncoinsCollected());
            text.render(score_str.c_str(), x_text,  y_text - 60);

            // Text ends

            int ret = player.render();
            if (ret == 1){ // collides
                game_state = LOST;
                break;
            }

            else if (ret == -1) break; // level ends so move to the next level

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        total_score += player.ncoinsCollected();

        // dealocate all the resources
        frame.clear();
        player.clear();
        obstacles.clear();
        coins.clear();

        if (game_state == LOST) break;

    } // level loop ends here


    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (game_state == LOST)
        {
            text.render("Game Over", 200.0f, 400.0f, 1.0f);
            string game_lost = "You Lost with Score: " + to_string(total_score);
            text.render(game_lost.c_str(), 100.0f, 300.0f, 1.0f);
        }

        else // win
        {
            text.render("Game Over", 200.0f, 400.0f, 1.0f);
            string game_lost = "You Win with Score: " + to_string(total_score);
            text.render(game_lost.c_str(), 100.0f, 300.0f, 1.0f);
        }
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.    
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
