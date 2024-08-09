#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Cat n Dog";

enum class Turn { CAT, DOG };

double mouse_x1, mouse_y1, mouse_x2, mouse_y2, theta, delta_x, delta_y, force;
bool shot = false;

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
SDL_Texture* loadTexture(string path, SDL_Renderer* renderer);

void angle() {
    delta_x = mouse_x2 - mouse_x1;
    delta_y = mouse_y2 - mouse_y1;

    force = std::sqrt(std::pow(delta_x, 2) + std::pow(delta_y, 2));

    if (delta_x != 0) {
        theta = std::atan2(delta_y, delta_x) * (180.0 / M_PI);
    } else {
        theta = 0;
    }

    if (force > 5) { // Limit to avoid very small force
        shot = true;
    }
}

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return (a.x + a.w > b.x &&
            a.x < b.x + b.w &&
            a.y + a.h > b.y &&
            a.y < b.y + b.h);
}

struct Projectile {
    double x, y;
    double vx, vy;
    SDL_Texture* texture;
    SDL_Rect rect;
};

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);

    SDL_Texture* background = loadTexture("background.png", renderer);
    SDL_Texture* catIdle = loadTexture("CAT.png", renderer);
    SDL_Texture* catPrepare = loadTexture("cat_prepare.png", renderer);
    SDL_Texture* catThrow = loadTexture("cat_throw.png", renderer);
    SDL_Texture* catHit = loadTexture("cat_hit.png", renderer); // Cat hit texture
    SDL_Texture* catLaugh = loadTexture("cat_laugh.png", renderer); // Cat laugh texture
    SDL_Texture* dogIdle = loadTexture("DOG.png", renderer);
    SDL_Texture* dogPrepare = loadTexture("dog_prepare.png", renderer);
    SDL_Texture* dogThrow = loadTexture("dog_throw.png", renderer);
    SDL_Texture* dogHit = loadTexture("dog_hit.png", renderer); // Dog hit texture
    SDL_Texture* dogLaugh = loadTexture("dog_laugh.png", renderer); // Dog laugh texture
    SDL_Texture* wallTexture = loadTexture("wall.png", renderer);

    SDL_Rect catRect;
    SDL_QueryTexture(catIdle, NULL, NULL, &catRect.w, &catRect.h);
    catRect.x = 20;
    catRect.y = 345;

    SDL_Rect dogRect;
    SDL_QueryTexture(dogIdle, NULL, NULL, &dogRect.w, &dogRect.h);
    dogRect.x = 600;
    dogRect.y = 345;

    // Define the floor level
    const int floorY = dogRect.y + dogRect.h / 2 + 40;

    // Define the wall object in the middle of the screen
    SDL_Rect wallRect;
    wallRect.w = 45;
    wallRect.h = 310;
    wallRect.x = SCREEN_WIDTH / 2 - wallRect.w / 2;
    wallRect.y = SCREEN_HEIGHT / 2 - wallRect.h / 2+150;

    SDL_Event e;
    bool quit = false;
    Projectile projectile;
    bool projectileInFlight = false;
    bool missed = false;
    Uint32 lastTime = SDL_GetTicks();
    Turn turn = Turn::CAT;

    SDL_Texture* currentCatTexture = catIdle;
    SDL_Texture* currentDogTexture = dogIdle;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                mouse_x1 = e.button.x;
                mouse_y1 = e.button.y;
                if (turn == Turn::CAT) {
                    currentCatTexture = catPrepare;
                } else {
                    currentDogTexture = dogPrepare;
                }
            } else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                mouse_x2 = e.button.x;
                mouse_y2 = e.button.y;
                angle();
                if (turn == Turn::CAT) {
                    currentCatTexture = catThrow;
                } else {
                    currentDogTexture = dogThrow;
                }
                if (shot) {
                    if (turn == Turn::CAT) {
                        projectile.x = catRect.x + catRect.w - 90;
                        projectile.y = catRect.y + catRect.h / 2 - 70;
                        projectile.texture = loadTexture("Can.png", renderer);  // Cat uses Can.png
                    } else {
                        projectile.x = dogRect.x + 90;
                        projectile.y = dogRect.y + dogRect.h / 2 - 70;
                        projectile.texture = loadTexture("Bone.png", renderer);  // Dog uses Bone.png
                    }
                    projectile.vx = std::cos(theta * M_PI / 180.0) * force / 10;
                    projectile.vy = std::sin(theta * M_PI / 180.0) * force / 10;

                    SDL_QueryTexture(projectile.texture, NULL, NULL, &projectile.rect.w, &projectile.rect.h);
                    projectileInFlight = true;
                }
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - lastTime) / 1000.0;
        lastTime = currentTime;

        if (projectileInFlight) {
            // Update projectile position
            projectile.vy += 9.81 * deltaTime; // Apply gravity
            projectile.x += projectile.vx;
            projectile.y += projectile.vy;
            projectile.rect.x = static_cast<int>(projectile.x);
            projectile.rect.y = static_cast<int>(projectile.y);

            bool hit = false;

            // Check collision with Dog
            if (turn == Turn::CAT && checkCollision(projectile.rect, dogRect)) {
                currentDogTexture = dogHit;
                hit = true;
                projectileInFlight = false;
                SDL_DestroyTexture(projectile.texture);
                projectile.texture = nullptr;

                if (turn == Turn::CAT) {
                    turn = Turn::DOG;
                    currentCatTexture = catIdle;
                } else {
                    turn = Turn::CAT;
                    currentDogTexture = dogIdle;
                }
            }
            // Check collision with Cat
            else if (turn == Turn::DOG && checkCollision(projectile.rect, catRect)) {
                currentCatTexture = catHit;
                hit = true;
                projectileInFlight = false;
                SDL_DestroyTexture(projectile.texture);
                projectile.texture = nullptr;

                if (turn == Turn::CAT) {
                    turn = Turn::DOG;
                    currentCatTexture = catIdle;
                } else {
                    turn = Turn::CAT;
                    currentDogTexture = dogIdle;
                }
            }
            // Check collision with the wall
            else if (checkCollision(projectile.rect, wallRect)) {
                // Simple ricochet logic
                projectile.vx = -projectile.vx; // Reverse X direction
                projectile.vy = -projectile.vy; // Reverse Y direction
            }
            // Check if projectile hits the floor
            else if (projectile.y > floorY) {
                projectileInFlight = false;
                SDL_DestroyTexture(projectile.texture);
                projectile.texture = nullptr;

                // Switch turn and show laughing texture
                if (turn == Turn::CAT) {
                    turn = Turn::DOG;
                    currentDogTexture = dogLaugh;
                } else {
                    turn = Turn::CAT;
                    currentCatTexture = catLaugh;
                }
            }
            // If hit, reset projectile
        }

        // Render everything
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        SDL_RenderCopy(renderer, wallTexture, NULL, &wallRect); // Draw the wall
        SDL_RenderCopy(renderer, currentCatTexture, NULL, &catRect);
        SDL_RenderCopy(renderer, currentDogTexture, NULL, &dogRect);

        if (projectileInFlight) {
            SDL_RenderCopy(renderer, projectile.texture, NULL, &projectile.rect);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(catIdle);
    SDL_DestroyTexture(catPrepare);
    SDL_DestroyTexture(catThrow);
    SDL_DestroyTexture(catHit); // Destroy cat hit texture
    SDL_DestroyTexture(catLaugh); // Destroy cat laugh texture
    SDL_DestroyTexture(dogIdle);
    SDL_DestroyTexture(dogPrepare);
    SDL_DestroyTexture(dogThrow);
    SDL_DestroyTexture(dogHit); // Destroy dog hit texture
    SDL_DestroyTexture(dogLaugh); // Destroy dog laugh texture
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(wallTexture); // Destroy wall texture
    quitSDL(window, renderer);

    return 0;
}

void logSDLError(std::ostream& os, const std::string &msg, bool fatal) {
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                           SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed() {
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN) {
                    return;
                }
            } else if (e.type == SDL_QUIT) {
                return;
            }
        }
        SDL_Delay(100);
    }
}

SDL_Texture* loadTexture(string path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr)
        cout << "Unable to load image " << path << " SDL_image Error: "
             << IMG_GetError() << endl;
    else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr)
            cout << "Unable to create texture from " << path << " SDL Error: "
                 << SDL_GetError() << endl;
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}
