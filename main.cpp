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
              MAX_BALLS = 5;

// constexpr int SCREEN_WIDTH  = 1600 / 2,
//               SCREEN_HEIGHT = 900 / 2,
//               FPS           = 60,
//               SIZE          = 500 / 2,
//               SPEED         = 200;

constexpr char    BG_COLOUR[]    = "#B3ECFF";
constexpr Vector2 ORIGIN         = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  BASE_SIZE      = { static_cast<float>(SIZE), static_cast<float>(SIZE) },
                  INIT_POS       = { ORIGIN.x - 200.0f, ORIGIN.y },
                  RUPEE_INIT_POS = { 21.f, ORIGIN.y }, //{ ORIGIN.x + 250.0f, ORIGIN.y };
                  PIANO2_INIT_POS = { ORIGIN.x + 505.f, ORIGIN.y },
                  SCORE_INIT_POS = { 200.f, 100.0f},
                  SECOND_SCORE_INIT_POS = { 800.f, 100.0f};

constexpr char LINK_FP[]  = "assets/pink_note.png";
constexpr char RUPEE_FP[] = "assets/Piano.png";
constexpr char PIANO2[] = "assets/Piano2.png";
constexpr char NUMBER[] = "assets/number0.png";
constexpr char NUMBER_1[] = "assets/number1.png";
constexpr char NUMBER_2[] = "assets/number2.png";
constexpr char NUMBER_3[] = "assets/number3.png";
constexpr char BLUE_NOTE[] = "assets/blue_note.png";
constexpr char ORANGE_NOTE[] = "assets/orange_note.png";
constexpr char PLAYER1_WIN[] = "assets/Player1win.png";
constexpr char PLAYER2_WIN[] = "assets/Player2win.png";
constexpr char YOU_WON[] = "assets/youwon.png";
constexpr char YOU_LOST[] = "assets/youlost.png";


