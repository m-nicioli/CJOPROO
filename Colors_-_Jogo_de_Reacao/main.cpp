#include "raylib.h"
#include <vector>
#include <string>

using namespace std;

// -----------------------------------------------------------------------------------------------------

// Estados do jogo
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

// Classe Botão
class Button {
public:
    Rectangle bounds;
    Color color;
    string label;

    Button(float x, float y, float width, float height, Color color, string label = "") {
        bounds = {x, y, width, height};
        this->color = color;
        this->label = label;
    }

    void Draw() {
        DrawRectangleRec(bounds, color);
        if (!label.empty()) {
            int textWidth = MeasureText(label.c_str(), 20);
            DrawText(label.c_str(), bounds.x + (bounds.width - textWidth) / 2, bounds.y + 10, 20, BLACK);
        }
    }

    bool IsClicked(Vector2 mousePoint) {
        return CheckCollisionPointRec(mousePoint, bounds);
    }
};

// Classe Gerenciador de Jogo
class GameManager {
public:
    vector<Button> buttons;
    Color targetColor;
    int score;
    float timeLeft;
    GameState state;
    Sound gameOverSound;

    GameManager() {
        score = 0;
        timeLeft = 8.0f;
        state = MENU; // Começa no menu
        InitializeButtons();
        ChooseTargetColor();
        
    }
    // -------------------------------------------------------
    void InitializeButtons() {
        buttons.clear();
        int screenWidth = GetScreenWidth();
        int buttonWidth = 100;
        int buttonHeight = 100;
        int spacing = 20;
        int startX = (screenWidth - (buttonWidth * 4 + spacing * 3)) / 2;
        

        // Cores iniciais
        vector<Color> colors = {RED, BLUE, GREEN, YELLOW, PURPLE, ORANGE, PINK, SKYBLUE, DARKGRAY, DARKPURPLE, BROWN, BEIGE};;

        // Ajustar as cores na tela
        for (std::vector<Color>::size_type i = 0; i < colors.size(); i++) {
            int x = startX + (i % 4) * (buttonWidth + spacing);
            int y = 150 + (i / 4) * (buttonHeight + spacing); 
            buttons.push_back(Button(x, y, buttonWidth, buttonHeight, colors[i]));
        }
        randomizeButtons(); 
    }
    // -------------------------------------------------------
    void ChooseTargetColor() {
        if (!buttons.empty()) {
            int randomIndex = GetRandomValue(0, buttons.size() - 1);
            targetColor = buttons[randomIndex].color;
        }
    }
    // -------------------------------------------------------
    void Update() {

        if (state == PLAYING) {
            timeLeft -= GetFrameTime();
            if (timeLeft <= 0) {
                PlaySound(gameOverSound); // Toca o efeito sonoro de game over
                state = GAME_OVER; // Fim de jogo
            }
        }
    }

