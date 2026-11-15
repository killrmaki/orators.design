#define RAYGUI_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "raygui.h"
#define END -1
#define MAX_DIALOGUE_LENGTH 2048
#define DIALOGUE_FROM_FILE NULL

typedef struct {
    const char *text;
    int next;
} Choice;

typedef struct {
    const char *speaker;
    const char *line; // Loaded from assets/dialogue.md by node index.
    const char *background; // "start", "tier2", "tier3", "castle"
    int choiceCount;
    Choice choices[3];
    int next; // -1 = end
} Node;

typedef enum {
    tier_1_1,   // Tier 1
    tier_1_2,
    tier_1_3,
    tier_1_4,
    tier_1_c,       // Choice
    tier_1a_1,
    tier_1a_2,
    tier_1a_3,
    tier_1a_4,
    tier_1a_5,
    tier_1b_1,
    tier_1b_2,
    tier_1b_3,
    tier_2_1,   // Tier 2
    tier_2_2,
    tier_2_3,
    tier_2_c,       // Choice   
    tier_2a_1,
    tier_2a_2,
    tier_2a_3,
    tier_2a_4,
    tier_2b_1,   
    tier_2b_2,
    tier_2b_3,
    tier_2b_4,
    tier_2c_1,   
    tier_2c_2,
    tier_2c_3,
    tier_2c_4,
    tier_3_1,   // Tier 3
    tier_3_2,
    tier_3_3,
    tier_3_4,
    tier_3_5,
    tier_3_6,
    tier_3_7,
    tier_3_8,
    tier_3_c,       // Choice
    tier_3a_1,
    tier_3a_2,
    tier_3a_3,
    tier_3a_4,
    tier_3a_5,
    tier_3a_6,
    tier_3a_7,
    tier_3a_8,
    end_b,          // Ending
    tier_3b_1,
    tier_3b_2,
    tier_3b_3,
    tier_3b_4,
    tier_3b_5,
    tier_3b_6,
    tier_3b_7,
    tier_3b_8,
    tier_3b_9,
    tier_3b_10,
    end_c,          // Ending
    tier_4_1,   // Tier 4
    tier_4_2,
    tier_4_3,
    tier_4_4,
    tier_4_5,
    tier_4_c,       // Choice
    tier_4a_1,
    tier_4a_2,
    tier_4a_3,
    tier_4a_4,
    tier_4a_5,
    tier_4a_6,
    tier_4b_1,
    tier_4b_2,
    tier_4b_3,
    tier_4b_4,
    tier_4b_5,
    tier_4b_6,
    tier_4b_7,
    tier_4b_8,
    tier_4b_9,
    tier_4b_10,
    tier_4b_11,
    end_a           // Ending
} Path;

typedef enum {
    none,
    plant1,
    plant2,
    plant3
} Plant;

typedef enum {
    TITLE_SCREEN,
    STORY_SCREEN
} Screen;

/*
Dialogue lines are loaded from assets/dialogue.md in story[] order.
Choice labels and destinations remain in the node data.
*/