Vector2 startVel[MAX_BALLS] = {
    { -1.0f,  0.15f }, { 1.0f, -0.2f }, { -1.0f, -0.1f },
    {  1.2f,  0.25f }, { -0.35f, 1.05f }
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
        //gRupeeScale    = { BASE_SIZE.x / 2.0f, BASE_SIZE.y / 2.0f * 1.6f },
        //gRupeeScale    = { BASE_SIZE.x / 3.5f, BASE_SIZE.y / 3.5f},

        gMousePosition = GetMousePosition();

Texture2D gLinkTexture;
Texture2D gRupeeTexture;
Texture2D gPiano2Texture;
Texture2D gScoreTexture;
Texture2D gSecondScoreTexture;
Texture2D gNumber0Texture;
Texture2D gNumber1Texture;
Texture2D gNumber2Texture;
Texture2D gNumber3Texture;
Texture2D gBlueNote;
Texture2D gOrangeNote;
Texture2D gBallTexture[MAX_BALLS];


Music gBGM;
Sound gRupeeSound;

unsigned int gStartTime;
Vector2 gLinkVelocity = { -1.0f, -0.5f };   // x: moves left, y: moves down 
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

bool changeScore(Texture2D &texture, int num)
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

   return true;

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
    gBlueNote = LoadTexture(BLUE_NOTE);
    gOrangeNote = LoadTexture(ORANGE_NOTE);

    gBallTexture[0] = gLinkTexture;     // pink
    gBallTexture[1] = gBlueNote;
    gBallTexture[2] = gOrangeNote;
    gBallTexture[3] = gLinkTexture;    
    gBallTexture[4] = gBlueNote;
    
    
    float scale_1 = fminf(BASE_SIZE.x / gLinkTexture.width, BASE_SIZE.y / gLinkTexture.height);
    gLinkScale = { gLinkTexture.width * scale_1, gLinkTexture.height * scale_1 };
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


    gBGM = LoadMusicStream("assets/game/ost.mp3");
    SetMusicVolume(gBGM, 0.33f);
    PlayMusicStream(gBGM);

    gRupeeSound = LoadSound("assets/game/rupee.mp3");

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

        if (gBallVelocity[0].x > 0.0f)
        {
            float diff = gBallPosition[0].y - gPiano2Position.y;

            if      (diff >  10.0f) gPiano2Movement.y =  1.0f;
            else if (diff < -10.0f) gPiano2Movement.y = -1.0f;
        }

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

    //gMousePosition = GetMousePosition();

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



        //collisions
        if (isColliding(
            &gBallPosition[i],  &gBallScale[i],
            &gRupeePosition, &gRupeeScale
        ) && gBallVelocity[i].x < 0.0f)
        {
            gBallVelocity[i].x = -gBallVelocity[i].x;
            // printf("Collision @ %us in game time.\n", static_cast<unsigned>(time(nullptr)) - gStartTime);
            // if (!IsSoundPlaying(gRupeeSound)) PlaySound(gRupeeSound);
        }

        if (isColliding(
            &gBallPosition[i],  &gBallScale[i],
            &gPiano2Position, &gPiano2Scale
        ) && gBallVelocity[i].x > 0.0f)
        {
            gBallVelocity[i].x = -gBallVelocity[i].x;
            // printf("Collision @ %us in game time.\n", static_cast<unsigned>(time(nullptr)) - gStartTime);
            // if (!IsSoundPlaying(gRupeeSound)) PlaySound(gRupeeSound);
        }

        if (gBallPosition[i].y - (gBallScale[i].y / 2.0f) < 0.0f)               
        {
            gBallPosition[i].y = gBallPosition[i].y / 2.0f;                     
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
            if (gScore2 <= 3)
            {
                changeScore(gSecondScoreTexture, gScore2);
            }

            else
            {
                gAppStatus = TERMINATED;

            }
            
        }
        else if (gBallPosition[i].x - gBallScale[i].x / 2.0f > SCREEN_WIDTH) //make ball reappear 
        {
            gBallPosition[i] = ORIGIN;
            gScore1 += 1;
            if (gScore1 <= 3)
            {
                changeScore(gScoreTexture, gScore1);
            }
            else
            {
                gAppStatus = TERMINATED;
            }
            
        }

    }

    // gRupeePosition = {
    //     gRupeePosition.x + SPEED * gRupeeMovement.x * deltaTime,
    //     gRupeePosition.y + SPEED * gRupeeMovement.y * deltaTime
    // };

    // gPiano2Position = {
    //     gPiano2Position.x + SPEED * gPiano2Movement.x * deltaTime,
    //     gPiano2Position.y + SPEED * gPiano2Movement.y * deltaTime
    // };

    // gLinkPosition = {
    //     gLinkPosition.x+ SPEED * deltaTime * gLinkVelocity.x,
    //     gLinkPosition.y + SPEED * deltaTime * gLinkVelocity.y
    // };

    // // to keep piano on screen
    // float halfLeft = gRupeeScale.y / 2.0f;
    // if (gRupeePosition.y - halfLeft < 0.0f)
    //     gRupeePosition.y = halfLeft;
    // else if (gRupeePosition.y + halfLeft > SCREEN_HEIGHT)
    //     gRupeePosition.y = SCREEN_HEIGHT - halfLeft;

    // float halfRight = gPiano2Scale.y / 2.0f;
    // if (gPiano2Position.y - halfRight < 0.0f)
    //     gPiano2Position.y = halfRight;
    // else if (gPiano2Position.y + halfRight > SCREEN_HEIGHT)
    //     gPiano2Position.y = SCREEN_HEIGHT - halfRight;


    // //collisions
    // if (isColliding(
    //     &gLinkPosition,  &gLinkScale,
    //     &gRupeePosition, &gRupeeScale
    // ) && gLinkVelocity.x < 0.0f)
    // {
    //     gLinkVelocity.x = -gLinkVelocity.x;
    //     // printf("Collision @ %us in game time.\n", static_cast<unsigned>(time(nullptr)) - gStartTime);
    //     // if (!IsSoundPlaying(gRupeeSound)) PlaySound(gRupeeSound);
    // }

    // if (isColliding(
    //     &gLinkPosition,  &gLinkScale,
    //     &gPiano2Position, &gPiano2Scale
    // ) && gLinkVelocity.x > 0.0f)
    // {
    //     gLinkVelocity.x = -gLinkVelocity.x;
    //     // printf("Collision @ %us in game time.\n", static_cast<unsigned>(time(nullptr)) - gStartTime);
    //     // if (!IsSoundPlaying(gRupeeSound)) PlaySound(gRupeeSound);
    // }

    // if (gLinkPosition.y - (gLinkScale.y / 2.0f) < 0.0f)               
    // {
    //     gLinkPosition.y = gLinkScale.y / 2.0f;                     
    //     gLinkVelocity.y = -gLinkVelocity.y; 
    // }
    // else if (gLinkPosition.y + (gLinkScale.y / 2.0f) > SCREEN_HEIGHT) 
    // {
    //     gLinkPosition.y = SCREEN_HEIGHT - (gLinkScale.y / 2.0f);
    //     gLinkVelocity.y = -gLinkVelocity.y;
    // }

    // if (gLinkPosition.x + gLinkScale.x / 2.0f < 0.0f) //make ball reappear 
    // {
    //     gLinkPosition = ORIGIN;
    //     gScore2 += 1;
    //     if (gScore2 <= 3)
    //     {
    //         changeScore(gSecondScoreTexture, gScore2);
    //     }

    //     else
    //     {
    //         gAppStatus = TERMINATED;

    //     }
        
    // }
    // else if (gLinkPosition.x - gLinkScale.x / 2.0f > SCREEN_WIDTH) //make ball reappear 
    // {
    //     gLinkPosition = ORIGIN;
    //     gScore1 += 1;
    //     if (gScore1 <= 3)
    //     {
    //         changeScore(gScoreTexture, gScore1);
    //     }
    //     else
    //     {
    //         gAppStatus = TERMINATED;
    //     }
        
    // }


    UpdateMusicStream(gBGM);
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    
    renderObject(&gRupeeTexture, &gRupeePosition, &gRupeeScale);
    renderObject(&gPiano2Texture, &gPiano2Position, &gPiano2Scale);
    renderObject(&gScoreTexture, &gScorePosition, &gScoreScale);
    renderObject(&gSecondScoreTexture, &gSecondScorePosition, &gSecondScoreScale);
    // renderObject(&gLinkTexture, &gLinkPosition, &gLinkScale);
    for (int i = 0; i < gBallCount; i++)
    {
    renderObject(&gBallTexture[i], &gBallPosition[i], &gBallScale[i]);
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
    UnloadMusicStream(gBGM);
    UnloadSound(gRupeeSound);
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

    shutdown();

    return 0;
}