    // -------------------------------------------------------
    void randomizeButtons() {
        // Embaralha as cores dos botões
        vector<Color> colors = {RED, BLUE, GREEN, YELLOW, PURPLE, ORANGE, PINK, SKYBLUE, DARKGRAY, DARKPURPLE, BROWN, BEIGE};
        for (int i = 0; i < buttons.size(); i++) {
            int randomIndex = GetRandomValue(0, colors.size() - 1);
            buttons[i].color = colors[randomIndex];
            colors.erase(colors.begin() + randomIndex); // Remove a cor escolhida para não repetir
        }
    }
     // -------------------------------------------------------
    void Draw() {
        if (state == MENU) {
            // Desenha o título centralizado
            DrawText("Colors - Jogo de Reação com Cores", GetScreenWidth() / 2 - MeasureText("Colors - Jogo de Reação com Cores", 40) / 2, GetScreenHeight() / 2 - 100, 40, BLACK);
            DrawText("Pressione ESPAÇO para começar", GetScreenWidth() / 2 - MeasureText("Pressione ESPAÇO para começar", 20) / 2, GetScreenHeight() / 2 - 30, 20, BLACK);

            // Botão "Sair" centralizado
            Button quitButton(GetScreenWidth() / 2 - 50, GetScreenHeight() / 2 + 50, 100, 40, RED, "Sair");
            quitButton.Draw();

            // Verifica clique no botão "Sair"
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePoint = GetMousePosition();
                if (quitButton.IsClicked(mousePoint)) {
                    CloseWindow();
                }
            }
        } else if (state == PLAYING) {
            // Desenha os botões
            for (auto& button : buttons) {
                button.Draw();
            }
            // Desenha a pontuação e o tempo restante
            DrawText(TextFormat("Pontuação: %d", score), 10, 10, 20, BLACK);
            DrawText(TextFormat("Tempo: %.1f", timeLeft), 10, 40, 20, BLACK);
            DrawText("Clique na cor:", 10, 70, 20, BLACK);
            DrawRectangle(200, 70, 30, 30, targetColor);
        } else if (state == GAME_OVER) {
            // Desenha a tela de game over
            DrawText("Fim de jogo!", GetScreenWidth() / 2 - MeasureText("Fim de jogo!", 40) / 2, GetScreenHeight() / 2 - 100, 40, RED);
            DrawText(TextFormat("Pontuação final: %d", score), GetScreenWidth() / 2 - MeasureText(TextFormat("Pontuação final: %d", score), 30) / 2, GetScreenHeight() / 2 - 50, 30, BLACK);

            // Botões centralizados
            Button restartButton(GetScreenWidth() / 2 - 120, GetScreenHeight() / 2 + 20, 100, 40, GREEN, "Reiniciar");
            Button menuButton(GetScreenWidth() / 2 + 20, GetScreenHeight() / 2 + 20, 100, 40, BLUE, "Menu");
            Button quitButton(GetScreenWidth() / 2 - 50, GetScreenHeight() / 2 + 80, 100, 40, RED, "Sair");

            restartButton.Draw();
            menuButton.Draw();
            quitButton.Draw();

            // Verifica clique nos botões
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePoint = GetMousePosition();
                if (restartButton.IsClicked(mousePoint)) {
                    *this = GameManager(); // Reinicia o jogo
                    state = PLAYING;
                } else if (menuButton.IsClicked(mousePoint)) {
                    *this = GameManager(); // Volta ao menu
                } else if (quitButton.IsClicked(mousePoint)) {
                    CloseWindow();
                }
            }
        }
    }
};
// --------------------------------------------------------------------------------------------------------------------
int main() {
    InitWindow(800, 600, "Colors - Jogo de Reação com Cores");
    InitAudioDevice();
    Sound gameOverSound = LoadSound("assets/zapsplat_game_sound_basic_digital_retro_incorrect_error_negative_007.mp3");
    Sound acertoSound = LoadSound("assets/zapsplat_game_sound_coin_collect_bright_simple_mild.mp3");
    SetSoundVolume(acertoSound, 0.5f);
    SetSoundVolume(gameOverSound, 0.5f);
    SetTargetFPS(60);

    GameManager gameManager;
    gameManager.gameOverSound = gameOverSound;

    while (!WindowShouldClose()) {
        gameManager.Update();

        if (gameManager.state == MENU) {
            if (IsKeyPressed(KEY_SPACE)) {
                gameManager.state = PLAYING; 
            }
        } else if (gameManager.state == PLAYING) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePoint = GetMousePosition();
                for (auto& button : gameManager.buttons) {
                    if (button.IsClicked(mousePoint)) {
                        if (button.color.r == gameManager.targetColor.r &&
                            button.color.g == gameManager.targetColor.g &&
                            button.color.b == gameManager.targetColor.b) {
                            gameManager.score++;
                            gameManager.timeLeft += 1.0f; // Ganha 1 segundo extra
                            gameManager.randomizeButtons();
                            gameManager.ChooseTargetColor();
                            PlaySound(acertoSound); // Toca o efeito sonoro de acerto
                        } else {
                            PlaySound(gameOverSound); // Toca o efeito sonoro de erro
                            gameManager.state = GAME_OVER; // Errou, fim de jogo
                        }
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        gameManager.Draw();

        EndDrawing();
    }

    UnloadSound(gameManager.gameOverSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}