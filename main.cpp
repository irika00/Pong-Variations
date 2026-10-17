#include "CS3113/cs3113.h"

/**

* Author: Irika Aggarwal

* Assignment: Pong Clone

* Date due: 10/16/2026

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/

// Global Constants
constexpr int SCREEN_WIDTH  = 2100 / 2,
              SCREEN_HEIGHT = 1500 / 2,
              FPS           = 60,
              SIZE          = 500 / 2,
              SPEED         = 200,
              MAX_BALLS = 8;


constexpr char    BG_COLOUR[]    = "#B3ECFF";
constexpr Vector2 ORIGIN         = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  BASE_SIZE      = { static_cast<float>(SIZE), static_cast<float>(SIZE) },
                  INIT_POS       = { ORIGIN.x - 200.0f, ORIGIN.y },
                  RUPEE_INIT_POS = { 21.f, ORIGIN.y }, //{ ORIGIN.x + 250.0f, ORIGIN.y };
                  PIANO2_INIT_POS = { ORIGIN.x + 505.f, ORIGIN.y },
                  SCORE_INIT_POS = { 200.f, 100.0f},
                  SECOND_SCORE_INIT_POS = { 800.f, 100.0f},
                  END_SCREEN_POS = ORIGIN;

constexpr char LINK_FP[]  = "assets/pink_note.png";
constexpr char RUPEE_FP[] = "assets/Piano.png";
constexpr char PIANO2[] = "assets/Piano2.png";
constexpr char NUMBER[] = "assets/number0.png";
constexpr char NUMBER_1[] = "assets/number1.png";
constexpr char NUMBER_2[] = "assets/number2.png";
constexpr char NUMBER_3[] = "assets/number3.png";
constexpr char NUMBER_4[] = "assets/number4.png";
constexpr char NUMBER_5[] = "assets/number5.png";
constexpr char NUMBER_6[] = "assets/number6.png";
constexpr char NUMBER_7[] = "assets/number7.png";
constexpr char BLUE_NOTE[] = "assets/blue_note.png";
constexpr char ORANGE_NOTE[] = "assets/orange_note.png";
constexpr char PLAYER1_WIN[] = "assets/Player1win.png";
constexpr char PLAYER2_WIN[] = "assets/Player2win.png";
constexpr char YOU_WON[] = "assets/youwon.png";
constexpr char YOU_LOST[] = "assets/youlost.png";


Vector2 startVel[MAX_BALLS] = {
    { -1.0f,  0.15f }, { 1.0f, -0.2f }, { -1.0f, -0.1f },
    {  1.2f,  0.25f }, { -0.35f, 1.05f }, { -0.25f, -1.05f }, { 1.35f, 1.05f }, { 0.35f, 1.05f }
};

// Global Variables
AppStatus gAppStatus     = RUNNING;
float     gAngle         = 0.0f,
          gPreviousTicks = 0.0f;

int gScore1 = 0;
int gScore2 = 0;
int gBallCount = 1;


Vector2 gLinkPosition  = ORIGIN,
        gLinkMovement  = { 0.0f, 0.0f },
        gLinkScale     = { BASE_SIZE.x, BASE_SIZE.y},

        gRupeePosition = RUPEE_INIT_POS,
        gRupeeMovement = { 0.0f, 0.0f },
        gRupeeScale    = { BASE_SIZE.x, BASE_SIZE.y},
        gPiano2Position = PIANO2_INIT_POS,
        gPiano2Movement = { 0.0f, 0.0f },
        gPiano2Scale    = { BASE_SIZE.x, BASE_SIZE.y},
        gScorePosition = SCORE_INIT_POS,
        gScoreMovement = { 0.0f, 0.0f },
        gScoreScale    = { BASE_SIZE.x, BASE_SIZE.y},
        gSecondScorePosition =  SECOND_SCORE_INIT_POS,
        gSecondScoreMovement = { 0.0f, 0.0f },
        gSecondScoreScale    = { BASE_SIZE.x, BASE_SIZE.y},
        gBallPosition[MAX_BALLS],
        gBallVelocity[MAX_BALLS],
        gBallScale[MAX_BALLS],
        gEndScreenScale =  {BASE_SIZE.x, BASE_SIZE.y},
        gEndScreenPosition = END_SCREEN_POS;



Texture2D gLinkTexture;
Texture2D gRupeeTexture;
Texture2D gPiano2Texture;
Texture2D gScoreTexture;
Texture2D gSecondScoreTexture;
Texture2D gNumber0Texture;
Texture2D gNumber1Texture;
Texture2D gNumber2Texture;
Texture2D gNumber3Texture;
Texture2D gNumber4Texture;
Texture2D gNumber5Texture;
Texture2D gNumber6Texture;
Texture2D gNumber7Texture;
Texture2D gBlueNote;
Texture2D gOrangeNote;
Texture2D gEndScreen;
Texture2D gYouWon;
Texture2D gYouLost;
Texture2D gPlayer1Won;
Texture2D gPlayer2Won;
Texture2D gBallTexture[MAX_BALLS];


Music gBGM;
Sound gLeftSound;
Sound gRightSound;


unsigned int gStartTime;
enum PLAYERS { ONE, TWO };
PLAYERS gCurrPlayers = TWO; 
 

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
bool isColliding(const Vector2 *positionA, const Vector2 *scaleA, const Vector2 *positionB, const Vector2 *scaleB);

// Function Definitions


/**
 * @brief Displays ending screen based on score
 */
