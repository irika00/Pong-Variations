#include "CS3113/cs3113.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 2100 / 2,
              SCREEN_HEIGHT = 1500 / 2,
              FPS           = 60,
              SIZE          = 500 / 2,
              SPEED         = 200;

// constexpr int SCREEN_WIDTH  = 1600 / 2,
//               SCREEN_HEIGHT = 900 / 2,
//               FPS           = 60,
//               SIZE          = 500 / 2,
//               SPEED         = 200;

constexpr char    BG_COLOUR[]    = "#F8F1C8";
constexpr Vector2 ORIGIN         = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  BASE_SIZE      = { static_cast<float>(SIZE), static_cast<float>(SIZE) },
                  INIT_POS       = { ORIGIN.x - 200.0f, ORIGIN.y },
                  RUPEE_INIT_POS = { 21.f, ORIGIN.y }, //{ ORIGIN.x + 250.0f, ORIGIN.y };
                  PIANO2_INIT_POS = { ORIGIN.x + 505.f, ORIGIN.y };

// Images owned by Nintendo — please don't sue me.
constexpr char LINK_FP[]  = "assets/pink_note.png";
constexpr char RUPEE_FP[] = "assets/Piano.png";
constexpr char PIANO2[] = "assets/Piano2.png";


// Global Variables
AppStatus gAppStatus     = RUNNING;
float     gAngle         = 0.0f,
          gPreviousTicks = 0.0f;

Vector2 gLinkPosition  = INIT_POS,
        gLinkMovement  = { 0.0f, 0.0f },
        gLinkScale     = { BASE_SIZE.x, BASE_SIZE.y},

        gRupeePosition = RUPEE_INIT_POS,
        gRupeeMovement = { 0.0f, 0.0f },
        gRupeeScale    = { BASE_SIZE.x, BASE_SIZE.y},
        gPiano2Position = PIANO2_INIT_POS,
        gPiano2Movement = { 0.0f, 0.0f },
        gPiano2Scale    = { BASE_SIZE.x, BASE_SIZE.y},
        //gRupeeScale    = { BASE_SIZE.x / 2.0f, BASE_SIZE.y / 2.0f * 1.6f },
        //gRupeeScale    = { BASE_SIZE.x / 3.5f, BASE_SIZE.y / 3.5f},

        gMousePosition = GetMousePosition();

Texture2D gLinkTexture;
Texture2D gRupeeTexture;
Texture2D gPiano2Texture;

Music gBGM;
Sound gRupeeSound;

unsigned int gStartTime;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
bool isColliding(const Vector2 *positionA, const Vector2 *scaleA, const Vector2 *positionB, const Vector2 *scaleB);

// Function Definitions


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

    
    float scale_1 = fminf(BASE_SIZE.x / gLinkTexture.width, BASE_SIZE.y / gLinkTexture.height);
    gLinkScale = { gLinkTexture.width * scale_1, gLinkTexture.height * scale_1 };
    float scale_2 = fminf(BASE_SIZE.x / gRupeeTexture.width, BASE_SIZE.y / gRupeeTexture.height);
    gRupeeScale = { gRupeeTexture.width * scale_2, gRupeeTexture.height * scale_2 };
    gLinkScale = {0.35f*(float)gLinkTexture.width, 0.35f*(float)gLinkTexture.height };
    float scale_3 = fminf(BASE_SIZE.x / gPiano2Texture.width, BASE_SIZE.y / gPiano2Texture.height);
    gPiano2Scale = { gPiano2Texture.width * scale_3, gPiano2Texture.height * scale_3 };



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


    if      (IsKeyDown(KEY_W)) gRupeeMovement.y = -1;
    else if (IsKeyDown(KEY_S)) gRupeeMovement.y =  1;

    if      (IsKeyDown(KEY_UP)) gPiano2Movement.y = -1;
    else if (IsKeyDown(KEY_DOWN)) gPiano2Movement.y =  1;

    // to avoid faster diagonal speed
    if (GetLength(&gRupeeMovement) > 1.0f) Normalise(&gRupeeMovement);
    if (GetLength(&gPiano2Movement) > 1.0f) Normalise(&gPiano2Movement);

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    // Delta time
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    gMousePosition = GetMousePosition();

    gRupeePosition = {
        gRupeePosition.x + SPEED * gRupeeMovement.x * deltaTime,
        gRupeePosition.y + SPEED * gRupeeMovement.y * deltaTime
    };

    gPiano2Position = {
        gPiano2Position.x + SPEED * gPiano2Movement.x * deltaTime,
        gPiano2Position.y + SPEED * gPiano2Movement.y * deltaTime
    };

    if (isColliding(
        &gLinkPosition,  &gLinkScale,
        &gRupeePosition, &gRupeeScale
    ))
    {
        printf("Collision @ %us in game time.\n", static_cast<unsigned>(time(nullptr)) - gStartTime);
        if (!IsSoundPlaying(gRupeeSound)) PlaySound(gRupeeSound);
    }

    UpdateMusicStream(gBGM);
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    // Render Link
    renderObject(&gLinkTexture, &gLinkPosition, &gLinkScale);

    // Render the rupee
    renderObject(&gRupeeTexture, &gRupeePosition, &gRupeeScale);

    renderObject(&gPiano2Texture, &gPiano2Position, &gPiano2Scale);

    EndDrawing();
}

void shutdown()
{
    UnloadTexture(gLinkTexture);
    UnloadTexture(gRupeeTexture);
    UnloadTexture(gPiano2Texture);
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