#include "raylib.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define TRAIL_LENGTH 250 // Lunghezza della traccia

int main(void) {
    // Inizializzazione della finestra
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Simulazione Doppio Pendolo - Raylib");
    SetTargetFPS(60);
    srand(time(NULL));

    // Variabili fisiche del doppio pendolo
    float r1 = 150.0f; // Lunghezza primo braccio
    float r2 = 150.0f; // Lunghezza secondo braccio
    float m1 = 20.0f;  // Massa primo pendolo
    float m2 = 20.0f;  // Massa secondo pendolo

    float theta1 = 0.0f;
    float theta2 = 0.0f;
    float w1 = 0.0f; // Velocità angolare 1
    float w2 = 0.0f; // Velocità angolare 2
    float g = 1.0f;  // Gravità

    // Origine (perno centrale)
    Vector2 origin = {screenWidth / 2.0f, screenHeight / 3.0f};

    // Array per la traccia
    Vector2 trail[TRAIL_LENGTH] = {0};
    int trailCount = 0;

    // Inizializzazione casuale al primo avvio
    theta1 = ((float)rand() / RAND_MAX) * PI * 2.0f;
    theta2 = ((float)rand() / RAND_MAX) * PI * 2.0f;

    while (!WindowShouldClose()) {
        // --- INPUT ---
        if (IsKeyPressed(KEY_SPACE)) {
            // Reset con angoli casuali e velocità azzerate
            theta1 = ((float)rand() / RAND_MAX) * PI * 2.0f;
            theta2 = ((float)rand() / RAND_MAX) * PI * 2.0f;
            w1 = 0.0f;
            w2 = 0.0f;
            trailCount = 0; // Svuota la traccia
        }

        // --- FISICA ---
        // Formule lagrangiane per le accelerazioni angolari del doppio pendolo
        float num1 = -g * (2 * m1 + m2) * sinf(theta1);
        float num2 = -m2 * g * sinf(theta1 - 2 * theta2);
        float num3 = -2 * sinf(theta1 - theta2) * m2;
        float num4 = w2 * w2 * r2 + w1 * w1 * r1 * cosf(theta1 - theta2);
        float den = r1 * (2 * m1 + m2 - m2 * cosf(2 * theta1 - 2 * theta2));
        float a1 = (num1 + num2 + num3 * num4) / den;

        num1 = 2 * sinf(theta1 - theta2);
        num2 = (w1 * w1 * r1 * (m1 + m2));
        num3 = g * (m1 + m2) * cosf(theta1);
        num4 = w2 * w2 * r2 * m2 * cosf(theta1 - theta2);
        den = r2 * (2 * m1 + m2 - m2 * cosf(2 * theta1 - 2 * theta2));
        float a2 = (num1 * (num2 + num3 + num4)) / den;

        // Aggiornamento velocità angolari
        w1 += a1;
        w2 += a2;

        // Aggiungiamo un leggerissimo smorzamento (attrito) per stabilità visiva
        w1 *= 0.999f;
        w2 *= 0.999f;

        // Aggiornamento angoli
        theta1 += w1;
        theta2 += w2;

        // Calcolo posizioni (X, Y)
        Vector2 pos1 = {origin.x + r1 * sinf(theta1), origin.y + r1 * cosf(theta1)};

        Vector2 pos2 = {pos1.x + r2 * sinf(theta2), pos1.y + r2 * cosf(theta2)};

        // --- AGGIORNAMENTO TRACCIA ---
        // Sposta tutti i punti vecchi in avanti nell'array
        for (int i = TRAIL_LENGTH - 1; i > 0; i--) {
            trail[i] = trail[i - 1];
        }
        trail[0] = pos2; // Inserisci la nuova posizione in testa
        if (trailCount < TRAIL_LENGTH)
            trailCount++;

        // --- DISEGNO ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Premi SPAZIO per resettare con posizioni casuali", 10, 10, 20, DARKGRAY);

        // Disegna la traccia che si dissolve
        for (int i = 0; i < trailCount - 1; i++) {
            // Calcola l'opacità (alpha) basata sull'età del punto (i)
            float alpha = 1.0f - ((float)i / TRAIL_LENGTH);

            // Mappa l'alpha da [0.0 - 1.0] a [0 - 255]
            Color trailColor = Fade(RED, alpha);

            // Disegna le linee di connessione per la traccia
            DrawLineEx(trail[i], trail[i + 1], 3.0f, trailColor);
        }

        // Disegna il primo braccio e il primo peso
        DrawLineEx(origin, pos1, 4.0f, BLACK);
        DrawCircleV(pos1, m1 / 1.5f, BLACK);

        // Disegna il secondo braccio e il secondo peso
        DrawLineEx(pos1, pos2, 4.0f, BLACK);
        DrawCircleV(pos2, m2 / 1.5f, BLACK);

        // Disegna il perno centrale
        DrawCircleV(origin, 8.0f, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