void endGame()
{
    if (gCurrPlayers == ONE)
    {
        if (gScore1 >= 8)
        {
            gEndScreen = gYouWon;

        }
        else
        {
            gEndScreen = gYouLost;

        }

    }
    else
    {
        if (gScore1 >= 8)
        {
            gEndScreen = gPlayer1Won;

        }
        else
        {
            gEndScreen = gPlayer2Won;

        }

    }

}

/**
 * @brief Changes the score visual based on numerical score
 *
 * @param texture The current score texture
 * @param num The value of the current score

 */
void changeScore(Texture2D &texture, int num)
{  
    if (num == 0)
    {
        texture = gNumber0Texture;
    }
    else if (num == 1)
   {
    
    texture = gNumber1Texture;

   }
   else if (num == 2)
   {

    texture = gNumber2Texture;

   }
   else if (num == 3)
   {

    texture = gNumber3Texture;

   }
   else if (num == 4)
   {

    texture = gNumber4Texture;

   }
   else if (num == 5)
   {

    texture = gNumber5Texture;

   }
   else if (num == 6)
   {

    texture = gNumber6Texture;

   }
   else if (num == 7)
   {

    texture = gNumber7Texture;

   }

}


/**
 * @brief Checks for a square collision between 2 Rectangle objects.
 *
 * @param positionA The position of the first object
 * @param scaleA The scale of the first object
 * @param positionB The position of the second object
 * @param scaleB The scale of the second object
 * @return true if a collision is detected,
 * @return false if a collision is not detected
 */
bool isColliding(const Vector2 *positionA, const Vector2 *scaleA,
                 const Vector2 *positionB, const Vector2 *scaleB)
{
    float xDistance = fabs(positionA->x - positionB->x) - ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabs(positionA->y - positionB->y) - ((scaleA->y + scaleB->y) / 2.0f);

    return xDistance < 0.0f && yDistance < 0.0f;
}

