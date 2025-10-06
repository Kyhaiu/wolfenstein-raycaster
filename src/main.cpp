#include <imgui/imgui.h>
#include <imgui-sdl2/imgui_impl_sdl2.h>
#include <imgui-sdl2/imgui_impl_sdlrenderer2.h>
#include <SDL.h>
#include <iostream>

#define SDL_main main

#define WIDTH 1366
#define HEIGHT 768
#define MAP_X_SIZE 8
#define MAP_Y_SIZE 8
#define WALL_2D_SIZE 96
#define PI 3.141592635

float py, px;    // Posição do jogador
float pdx, pdy;  // Vetor de direção do jogador
float pa = 0.0f; // Angulo do Jogador (Olhando para a direita por padrão)

int map2D[MAP_X_SIZE][MAP_Y_SIZE] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}};

static inline float degToRad(float d) { return float(d * (PI / 180.0f)); }
static inline float FixAng(float a)
{
  if (a > 359)
    a -= 360;
  if (a < 0)
    a += 360;
  return a;
}

static inline float projDistance(float ax, float ay, float bx, float by, float ang)
{
  // mesma fórmula que você mostrou: projeção no vetor do ângulo
  return cosf(degToRad(ang)) * (bx - ax) - sinf(degToRad(ang)) * (by - ay);
}

void drawRays2D(SDL_Renderer *renderer)
{
  const int numRays = 120;
  const float fov = float(60.0f * (PI / 180.0f)); // FOV em radianos
  float ra = pa - fov / 2.0f;                     // raio inicial

  for (int r = 0; r < numRays; r++)
  {
    // Corrigir ângulo
    if (ra < 0)
      ra += float(2 * PI);
    if (ra > 2 * PI)
      ra -= float(2 * PI);

    float Tan = tanf(ra);
    float rx, ry, disV = 1e6f, disH = 1e6f;
    float vx = 0, vy = 0, hx = 0, hy = 0;

    // --- VERIFICAÇÃO VERTICAL ---
    {
      int dof = 0;
      float stepX, stepY;

      if (cosf(ra) > 0.001f)
      { // direita
        rx = float(((int)(px / WALL_2D_SIZE) * WALL_2D_SIZE) + WALL_2D_SIZE);
        ry = py + (rx - px) * Tan;
        stepX = WALL_2D_SIZE;
        stepY = stepX * Tan;
      }
      else if (cosf(ra) < -0.001f)
      { // esquerda
        rx = ((int)(px / WALL_2D_SIZE) * WALL_2D_SIZE) - 0.0001f;
        ry = py + (rx - px) * Tan;
        stepX = -WALL_2D_SIZE;
        stepY = stepX * Tan;
      }
      else
      { // olhando para cima/baixo (sem interseção vertical)
        rx = px;
        ry = py;
        dof = MAP_X_SIZE;
      }

      while (dof < MAP_X_SIZE)
      {
        int mx = (int)(rx) / WALL_2D_SIZE;
        int my = (int)(ry) / WALL_2D_SIZE;
        if (mx >= 0 && mx < MAP_X_SIZE && my >= 0 && my < MAP_Y_SIZE)
        {
          if (map2D[my][mx] == 1)
          {
            vx = rx;
            vy = ry;
            disV = (px - vx) * (px - vx) + (py - vy) * (py - vy);
            break;
          }
          else
          {
            rx += stepX;
            ry += stepY;
            dof++;
          }
        }
        else
          break;
      }
    }

    // --- VERIFICAÇÃO HORIZONTAL ---
    {
      int dof = 0;
      float stepX, stepY;
      float aTan = 1.0f / Tan;

      if (sinf(ra) > 0.001f)
      { // para baixo
        ry = float(((int)(py / WALL_2D_SIZE) * WALL_2D_SIZE) + WALL_2D_SIZE);
        rx = px + (ry - py) * aTan;
        stepY = WALL_2D_SIZE;
        stepX = stepY * aTan;
      }
      else if (sinf(ra) < -0.001f)
      { // para cima
        ry = ((int)(py / WALL_2D_SIZE) * WALL_2D_SIZE) - 0.0001f;
        rx = px + (ry - py) * aTan;
        stepY = -WALL_2D_SIZE;
        stepX = stepY * aTan;
      }
      else
      { // olhando direto pra esquerda/direita
        rx = px;
        ry = py;
        dof = MAP_Y_SIZE;
      }

      while (dof < MAP_Y_SIZE)
      {
        int mx = (int)(rx) / WALL_2D_SIZE;
        int my = (int)(ry) / WALL_2D_SIZE;
        if (mx >= 0 && mx < MAP_X_SIZE && my >= 0 && my < MAP_Y_SIZE)
        {
          if (map2D[my][mx] == 1)
          {
            hx = rx;
            hy = ry;
            disH = (px - hx) * (px - hx) + (py - hy) * (py - hy);
            break;
          }
          else
          {
            rx += stepX;
            ry += stepY;
            dof++;
          }
        }
        else
          break;
      }
    }

    // Escolhe interseção mais próxima
    float finalX, finalY;
    if (disV < disH)
    {
      finalX = vx;
      finalY = vy;
      SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255);
    }
    else
    {
      finalX = hx;
      finalY = hy;
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    }

    // Desenhar o raio
    SDL_RenderDrawLine(renderer, (int)px, (int)py, (int)finalX, (int)finalY);

    // Próximo raio
    ra += fov / numRays;
  }
}

