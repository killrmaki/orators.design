#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "raylib.h"
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
        {{"Heal", tier_2c_1}, {"Enchant", tier_2a_1}, {"Command", tier_2b_1}},
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
// -----------------------------------------------------------------------------
// Orator's Tale: Circe -- Windows visual novel interface
// Original story[] and dialogue.md are preserved above.
// The herb associations below are FICTIONAL game symbolism, not medical advice.
// -----------------------------------------------------------------------------
#define GAME_WIDTH 1400
#define GAME_HEIGHT 900
typedef struct {
    const char *name;
    const char *botanical;
    const char *fact;
    const char *magic;
    const char *choice;
    Texture2D picture;
} HerbCard;
static void DrawImageFit(Texture2D img, Rectangle box, Color tint)
{
    if (img.id == 0 || img.width <= 0 || img.height <= 0) return;
    float sx = box.width / (float)img.width;
    float sy = box.height / (float)img.height;
    float scale = (sx < sy) ? sx : sy;
    float width = (float)img.width * scale;
    float height = (float)img.height * scale;
    DrawTexturePro(img, (Rectangle){0, 0, (float)img.width, (float)img.height},
        (Rectangle){box.x + (box.width - width)/2, box.y + (box.height - height)/2, width, height},
        (Vector2){0, 0}, 0.0f, tint);
}
static void DrawBackgroundCover(Texture2D img)
{
    if (img.id == 0) { ClearBackground((Color){40, 55, 56, 255}); return; }
    float sx = GAME_WIDTH/(float)img.width;
    float sy = GAME_HEIGHT/(float)img.height;
    float scale = (sx > sy) ? sx : sy;
    float srcW = GAME_WIDTH/scale;
    float srcH = GAME_HEIGHT/scale;
    DrawTexturePro(img, (Rectangle){(img.width-srcW)/2, (img.height-srcH)/2, srcW, srcH},
        (Rectangle){0, 0, GAME_WIDTH, GAME_HEIGHT}, (Vector2){0, 0}, 0.0f, WHITE);
}
static int DrawWrappedTextMax(const char *text, int x, int y, int maxWidth,
                              int fontSize, int lineSpacing, int maxLines, Color color)
{
    char line[MAX_DIALOGUE_LENGTH];
    char candidate[MAX_DIALOGUE_LENGTH];
    size_t count = 0;
    int lines = 0;
    line[0] = '\0';
    while (*text && lines < maxLines) {
        while (*text == ' ' || *text == '\t') text++;
        if (*text == '\n') {
            DrawText(line, x, y + lines*lineSpacing, fontSize, color);
            lines++; count = 0; line[0] = '\0'; text++;
            continue;
        }
        if (!*text) break;
        const char *word = text;
        while (*text && *text != ' ' && *text != '\t' && *text != '\n') text++;
        size_t wordLen = (size_t)(text-word);
        if (wordLen >= sizeof(line)) wordLen = sizeof(line)-1;
        size_t newLen = count + (count ? 1 : 0) + wordLen;
        if (newLen >= sizeof(candidate)) continue;
        if (count) {
            memcpy(candidate, line, count);
            candidate[count] = ' ';
            memcpy(candidate + count+1, word, wordLen);
        } else memcpy(candidate, word, wordLen);
        candidate[newLen] = '\0';
        if (count && MeasureText(candidate, fontSize) > maxWidth) {
            DrawText(line, x, y + lines*lineSpacing, fontSize, color);
            lines++;
            count = wordLen;
            memcpy(line, word, count);
            line[count] = '\0';
        } else {
            count = newLen;
            memcpy(line, candidate, newLen+1);
        }
    }
    if (count && lines < maxLines) {
        DrawText(line, x, y + lines*lineSpacing, fontSize, color);
        lines++;
    }
    return lines;
}
static int GameButton(Rectangle rec, const char *label)
{
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, rec);
    Color fill = hovered ? (Color){105, 91, 62, 245} : (Color){51, 59, 57, 245};
    DrawRectangleRounded(rec, 0.17f, 8, fill);
    DrawRectangleRoundedLinesEx(rec, 0.17f, 8, 2,
        hovered ? (Color){247, 217, 144, 255} : (Color){160, 166, 146, 255});
    int fontSize = 22;
    while (fontSize > 13 && MeasureText(label, fontSize) > (int)rec.width-22) fontSize--;
    DrawText(label, (int)(rec.x + (rec.width-MeasureText(label,fontSize))/2),
             (int)(rec.y + (rec.height-fontSize)/2), fontSize, RAYWHITE);
    return hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}