void renderObject(const Texture2D *texture, const Vector2 *position,
                  const Vector2 *scale)
{
    // Whole texture (UV coordinates)
    Rectangle textureArea = {
        // top-left corner
        0.0f, 0.0f,

        // bottom-right corner (of texture)
        static_cast<float>(texture->width),
        static_cast<float>(texture->height)
    };

    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {
        position->x,
        position->y,
        static_cast<float>(scale->x),
        static_cast<float>(scale->y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 originOffset = {
        static_cast<float>(scale->x) / 2.0f,
        static_cast<float>(scale->y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        *texture,
        textureArea, destinationArea, originOffset,
        gAngle, WHITE
    );
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "User Input / Collision Detection");
    InitAudioDevice();

    gStartTime = time(nullptr);

    gLinkTexture  = LoadTexture(LINK_FP);
    gRupeeTexture = LoadTexture(RUPEE_FP);
    gPiano2Texture = LoadTexture(PIANO2);
    gScoreTexture = LoadTexture(NUMBER);
    gSecondScoreTexture = LoadTexture(NUMBER);
    gNumber0Texture = LoadTexture(NUMBER);
    gNumber1Texture = LoadTexture(NUMBER_1);
    gNumber2Texture = LoadTexture(NUMBER_2);
    gNumber3Texture = LoadTexture(NUMBER_3);
    gNumber4Texture = LoadTexture(NUMBER_4);
    gNumber5Texture = LoadTexture(NUMBER_5);
    gNumber6Texture = LoadTexture(NUMBER_6);
    gNumber7Texture = LoadTexture(NUMBER_7);
    gBlueNote = LoadTexture(BLUE_NOTE);
    gOrangeNote = LoadTexture(ORANGE_NOTE);
    gYouWon = LoadTexture(YOU_WON);
    gYouLost = LoadTexture(YOU_LOST);
    gPlayer1Won = LoadTexture(PLAYER1_WIN);
    gPlayer2Won = LoadTexture(PLAYER2_WIN);
    gEndScreen = gPlayer2Won;
    gBallTexture[0] = gLinkTexture;     // pink
    gBallTexture[1] = gBlueNote;
    gBallTexture[2] = gOrangeNote;
    gBallTexture[3] = gLinkTexture;    
    gBallTexture[4] = gBlueNote;
    gBallTexture[5] = gOrangeNote; 
    gBallTexture[6] = gLinkTexture;
    gBallTexture[7] = gOrangeNote;
    
    
    float scale_2 = fminf(BASE_SIZE.x / gRupeeTexture.width, BASE_SIZE.y / gRupeeTexture.height);
    gRupeeScale = { gRupeeTexture.width * scale_2 *1.2f, gRupeeTexture.height * scale_2 *1.2f};
    gLinkScale = {0.25f*(float)gLinkTexture.width, 0.25f*(float)gLinkTexture.height };
    float scale_3 = fminf(BASE_SIZE.x / gPiano2Texture.width, BASE_SIZE.y / gPiano2Texture.height);
    gPiano2Scale = { gPiano2Texture.width * scale_3 *1.2f, gPiano2Texture.height * scale_3 *1.2f};
    float scale_4 = fminf(BASE_SIZE.x / gScoreTexture.width, BASE_SIZE.y / gLinkTexture.height);
    gScoreScale = { gScoreTexture.width * scale_4*2.0f, gScoreTexture.height * scale_4 *2.0f };
    float scale_5 = fminf(BASE_SIZE.x / gScoreTexture.width, BASE_SIZE.y / gLinkTexture.height);
    gSecondScoreScale = { gSecondScoreTexture.width * scale_5*2.0f, gSecondScoreTexture.height * scale_5 *2.0f };


    for (int i = 0; i < MAX_BALLS; i++)
    {
        gBallPosition[i] = ORIGIN;
        gBallVelocity[i] = startVel[i];
        gBallScale[i] = gLinkScale;

    }


    gBGM = LoadMusicStream("assets/bgmusic.mp3");
    SetMusicVolume(gBGM, 0.33f);
    PlayMusicStream(gBGM);

    gLeftSound = LoadSound("assets/b6.mp3");
    gRightSound = LoadSound("assets/f6.mp3");

    SetTargetFPS(FPS);
}

void processInput()
{
    gRupeeMovement = { 0.0f, 0.0f };
    gPiano2Movement = { 0.0f, 0.0f };

    if (IsKeyPressed(KEY_ONE))   gBallCount = 1;
    if (IsKeyPressed(KEY_TWO))   gBallCount = 2;
    if (IsKeyPressed(KEY_THREE)) gBallCount = 3;
    if (IsKeyPressed(KEY_FOUR))  gBallCount = 4;
    if (IsKeyPressed(KEY_FIVE))  gBallCount = 5;
    if (IsKeyPressed(KEY_SIX))  gBallCount = 6;
    if (IsKeyPressed(KEY_SEVEN))  gBallCount = 7;
    if (IsKeyPressed(KEY_EIGHT))  gBallCount = 8;

    if (gCurrPlayers == TWO) 
    {
        if      (IsKeyDown(KEY_W)) gRupeeMovement.y = -1;
        else if (IsKeyDown(KEY_S)) gRupeeMovement.y =  1;

        if      (IsKeyDown(KEY_UP)) gPiano2Movement.y = -1;
        else if (IsKeyDown(KEY_DOWN)) gPiano2Movement.y =  1;
    }
    else
    {
        if      (IsKeyDown(KEY_W)) gRupeeMovement.y = -1;
        else if (IsKeyDown(KEY_S)) gRupeeMovement.y =  1;


        //automatic movement when in single player mode
        int target = 0;
        for (int i = 1; i < gBallCount; i++)
        {
            if (gBallPosition[i].x > gBallPosition[target].x) target = i;
        }

        float diff = gBallPosition[target].y - gPiano2Position.y;

        if      (diff >  10.0f) gPiano2Movement.y =  1.0f;
        else if (diff < -10.0f) gPiano2Movement.y = -1.0f;
        
    }

    
    // to avoid faster diagonal speed
    if (GetLength(&gRupeeMovement) > 1.0f) Normalise(&gRupeeMovement);
    if (GetLength(&gPiano2Movement) > 1.0f) Normalise(&gPiano2Movement);


    //switch to single or double player mode
    if (IsKeyPressed(KEY_T))
    {
        if (gCurrPlayers == ONE)
        {
            gCurrPlayers = TWO;
            gScore1 = 0;
            gScore2 = 0;   

        }
        else
        {
            gCurrPlayers = ONE;
            gScore1 = 0;
            gScore2 = 0;

        }
        gScoreTexture = gNumber0Texture;
        gSecondScoreTexture = gNumber0Texture;
        
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    // Delta time
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;


    gRupeePosition = {
        gRupeePosition.x + SPEED * gRupeeMovement.x * deltaTime,
        gRupeePosition.y + SPEED * gRupeeMovement.y * deltaTime
    };

    gPiano2Position = {
        gPiano2Position.x + SPEED * gPiano2Movement.x * deltaTime,
        gPiano2Position.y + SPEED * gPiano2Movement.y * deltaTime
    };

    // to keep piano on screen
    float halfLeft = gRupeeScale.y / 2.0f;
    if (gRupeePosition.y - halfLeft < 0.0f)
        gRupeePosition.y = halfLeft;
    else if (gRupeePosition.y + halfLeft > SCREEN_HEIGHT)
        gRupeePosition.y = SCREEN_HEIGHT - halfLeft;

    float halfRight = gPiano2Scale.y / 2.0f;
    if (gPiano2Position.y - halfRight < 0.0f)
        gPiano2Position.y = halfRight;
    else if (gPiano2Position.y + halfRight > SCREEN_HEIGHT)
        gPiano2Position.y = SCREEN_HEIGHT - halfRight;

    
    for (int i = 0; i < gBallCount; i++ ){

        gBallPosition[i] = {
            gBallPosition[i].x+ SPEED * deltaTime * gBallVelocity[i].x,
            gBallPosition[i].y + SPEED * deltaTime * gBallVelocity[i].y
        };

        Vector2 hitScale = { gBallScale[i].x * 0.8f, gBallScale[i].y * 0.8f }; 
        
        //collisions
        if (isColliding(
            &gBallPosition[i],  &hitScale,
            &gRupeePosition, &gRupeeScale
        ) && gBallVelocity[i].x < 0.0f)
        {
            gBallVelocity[i].x = -gBallVelocity[i].x;
            if (!IsSoundPlaying(gLeftSound)) PlaySound(gLeftSound);
        }

        if (isColliding(
            &gBallPosition[i],  &hitScale,
            &gPiano2Position, &gPiano2Scale
        ) && gBallVelocity[i].x > 0.0f)
        {
            gBallVelocity[i].x = -gBallVelocity[i].x;
            if (!IsSoundPlaying(gRightSound)) PlaySound(gRightSound);
        }

        if (gBallPosition[i].y - (gBallScale[i].y / 2.0f) < 0.0f)               
        {
            gBallPosition[i].y = gBallScale[i].y / 2.0f;                     
            gBallVelocity[i].y = -gBallVelocity[i].y;
        }
        else if (gBallPosition[i].y + gBallScale[i].y / 2.0f > SCREEN_HEIGHT)
        {
            gBallPosition[i].y = SCREEN_HEIGHT - (gBallScale[i].y / 2.0f);
            gBallVelocity[i].y = -gBallVelocity[i].y;
        }

        if (gBallPosition[i].x + gBallScale[i].x / 2.0f < 0.0f) //make ball reappear 
        {
            gBallPosition[i] = ORIGIN;
            gScore2 += 1;
            if (gScore2 < 8)
            {
                
                changeScore(gSecondScoreTexture, gScore2);
            }

            else
            {
                endGame();
                gAppStatus = ENDSCREEN;

            }
            
        }
        else if (gBallPosition[i].x - gBallScale[i].x / 2.0f > SCREEN_WIDTH) //make ball reappear 
        {
            gBallPosition[i] = ORIGIN;
            gScore1 += 1;
            if (gScore1 < 8)
            {
                changeScore(gScoreTexture, gScore1);
            }
            else
            {
                endGame();
                gAppStatus = ENDSCREEN;
            }
            
        }

    }


    UpdateMusicStream(gBGM);
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    

    if (gAppStatus == ENDSCREEN)
    {
        Vector2 screentex = {static_cast<float> (gEndScreen.width), static_cast<float>(gEndScreen.height)};
        renderObject(&gEndScreen, &gEndScreenPosition, &screentex);
    }

    else
    {
        renderObject(&gRupeeTexture, &gRupeePosition, &gRupeeScale);
        renderObject(&gPiano2Texture, &gPiano2Position, &gPiano2Scale);
        renderObject(&gScoreTexture, &gScorePosition, &gScoreScale);
        renderObject(&gSecondScoreTexture, &gSecondScorePosition, &gSecondScoreScale);
        for (int i = 0; i < gBallCount; i++)
        {
        renderObject(&gBallTexture[i], &gBallPosition[i], &gBallScale[i]);
        }

    }

    EndDrawing();
}

void shutdown()
{
    UnloadTexture(gLinkTexture);
    UnloadTexture(gRupeeTexture);
    UnloadTexture(gPiano2Texture);
    UnloadTexture(gNumber0Texture);
    UnloadTexture(gNumber1Texture);
    UnloadTexture(gNumber2Texture);
    UnloadTexture(gNumber3Texture);
    UnloadTexture(gNumber4Texture);
    UnloadTexture(gNumber5Texture);
    UnloadTexture(gNumber6Texture);
    UnloadTexture(gNumber7Texture);
    UnloadTexture(gEndScreen);
    UnloadMusicStream(gBGM);
    UnloadSound(gRightSound);
    UnloadSound(gLeftSound);
    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    while (gAppStatus == ENDSCREEN)
    {
        processInput();
        render();
    }

    shutdown();

    return 0;
}