static Node story[] = {
    // Tier 1
    {
        "Perse",
        DIALOGUE_FROM_FILE,
        "start",
        0,
        {{0}},
        tier_1_2
    },
    {
        "Circe",
        DIALOGUE_FROM_FILE,
        "start",
        0,
        {{0}},
        tier_1_3
    },
    {
        "Perse",
        DIALOGUE_FROM_FILE,
        "start",
        0,
        {{0}},
        tier_1_4
    },
    {
        "Circe",
        DIALOGUE_FROM_FILE,
        "start",
        0,
        {{0}},
        tier_1_c
    },
    {
        "Perse",
        DIALOGUE_FROM_FILE,
        "start",
        2,
        {{"Honor your mother's wishes", tier_1a_1}, {"Defy your mother's expectations", tier_1b_1}},
        END
    },

    {
        "Circe",
        DIALOGUE_FROM_FILE,
        "start",
        0,
        {{0}},
        tier_1a_2
    },
    {
        "Perse",
        DIALOGUE_FROM_FILE,
        "start",
        0,
        {{0}},
        tier_1a_3
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "start",
        0,
        {{0}},
        tier_1a_4
    },
    {
        "Perse",
        DIALOGUE_FROM_FILE,
        "start",
        0,
        {{0}},
        tier_1a_5
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "start",
        0,
        {{0}},
        tier_2_1
    },

    {
        "Circe",
        DIALOGUE_FROM_FILE,
        "start",
        0,
        {{0}},
        tier_1b_2
    },
    {
        "Perse",
        DIALOGUE_FROM_FILE,
        "start",
        0,
        {{0}},
        tier_1b_3
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "start",
        0,
        {{0}},
        tier_4_1
    },

    // Tier 2
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_2_2
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_2_3
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_2_c
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        3,
        {{"Heal", tier_2a_1}, {"Enchant", tier_2b_1}, {"Command", tier_2c_1}},
        END
    },

    {
        "Perse",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_2a_2
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_2a_3
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_2a_4
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_3_1
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_2b_2
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_2b_3
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_2b_4
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_3_1
    },

    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_2c_2
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_2c_3
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_2c_4
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier2",
        0,
        {{0}},
        tier_3_1
    },

    // Tier 3
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3_2
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3_3
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3_4
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3_5
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3_6
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3_7
    },
    {
        "Sailor",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3_8
    },
    {
        "Circe",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3_c
    },
    {
        "Sailor",
        DIALOGUE_FROM_FILE,
        "tier3",
        2,
        {{"Mercy", tier_3a_1}, {"Turn the sailors into pigs", tier_3b_1}},
        END
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3a_2
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3a_3
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3a_4
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3a_5
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3a_6
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3a_7
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3a_8
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        end_b
    },
    {
        "END",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        END
    },

    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3b_2
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3b_3
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3b_4
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3b_5
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3b_6
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3b_7
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3b_8
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3b_9
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        tier_3b_10
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        end_c
    },
    {
        "END",
        DIALOGUE_FROM_FILE,
        "tier3",
        0,
        {{0}},
        END
    },

    // Tier 4
    {
        "Helios",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4_2
    },
    {
        "Circe",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4_3
    },
    {
        "Helios",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4_4
    },
    {
        "Helios",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4_5
    },
    {
        "Circe",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4_c
    },
    {
        "Helios",
        DIALOGUE_FROM_FILE,
        "castle",
        2,
        {{"Leave", tier_4a_1}, {"Stay", tier_4b_1}},
        END
    },

    {
        "Circe",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4a_2
    },
    {
        "Helios",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4a_3
    },
    {
        "Circe",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4a_4
    },
    {
        "Helios",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4a_5
    },
    {
        "Circe",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4a_6
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_2_1
    },

    {
        "Circe",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4b_2
    },
    {
        "Helios",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4b_3
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4b_4
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4b_5
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4b_6
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4b_7
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4b_8
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4b_9
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4b_10
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        tier_4b_11
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        end_a
    },
    {
        "Narrator",
        DIALOGUE_FROM_FILE,
        "castle",
        0,
        {{0}},
        END
    }
};

static char dialogueText[sizeof(story) / sizeof(story[0])][MAX_DIALOGUE_LENGTH];