static Texture2D PickSpeakerTexture(const Node *node, Texture2D circe,
        Texture2D perse, Texture2D helios, Texture2D nymphs, Texture2D sailors)
{
    if (!strcmp(node->speaker, "Circe")) return circe;
    if (!strcmp(node->speaker, "Perse")) return perse;
    if (!strcmp(node->speaker, "Helios")) return helios;
    if (!strcmp(node->speaker, "Sailor")) return sailors;
    // Nymph illustration appears only in the forest scenes, not every narration.
    if (!strcmp(node->speaker, "Narrator") && !strcmp(node->background, "tier2")) return nymphs;
    return (Texture2D){0};
}
// Scene-specific rules: only show nymph companions after the herb decision.
// In the mercy route, remove them once the narrative describes their deaths.
static bool ShouldShowNymphs(int currentNode, int currentHerb)
{
    // Only appear in the sailor confrontation, never in the herb/early-island scenes.
    if (currentHerb < 0) return false;
    if (currentNode >= tier_3_6 && currentNode <= tier_3_c) return true;
    // Mercy route: they remain visible until the narrative's tragic aftermath.
    if (currentNode >= tier_3a_1 && currentNode <= tier_3a_2) return true;
    if (currentNode >= tier_3b_1 && currentNode <= end_c) return true;
    return false;
}
static bool AreSailorsTransformed(int currentNode)
{
    // The 'turn them into pigs' decision leads to tier_3b_1.
    // This intentionally does not include any mercy nodes.
    return currentNode >= tier_3b_1 && currentNode <= end_c;
}
static bool ShouldShowSailors(int currentNode)
{
    // Before the decision, they are human; they remain human on the mercy route.
    return (currentNode >= tier_3_6 && currentNode <= tier_3_c) ||
           (currentNode >= tier_3a_1 && currentNode <= tier_3a_2);
}
// Position sprite groups within the world area ABOVE the dialogue panel (y < 645).
// Sailors/pigs left; nymphs right; Circe steps in front to protect them.
static bool IsSailorEncounter(int node)
{
    return (node >= tier_3_6 && node <= end_c);
}
static void DrawSceneCompanions(int currentNode, int currentHerb,
                                Texture2D nymphs, Texture2D sailors, Texture2D pigs,
                                Texture2D circe)
{
    if (!IsSailorEncounter(currentNode)) return;
    // Left: the sailors (or transformed pigs) sit low enough to overlap the
    // upper edge of the dialogue area; the panel is drawn afterward in front.
    if (AreSailorsTransformed(currentNode)) {
        DrawImageFit(pigs, (Rectangle){15, 340, 620, 330}, WHITE);
    } else if (ShouldShowSailors(currentNode)) {
        DrawImageFit(sailors, (Rectangle){10, 260, 620, 430}, WHITE);
    }
    // Right: nymphs stay grounded and slightly smaller, behind Circe.
    if (ShouldShowNymphs(currentNode, currentHerb)) {
        DrawImageFit(nymphs, (Rectangle){1090, 365, 300, 270}, WHITE);
    }
    // Circe protects them at front-right, near full portrait height.
    DrawImageFit(circe, (Rectangle){820, 70, 465, 575}, WHITE);
}
static void DrawSpeakerPortrait(const Node *node, Texture2D portrait)
{
    if (!portrait.id) return;
    bool left = !strcmp(node->speaker, "Circe");
    Rectangle area = left ? (Rectangle){32, 65, 520, 580}
                          : (Rectangle){920, 65, 465, 580};
    DrawImageFit(portrait, area, WHITE);
}
// Nobility measures alignment with Helios' authority, NOT kindness or morality.
// These are editable GAME DESIGN values; edit the numbers to rebalance the story.
// Every available choice has a defined delta, including neutral (0) choices.
static int NobilityDelta(int decisionNode, int option)
{
    switch (decisionNode) {
        case tier_1_c: return (option == 0) ? +1 : -1;  // Accept / defy expectations
        case tier_2_c: return (option == 0) ? +1 : (option == 1 ? 0 : -1); // Heal / Enchant / Command
        case tier_3_c: return (option == 0) ? +1 : -1;  // Mercy / transformation
        case tier_4_c: return (option == 0) ? -2 : +2; // Leave / stay under Helios
        default: return 0;
    }
}
static const char *NobilityReason(int decisionNode, int option)
{
    switch (decisionNode) {
        case tier_1_c: return option == 0 ? "Follows family expectations." : "Challenges family expectations.";
        case tier_2_c:
            return option == 0 ? "Healing aligns with a restrained use of power." :
                   option == 1 ? "Enchantment neither reinforces nor rejects his rule." :
                                 "Commanding magic asserts Circe's independent power.";
        case tier_3_c: return option == 0 ? "Shows restraint when using divine power." :
                                              "Uses transformative power without approval.";
        case tier_4_c: return option == 0 ? "Leaves Helios' direct authority." :
                                              "Remains under Helios' rule.";
        default: return "No change in allegiance.";
    }
}
static void ScoreLabel(int value, char *out, size_t count)
{
    if (value > 0) snprintf(out, count, "NOBILITY +%d", value);
    else if (value < 0) snprintf(out, count, "NOBILITY %d", value);
    else snprintf(out, count, "NOBILITY +0");
}
// An explicit acknowledgment panel ensures the player SEES the score change.
static bool DrawNobilityResult(int delta, int total, const char *reason)
{
    DrawRectangle(0, 0, GAME_WIDTH, GAME_HEIGHT, (Color){4, 8, 14, 195});
    DrawRectangleRounded((Rectangle){320, 230, 760, 425}, 0.07f, 14,
                         (Color){24, 34, 39, 250});
    DrawRectangleRoundedLinesEx((Rectangle){320, 230, 760, 425}, 0.07f, 14, 3,
                                (Color){229, 190, 118, 255});
    DrawText("YOUR CHOICE HAS CONSEQUENCES", 375, 280, 30, (Color){248, 220, 168, 255});
    char score[64]; ScoreLabel(delta, score, sizeof score);
    Color col = delta > 0 ? (Color){154, 230, 169, 255} :
                delta < 0 ? (Color){255, 157, 147, 255} : RAYWHITE;
    DrawText(score, (GAME_WIDTH-MeasureText(score, 47))/2, 365, 47, col);
    char totalLabel[96]; snprintf(totalLabel, sizeof totalLabel, "TOTAL NOBILITY: %+d", total);
    DrawText(totalLabel, (GAME_WIDTH-MeasureText(totalLabel, 29))/2, 445, 29, RAYWHITE);
    DrawWrappedTextMax(reason, 380, 505, 650, 20, 26, 2,
                       (Color){223, 221, 199, 255});
    return GameButton((Rectangle){530, 567, 340, 59}, "CONTINUE STORY >");
}
static int DrawHerbSelection(const HerbCard herbs[3])
{
    DrawRectangle(0, 0, GAME_WIDTH, GAME_HEIGHT, (Color){6, 17, 17, 105});
    DrawText("THE FOREST'S OFFERING", 70, 30, 39, (Color){246,227,180,255});
    DrawText("Choose one plant to shape Circe's reputation and magic.", 72, 79, 23, RAYWHITE);
    int chosen = -1;
    for (int i=0; i<3; i++) {
        Rectangle card = {(float)(64 + i*448), 142, 400, 585};
        bool hovered = CheckCollisionPointRec(GetMousePosition(),card);
        DrawRectangleRounded(card, 0.05f, 12, (Color){20,30,30,240});
        DrawRectangleRoundedLinesEx(card, 0.05f, 12, hovered?4:2,
             hovered?(Color){240,208,128,255}:(Color){137,156,129,255});
        Rectangle illustration = {card.x+40, card.y+14, 320, 288};
        if (herbs[i].picture.id) {
            DrawImageFit(herbs[i].picture, illustration, WHITE);
        } else {
            const char *missing = "Botanical artwork unavailable";
            DrawRectangleRounded(illustration, 0.04f, 8, (Color){30,43,38,255});
            DrawText(missing,
                     (int)(illustration.x + (illustration.width-MeasureText(missing,18))/2),
                     (int)(illustration.y + (illustration.height-18)/2),
                     18, (Color){188,218,185,255});
        }
        DrawText(herbs[i].name, (int)card.x+25, (int)card.y+312, 31,
                 (Color){252,226,161,255});
        DrawText(herbs[i].botanical, (int)card.x+25, (int)card.y+352, 20,
                 (Color){188,218,185,255});
        DrawWrappedTextMax(herbs[i].fact, (int)card.x+25, (int)card.y+388,
                           355, 19, 24, 3, RAYWHITE);
        DrawWrappedTextMax(herbs[i].magic, (int)card.x+25, (int)card.y+453,
                           355, 19, 23, 2, (Color){240,204,157,255});
        char score[64]; ScoreLabel(NobilityDelta(tier_2_c, i), score, sizeof score);
        DrawText(score, (int)card.x+27, (int)card.y+496, 22,
                 (Color){255, 224, 155, 255});
        if (GameButton((Rectangle){card.x+35,card.y+520,330,49},herbs[i].choice)) chosen=i;
    }
    DrawRectangle(0, 758, GAME_WIDTH, 142, (Color){13,19,23,245});
    DrawText("Nobility = obedience to Helios  |  Real plants / fictional magic", 65, 776, 22, (Color){248,222,167,255});
    DrawWrappedTextMax("Botanical notes are educational descriptions, not instructions for using or ingesting these plants. Some are poisonous. The magical effects belong only to this story.",
                       65, 810, 1240, 20, 26, 3, RAYWHITE);
    return chosen;
}
static Texture2D LoadOptionalTexture(const char *path)
{
    if (!FileExists(path)) {
        TraceLog(LOG_WARNING, "Optional texture not found: %s", path);
        return (Texture2D){0};
    }
    return LoadTexture(path);
}
int main(void)
{
    if (!LoadDialogue()) return 1;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Orator's Tale: Circe");
    SetTargetFPS(60);
    Texture2D title_background = LoadTexture("assets/titlebackground.png");
    Texture2D start_background = LoadTexture("assets/startbg.png");
    Texture2D tier2_background = LoadTexture("assets/tier2bg.png");
    Texture2D tier3_background = LoadTexture("assets/tier3bg.png");
    Texture2D castle_background = LoadTexture("assets/castlebg.png");
    Texture2D circe = LoadOptionalTexture("assets/sprites/circe.png");
    Texture2D perse = LoadOptionalTexture("assets/sprites/perse.png");
    Texture2D helios = LoadOptionalTexture("assets/sprites/helios.png");
    Texture2D nymphs = LoadOptionalTexture("assets/sprites/nymphs.png");
    Texture2D sailors = LoadOptionalTexture("assets/sprites/sailors.png");
    Texture2D pigs = LoadOptionalTexture("assets/sprites/pigs.png");
    Texture2D verbena = LoadOptionalTexture("assets/plants/verbena.png");
    Texture2D papaver = LoadOptionalTexture("assets/plants/papaver.png");
    Texture2D nicotiana = LoadOptionalTexture("assets/plants/nicotiana.png");
    Texture2D sachet = LoadOptionalTexture("assets/plants/herb_sachet.png");
    // Match each existing story[] choice: Heal, Enchant, Command.
    HerbCard herbs[3] = {
        {"HEAL", "Verbena officinalis", "A flowering perennial long associated with traditional herbal folklore.",
         "In this fantasy: protection, restoration, and compassion.", "Choose HEAL", verbena},
        {"ENCHANT", "Papaver somniferum", "The opium poppy, known for its distinctive flowers and seed pods.",
         "In this fantasy: dreams, illusions, and enchantment.", "Choose ENCHANT", papaver},
        {"COMMAND", "Nicotiana rustica", "A tobacco species containing highly toxic nicotine.",
         "In this fantasy: influence, control, and power.", "Choose COMMAND", nicotiana}
    };
    int currentNode = tier_1_1;
    int currentHerb = -1;
    int nobility = 0;
    int lastDelta = 0;
    const char *lastReason = "";
    bool showScoreResult = false;
    Screen screen = TITLE_SCREEN;
    int shouldQuit = 0;
    while (!WindowShouldClose() && !shouldQuit) {
        BeginDrawing();
        ClearBackground(BLACK);
        if (screen == TITLE_SCREEN) {
            DrawBackgroundCover(title_background);
            DrawRectangle(0,0,GAME_WIDTH,GAME_HEIGHT,(Color){6,14,20,125});
            DrawRectangleRounded((Rectangle){270,220,860,440},0.07f,16,(Color){15,22,26,224});
            const char *title = "ORATOR'S TALE: CIRCE";
            DrawText(title,(GAME_WIDTH-MeasureText(title,50))/2,275,50,(Color){248,225,176,255});
            DrawText("An interactive story shaped by your choices",377,351,25,RAYWHITE);
            if (GameButton((Rectangle){510,440,380,70},"START STORY")) {
                currentNode=tier_1_1;
                currentHerb=-1;
                nobility=0;
                showScoreResult=false;
                screen=STORY_SCREEN;
            }
            if (GameButton((Rectangle){510,532,380,60},"QUIT")) shouldQuit=1;
        } else {
            const Node *node = &story[currentNode];
            Texture2D bg = start_background;
            if (!strcmp(node->background,"tier2")) bg=tier2_background;
            else if (!strcmp(node->background,"tier3")) bg=tier3_background;
            else if (!strcmp(node->background,"castle")) bg=castle_background;
            DrawBackgroundCover(bg);
            if (currentNode == tier_2_c && !showScoreResult) {
                int chosen = DrawHerbSelection(herbs);
                if (chosen >= 0 && chosen < node->choiceCount) {
                    currentHerb=chosen;
                    lastDelta=NobilityDelta(currentNode, chosen);
                    lastReason=NobilityReason(currentNode, chosen);
                    nobility+=lastDelta;
                    currentNode=node->choices[chosen].next;
                    showScoreResult=true;
                }
            } else {
                DrawRectangle(0,0,GAME_WIDTH,85,(Color){12,19,26,180});
                DrawText("ORATOR'S TALE: CIRCE",36,24,29,(Color){241,222,179,255});
                char nobilityText[75];
                snprintf(nobilityText,sizeof nobilityText,"NOBILITY: %+d",nobility);
                DrawText(nobilityText,560,30,22,(Color){255,224,155,255});
                if (currentHerb>=0) {
                    char chosenText[90];
                    snprintf(chosenText,sizeof(chosenText),"Herb: %s",herbs[currentHerb].botanical);
                    DrawText(chosenText,865,30,21,(Color){226,235,190,255});
                    if (sachet.id) DrawImageFit(sachet,(Rectangle){1310,8,60,66},WHITE);
                }
                DrawSceneCompanions(currentNode, currentHerb, nymphs, sailors, pigs, circe);
                Texture2D foreground = PickSpeakerTexture(node,circe,perse,helios,nymphs,sailors);
                // Hide automatic narrator nymph sprite; the scene layer controls it.
                if (!strcmp(node->speaker, "Narrator") ||
                    !strcmp(node->speaker, "Sailor") ||
                    (IsSailorEncounter(currentNode) && !strcmp(node->speaker,"Circe")))
                    foreground = (Texture2D){0};
                DrawSpeakerPortrait(node, foreground);
                DrawRectangleRounded((Rectangle){30,645,1340,230},0.03f,12,(Color){11,16,24,241});
                DrawRectangleRoundedLinesEx((Rectangle){30,645,1340,230},0.03f,12,2,
                    (Color){174,154,120,255});
                DrawText(node->speaker,65,665,28,(Color){184,217,241,255});
                DrawWrappedTextMax(node->line,65,710,890,23,31,5,RAYWHITE);
                if (!showScoreResult && node->choiceCount>0) {
                    DrawText("CHOICE / NOBILITY", 992, 656, 18, (Color){247,220,166,255});
                    for (int i=0;i<node->choiceCount;i++) {
                        Rectangle button = {990,(float)(688 + i*62),345,52};
                        int delta = NobilityDelta(currentNode,i);
                        char choiceLabel[160]; char shortScore[40];
                        ScoreLabel(delta,shortScore,sizeof shortScore);
                        snprintf(choiceLabel,sizeof choiceLabel,"%s  (%+d Nobility)",node->choices[i].text,delta);
                        if (GameButton(button,choiceLabel)) {
                            int next=node->choices[i].next;
                            lastDelta=delta;
                            lastReason=NobilityReason(currentNode,i);
                            nobility+=delta;
                            if (next==END) screen=TITLE_SCREEN;
                            else currentNode=next;
                            showScoreResult=true;
                            break;
                        }
                    }
                } else if (!showScoreResult && node->next == END) {
                    if (GameButton((Rectangle){730,795,285,57},"RESTART STORY")) {
                        currentNode=tier_1_1;
                        currentHerb=-1;
                        nobility=0;
                        lastDelta=0;
                        lastReason="";
                    }
                    if (GameButton((Rectangle){1050,795,285,57},"BACK TO TITLE")) {
                        currentNode=tier_1_1;
                        currentHerb=-1;
                        nobility=0;
                        lastDelta=0;
                        lastReason="";
                        screen=TITLE_SCREEN;
                    }
                } else if (!showScoreResult && node->next>=0) {
                    if (GameButton((Rectangle){1080,795,255,57},"CONTINUE  >"))
                        currentNode=node->next;
                }
            }
            if (showScoreResult && DrawNobilityResult(lastDelta, nobility, lastReason))
                showScoreResult = false;
        }
        EndDrawing();
    }
    if (title_background.id) UnloadTexture(title_background);
    if (start_background.id) UnloadTexture(start_background);
    if (tier2_background.id) UnloadTexture(tier2_background);
    if (tier3_background.id) UnloadTexture(tier3_background);
    if (castle_background.id) UnloadTexture(castle_background);
    if (circe.id) UnloadTexture(circe);
    if (perse.id) UnloadTexture(perse);
    if (helios.id) UnloadTexture(helios);
    if (nymphs.id) UnloadTexture(nymphs);
    if (sailors.id) UnloadTexture(sailors);
    if (pigs.id) UnloadTexture(pigs);
    if (verbena.id) UnloadTexture(verbena);
    if (papaver.id) UnloadTexture(papaver);
    if (nicotiana.id) UnloadTexture(nicotiana);
    if (sachet.id) UnloadTexture(sachet);
    CloseWindow();
    return 0;
}