void drawPlayer(SDL_Renderer *renderer)
{
  // Desenha o jogador como um quadrado
  SDL_Rect rect = {int(px) - 2, int(py) - 2, 5, 5}; // Centralizar o quadrado
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // azul
  SDL_RenderFillRect(renderer, &rect);

  // Desenha a linha de direção
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // vermelho
  SDL_RenderDrawLine(renderer,
                     int(px), int(py),
                     int(px + pdx * 10), int(py + pdy * 10)); // Multiplicamos por 10 para alongar a linha
}

void drawMap(SDL_Renderer *renderer)
{
  for (int y = 0; y < MAP_X_SIZE; y++)
  {
    for (int x = 0; x < MAP_Y_SIZE; x++)
    {
      if (map2D[y][x] == 1)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

      // Subtrai apenas 1 pixel para grade mais sutil
      SDL_Rect cell = {
          x * WALL_2D_SIZE + 1,
          y * WALL_2D_SIZE + 1,
          WALL_2D_SIZE - 1,
          WALL_2D_SIZE - 1};
      SDL_RenderFillRect(renderer, &cell);
    }
  }
}

void movePlayer(const Uint8 *keystates, float &px, float &py, float speed, float deltaTime)
{
  const float rotSpeed = 2.0f * deltaTime; // 1.5 rad/s

  if (keystates[SDL_SCANCODE_A])
  {
    pa -= rotSpeed;
    if (pa < 0)
      pa += float(2 * PI);
    pdx = cosf(pa);
    pdy = sinf(pa);
  }

  if (keystates[SDL_SCANCODE_D])
  {
    pa += rotSpeed;
    if (pa > 2 * PI)
      pa -= float(2 * PI);
    pdx = cosf(pa);
    pdy = sinf(pa);
  }

  if (keystates[SDL_SCANCODE_W])
  {
    px += pdx * speed;
    py += pdy * speed;
  }

  if (keystates[SDL_SCANCODE_S])
  {
    px -= pdx * speed;
    py -= pdy * speed;
  }
}

int main()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Raycast engine",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        1366, 768,
                                        SDL_WINDOW_SHOWN);
  if (!window)
  {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
  {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  // Initializing the game values
  px = 300;
  py = 300;

  // Main loop
  bool running = true;
  SDL_Event event;
  const float playerSpeed = .2f;
  Uint32 lastTime = SDL_GetTicks();

  while (running)
  {

    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f; // em segundos
    lastTime = currentTime;

    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        running = false;
    }

    // Pegando o estado do teclado
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    movePlayer(keystates, px, py, playerSpeed, deltaTime);

    // Limpar a tela
    SDL_SetRenderDrawColor(renderer, 78, 78, 78, 255);
    SDL_RenderClear(renderer);

    // Desenha o mapa
    drawMap(renderer);

    // Desenhar jogador
    drawPlayer(renderer);

    // Desenha os raios
    drawRays2D(renderer);

    // Atualizar tela
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}