static int LoadDialogue(void)
{
    FILE *file = fopen("assets/dialogue.md", "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open assets/dialogue.md\n");
        return 0;
    }

    size_t nodeCount = sizeof(story) / sizeof(story[0]);
    for (size_t i = 0; i < nodeCount; i++) {
        if (fgets(dialogueText[i], sizeof(dialogueText[i]), file) == NULL) {
            fprintf(stderr, "assets/dialogue.md is missing line %zu\n", i + 1);
            fclose(file);
            return 0;
        }

        if (strchr(dialogueText[i], '\n') == NULL && !feof(file)) {
            fprintf(stderr, "Line %zu in assets/dialogue.md is too long (maximum %d characters)\n",
                    i + 1, MAX_DIALOGUE_LENGTH - 1);
            fclose(file);
            return 0;
        }

        dialogueText[i][strcspn(dialogueText[i], "\r\n")] = '\0';
        story[i].line = dialogueText[i];
    }

    if (fgetc(file) != EOF) {
        fprintf(stderr, "assets/dialogue.md has more lines than story nodes\n");
        fclose(file);
        return 0;
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading assets/dialogue.md\n");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

static int DrawWrappedText(const char *text, int x, int y, int maxWidth, int fontSize, int lineSpacing, Color color)
{
    size_t capacity = strlen(text) + 1;
    char line[capacity];
    char candidate[capacity];
    line[0] = '\0';
    size_t lineLength = 0;
    int lineCount = 0;

    while (*text != '\0') {
        while (*text == ' ' || *text == '\t') text++;

        if (*text == '\n') {
            DrawText(line, x, y + lineCount * lineSpacing, fontSize, color);
            lineCount++;
            lineLength = 0;
            line[0] = '\0';
            text++;
            continue;
        }

        if (*text == '\0') break;

        const char *word = text;
        while (*text != '\0' && *text != ' ' && *text != '\t' && *text != '\n') text++;
        size_t wordLength = (size_t)(text - word);

        size_t candidateLength = lineLength;
        if (candidateLength > 0) candidate[candidateLength++] = ' ';
        memcpy(candidate + candidateLength, word, wordLength);
        candidateLength += wordLength;
        candidate[candidateLength] = '\0';

        if (lineLength > 0 && MeasureText(candidate, fontSize) > maxWidth) {
            DrawText(line, x, y + lineCount * lineSpacing, fontSize, color);
            lineCount++;
            lineLength = 0;
            line[0] = '\0';
            candidateLength = wordLength;
            memcpy(candidate, word, wordLength);
            candidate[candidateLength] = '\0';
        }

        memcpy(line, candidate, candidateLength + 1);
        lineLength = candidateLength;
    }

    if (lineLength > 0) {
        DrawText(line, x, y + lineCount * lineSpacing, fontSize, color);
        lineCount++;
    }

    return lineCount;
}

static Texture2D LoadCharacterSprite(const char *path)
{
    if (!FileExists(path)) {
        TraceLog(LOG_WARNING, "Character sprite not found: %s", path);
        return (Texture2D){0};
    }

    return LoadTexture(path);
}

static Texture2D *GetCharacterSprite(const char *speaker, Texture2D *circe, Texture2D *perse,
                                     Texture2D *helios, Texture2D *sailor)
{
    if (strcmp(speaker, "Circe") == 0) return circe;
    if (strcmp(speaker, "Perse") == 0) return perse;
    if (strcmp(speaker, "Helios") == 0) return helios;
    if (strcmp(speaker, "Sailor") == 0) return sailor;
    return NULL;
}

static void DrawCharacterSprite(const Texture2D *sprite)
{
    if (sprite == NULL || sprite->id == 0 || sprite->width <= 0 || sprite->height <= 0) return;

    float scaleX = 290.0f / sprite->width;
    float scaleY = 320.0f / sprite->height;
    float scale = scaleX < scaleY ? scaleX : scaleY;
    float width = sprite->width * scale;
    float height = sprite->height * scale;
    Rectangle source = {0, 0, (float)sprite->width, (float)sprite->height};
    Rectangle destination = {800.0f - width - 30.0f, 330.0f - height, width, height};

    DrawTexturePro(*sprite, source, destination, (Vector2){0, 0}, 0, WHITE);
}

int main(void)
{
    if (!LoadDialogue()) return 1;

    InitWindow(800, 600, "Orator's Tale: Circe");
    Texture2D title_background = LoadTexture("assets/titlebackground.png"); // Title screen
    Texture2D start_background = LoadTexture("assets/startbg.png"); // Start screen
    Texture2D tier2_background = LoadTexture("assets/tier2bg.png"); // Tier 2 screen
    Texture2D castle_background = LoadTexture("assets/castlebg.png"); // Castle screen
    Texture2D tier3_background = LoadTexture("assets/tier3bg.png"); // Tier 3 screen
    Texture2D circe_sprite = LoadCharacterSprite("assets/sprites/circe.png");
    Texture2D perse_sprite = LoadCharacterSprite("assets/sprites/perse.png");
    Texture2D helios_sprite = LoadCharacterSprite("assets/sprites/helios.png");
    Texture2D sailor_sprite = LoadCharacterSprite("assets/sprites/sailor.png");
    SetTargetFPS(30);

    int currentNode = tier_1_1;
    int currentPlant = none;
    Screen screen = TITLE_SCREEN;
    int shouldQuit = 0;

    while (!WindowShouldClose() && !shouldQuit)
    {
        BeginDrawing();
        ClearBackground((Color){35, 45, 70, 255});

        if (screen == TITLE_SCREEN)
        {
            DrawTexture(title_background, 0, 0, WHITE);
            DrawText("Orator's Tale: Circe", (800 - MeasureText("Orator's Tale: Circe", 42))/2, 150, 42, RAYWHITE);

            if (GuiButton((Rectangle){310, 270, 180, 50}, "Start"))
            {
                currentNode = tier_1_1;
                screen = STORY_SCREEN;
            }

            if (GuiButton((Rectangle){310, 340, 180, 50}, "Quit"))
                shouldQuit = 1;
        }
        else
        {
            const Node *node = &story[currentNode];

            const char *bg = node->background;

            if (strcmp(bg, "start") == 0) DrawTexture(start_background, 0, 0, WHITE);
            else if (strcmp(bg, "tier2") == 0) DrawTexture(tier2_background, 0, 0, WHITE);
            else if (strcmp(bg, "tier3") == 0) DrawTexture(tier3_background, 0, 0, WHITE);
            else if (strcmp(bg, "castle") == 0) DrawTexture(castle_background, 0, 0, WHITE);

            DrawCharacterSprite(GetCharacterSprite(node->speaker, &circe_sprite, &perse_sprite,
                                                  &helios_sprite, &sailor_sprite));

            DrawRectangle(30, 330, 740, 250, (Color){15, 18, 30, 235});
            DrawText(node->speaker, 60, 345, 24, SKYBLUE);
            int dialogueLines = DrawWrappedText(node->line, 60, 378, 680, 20, 22, RAYWHITE);

            if (node->choiceCount > 0) {
                int choiceY = 388 + dialogueLines * 22;
                for (int i = 0; i < node->choiceCount; i++) {
                    Rectangle button = {60, choiceY + i * 40, 680, 34};
                    if (GuiButton(button, node->choices[i].text)) {
                        int nextNode = node->choices[i].next;
                        if (nextNode == END) {
                            currentNode = tier_1_1;
                            screen = TITLE_SCREEN;
                        } else {
                            currentNode = nextNode;
                        }
                        break;
                    }
                }
            } else if (node->next == END) {
                if (GuiButton((Rectangle){60, 525, 320, 40}, "Restart Story")) {
                    currentNode = tier_1_1;
                    screen = STORY_SCREEN;
                }

                if (GuiButton((Rectangle){420, 525, 320, 40}, "Title Screen")) {
                    currentNode = tier_1_1;
                    screen = TITLE_SCREEN;
                }
            } else if (node->next >= 0) {
                if (GuiButton((Rectangle){570, 525, 180, 40}, "Continue"))
                    currentNode = node->next;
            }
        }
        EndDrawing();
    }
    if (circe_sprite.id != 0) UnloadTexture(circe_sprite);
    if (perse_sprite.id != 0) UnloadTexture(perse_sprite);
    if (helios_sprite.id != 0) UnloadTexture(helios_sprite);
    if (sailor_sprite.id != 0) UnloadTexture(sailor_sprite);
    CloseWindow();
    return 0;
}