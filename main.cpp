#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Cat n Dog";

Mix_Music* menuMusic = nullptr;
Mix_Music* gameMusic = nullptr;
Mix_Music* endMusic = nullptr;


enum class Turn { CAT, DOG };

double mouse_x1, mouse_y1, mouse_x2, mouse_y2, theta, delta_x, delta_y, force;
bool shot = false;

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
SDL_Texture* loadTexture(string path, SDL_Renderer* renderer);
SDL_Texture* renderText(const std::string& message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer);

void loadMusic();
void closeMusic();

struct HealthBar {
    SDL_Rect barRect;
    int health;
    int maxHealth;

    HealthBar(int x, int y, int w, int h, int maxHealth)
        : barRect({x, y, w, h}), health(maxHealth), maxHealth(maxHealth) {}

    void render(SDL_Renderer* renderer) {
        // Render the background (red bar)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &barRect);

        // Render the foreground (green bar based on current health)
        SDL_Rect healthRect = barRect;
        healthRect.w = static_cast<int>(barRect.w * (static_cast<float>(health) / maxHealth));
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &healthRect);
    }

    void decreaseHealth(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0;
        }
    }
    void fullhp()
    {
        health = maxHealth;
    }
    int gethp()
    {
        return health;
    }
};

int showMainMenu(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color textColor = {255, 255, 255, 255}; // Black color

    SDL_Texture* playTexture = renderText("Play", font, textColor, renderer);
    SDL_Texture* exitTexture = renderText("Exit", font, textColor, renderer);
    SDL_Texture* menu = loadTexture("menu.png", renderer);

    SDL_Rect playRect;
    SDL_QueryTexture(playTexture, NULL, NULL, &playRect.w, &playRect.h);
    playRect.x = SCREEN_WIDTH / 2 - playRect.w / 2;
    playRect.y = SCREEN_HEIGHT / 2 - 50;

    SDL_Rect exitRect;
    SDL_QueryTexture(exitTexture, NULL, NULL, &exitRect.w, &exitRect.h);
    exitRect.x = SCREEN_WIDTH / 2 - exitRect.w / 2;
    exitRect.y = SCREEN_HEIGHT / 2 + 50;

    SDL_Event e;
    bool quitMenu = false;
    int selection = 0; // 0 for no selection, 1 for play, 2 for exit

    while (!quitMenu) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quitMenu = true;
                    selection = 2; // Exit
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        //SDL_Delay(1001);
                        int mouseX = e.button.x;
                        int mouseY = e.button.y;

                        if (mouseX >= playRect.x && mouseX <= playRect.x + playRect.w &&
                            mouseY >= playRect.y && mouseY <= playRect.y + playRect.h) {
                            selection = 1; // Play
                            quitMenu = true;

                        }

                        if (mouseX >= exitRect.x && mouseX <= exitRect.x + exitRect.w &&
                            mouseY >= exitRect.y && mouseY <= exitRect.y + exitRect.h) {
                            selection = 2; // Exit
                            quitMenu = true;
                        }
                    }
                    break;

                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        quitMenu = true;
                        selection = 2; // Exit
                    }
                    break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, menu, NULL, NULL);
        SDL_RenderCopy(renderer, playTexture, NULL, &playRect);
        SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
        SDL_RenderPresent(renderer);

    }
    SDL_DestroyTexture(playTexture);
    SDL_DestroyTexture(exitTexture);
    return selection;
}

int endgameMainMenu(SDL_Renderer* renderer, TTF_Font* font,int winner) {
    Mix_HaltMusic();
    Mix_PlayMusic(endMusic, -1);
    SDL_Color textColor = {255, 255, 255, 255}; // Black color

    SDL_Texture* playTexture = renderText("Retry", font, textColor, renderer);
    SDL_Texture* exitTexture = renderText("Exit", font, textColor, renderer);
    SDL_Texture* dogwin = loadTexture("dogwin.png", renderer);
    SDL_Texture* catwin = loadTexture("catwin.png", renderer);

    SDL_Rect playRect;
    SDL_QueryTexture(playTexture, NULL, NULL, &playRect.w, &playRect.h);
    playRect.x = SCREEN_WIDTH / 2 - playRect.w / 2;
    playRect.y = SCREEN_HEIGHT / 2 - 50;

    SDL_Rect exitRect;
    SDL_QueryTexture(exitTexture, NULL, NULL, &exitRect.w, &exitRect.h);
    exitRect.x = SCREEN_WIDTH / 2 - exitRect.w / 2;
    exitRect.y = SCREEN_HEIGHT / 2 + 50;

    SDL_Event e;
    bool quitMenu = false;
    int selection = 0; // 0 for no selection, 1 for play, 2 for exit

    while (!quitMenu) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quitMenu = true;
                    selection = 2; // Exit
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        //SDL_Delay(1001);
                        int mouseX = e.button.x;
                        int mouseY = e.button.y;

                        if (mouseX >= playRect.x && mouseX <= playRect.x + playRect.w &&
                            mouseY >= playRect.y && mouseY <= playRect.y + playRect.h) {
                            selection = 1; // Play
                            Mix_HaltMusic();
                            Mix_PlayMusic(gameMusic, -1);
                            quitMenu = true;

                        }

                        if (mouseX >= exitRect.x && mouseX <= exitRect.x + exitRect.w &&
                            mouseY >= exitRect.y && mouseY <= exitRect.y + exitRect.h) {
                            selection = 2; // Exit
                            quitMenu = true;
                        }
                    }
                    break;

                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        quitMenu = true;
                        selection = 2; // Exit
                    }
                    break;
            }
        }

        SDL_RenderClear(renderer);
        if (winner == 1)
            SDL_RenderCopy(renderer, catwin, NULL, NULL);
            else
            SDL_RenderCopy(renderer, dogwin, NULL, NULL);
        SDL_RenderCopy(renderer, playTexture, NULL, &playRect);
        SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(playTexture);
    SDL_DestroyTexture(exitTexture);

    return selection;
}

void angle() {
    delta_x = mouse_x2 - mouse_x1;
    delta_y = mouse_y2 - mouse_y1;

    force = std::sqrt(std::pow(delta_x, 2) + std::pow(delta_y, 2));

    if (delta_x != 0) {
        theta = std::atan2(delta_y, delta_x) * (180.0 / M_PI);
    } else {
        theta = 0;
    }

    if (5 < force ) { // Limit to avoid very small force
        shot = true;
    }
}

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return (a.x + a.w > b.x &&
            a.x < b.x + b.w &&
            a.y + a.h > b.y &&
            a.y < b.y + b.h);
}

void solveForHit(const SDL_Rect& attacker, const SDL_Rect& target, double& vxOut, double& vyOut) {
    double deltaX = target.x - attacker.x;
    double deltaY = target.y - attacker.y;

    double t = deltaX / (force / 10.0);

    vxOut = deltaX / t;
    vyOut = deltaY / t + 0.5 * 9.81 * t;
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

    TTF_Font* font = TTF_OpenFont("Font.ttf", 42); // Load the font at size 24
    TTF_Font* fontmenu = TTF_OpenFont("Font.ttf", 70);
    if (font == nullptr) {
        logSDLError(std::cout, "TTF_OpenFont", true);
    }

    loadMusic();
    Mix_PlayMusic(menuMusic, -1);

    int menuSelection = showMainMenu(renderer, fontmenu);

    if (menuSelection == 2) {
        quitSDL(window, renderer);
        return 0; // Exit game
    }
    Mix_HaltMusic();
    Mix_PlayMusic(gameMusic, -1);

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

    HealthBar catHealthBar(20, 20, 200, 20, 100); // Cat health bar
    HealthBar dogHealthBar(580, 20, 200, 20, 100); // Dog health bar


    SDL_Event e;
    bool quit = false;
    Projectile projectile;
    bool projectileInFlight = false;
    bool missed = false;
    Uint32 lastTime = SDL_GetTicks();
    Turn turn = Turn::DOG;

    SDL_Texture* currentCatTexture = catIdle;
    SDL_Texture* currentDogTexture = dogIdle;

    SDL_Texture* commentaryTexture = nullptr;
    Uint32 commentaryStartTime = 0;
    const Uint32 commentaryDuration = 1000; // 1 seconds
    SDL_Color textColor = {0, 0, 0, 255}; // Black color


    if (menuSelection == 1 )
    {
        while (!quit ) {
            while (SDL_PollEvent(&e)) {
                switch (e.type) {
                    case SDL_QUIT:
                        quit = true;
                        break;

                    case SDL_MOUSEBUTTONDOWN:
                        if (e.button.button == SDL_BUTTON_LEFT) {
                            cout << "Mouse button down" << endl;
                            mouse_x1 = e.button.x;
                            mouse_y1 = e.button.y;
                            if (turn == Turn::CAT) {
                                currentCatTexture = catPrepare;
                            } else {
                                currentDogTexture = dogPrepare;
                            }
                        }
                        break;

                    case SDL_MOUSEBUTTONUP:
                        if (e.button.button == SDL_BUTTON_LEFT) {
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
                                    projectile.texture = loadTexture("Can.png", renderer);
                                } else {
                                    projectile.x = dogRect.x + 90;
                                    projectile.y = dogRect.y + dogRect.h / 2 - 70;
                                    projectile.texture = loadTexture("Bone.png", renderer);
                                }
                                projectile.vx = std::cos(theta * M_PI / 180.0) * force / 10;
                                projectile.vy = std::sin(theta * M_PI / 180.0) * force / 10;

                                SDL_QueryTexture(projectile.texture, NULL, NULL, &projectile.rect.w, &projectile.rect.h);
                                projectileInFlight = true;
                            }
                        }
                        break;

                    case SDL_KEYDOWN:
                        if (e.key.keysym.sym == SDLK_d) {
                            double vx, vy;
                            cout << "SDL" << endl;

                            currentDogTexture = dogThrow;
                            if (turn == Turn::CAT) {
                                solveForHit(catRect, dogRect, vx, vy);
                                projectile.x = catRect.x + catRect.w - 90;
                                projectile.y = catRect.y + catRect.h / 2 - 70;
                                projectile.texture = loadTexture("Can.png", renderer);
                            } else {
                                solveForHit(dogRect, catRect, vx, vy);
                                projectile.x = dogRect.x + 90;
                                projectile.y = dogRect.y + dogRect.h / 2 - 70;
                                projectile.texture = loadTexture("Bone.png", renderer);
                            }

                            projectile.vx = vx;
                            projectile.vy = vy;
                            cout << "Hit calculated with vx: " << vx << ", vy: " << vy << endl;

                            SDL_QueryTexture(projectile.texture, NULL, NULL, &projectile.rect.w, &projectile.rect.h);
                            projectileInFlight = true;

                            if (turn == Turn::CAT) {
                                currentCatTexture = catThrow;
                            } else {
                                currentDogTexture = dogThrow;
                            }
                        }
                        break;
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
                    turn = Turn::DOG;
                    currentCatTexture = catIdle;
                    commentaryTexture = renderText("Dog Hit !", font, textColor, renderer);
                    commentaryStartTime = SDL_GetTicks();
                    dogHealthBar.decreaseHealth(10);
                    if (dogHealthBar.gethp()<=0)
                    {
                        int endgame = endgameMainMenu(renderer, fontmenu,1);
                        if (endgame == 2) {
                            quitSDL(window, renderer);
                            return 0; // Exit game
                        }
                        else {

                            dogHealthBar.fullhp();
                            catHealthBar.fullhp();

                        }
                    }


                }
                // Check collision with Cat
                else if (turn == Turn::DOG && checkCollision(projectile.rect, catRect)) {
                    currentCatTexture = catHit;
                    hit = true;
                    projectileInFlight = false;
                    SDL_DestroyTexture(projectile.texture);
                    turn = Turn::CAT;
                    currentDogTexture = dogIdle;
                    commentaryTexture = renderText("Cat Hit !", font, textColor, renderer);
                    commentaryStartTime = SDL_GetTicks();
                    catHealthBar.decreaseHealth(50);
                    if (catHealthBar.gethp()<=0)
                    {
                        int endgame = endgameMainMenu(renderer, fontmenu,0);
                        if (endgame == 2) {
                            quitSDL(window, renderer);
                            return 0; // Exit game
                        }
                        else {

                            dogHealthBar.fullhp();
                            catHealthBar.fullhp();

                        }
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
                        currentCatTexture = catIdle;
                        commentaryTexture = renderText("Cat Missed !", font, textColor, renderer);
                        commentaryStartTime = SDL_GetTicks();

                    } else {
                        turn = Turn::CAT;
                        currentCatTexture = catLaugh;
                        currentDogTexture = dogIdle;
                        commentaryTexture = renderText("Dog Missed !", font, textColor, renderer);
                        commentaryStartTime = SDL_GetTicks();
                    }
                }
                // **New check: Remove projectile if it hits left or right edge of the screen**
                else if (projectile.x < 0 || projectile.x > SCREEN_WIDTH) {
                    projectileInFlight = false;
                    SDL_DestroyTexture(projectile.texture);
                    projectile.texture = nullptr;

                    // Switch turn and show laughing texture
                    if (turn == Turn::CAT) {
                        turn = Turn::DOG;
                        currentDogTexture = dogLaugh;
                        currentCatTexture = catIdle;
                        commentaryTexture = renderText("Cat Missed !", font, textColor, renderer);
                        commentaryStartTime = SDL_GetTicks();
                        currentDogTexture = dogLaugh;
                    } else {
                        turn = Turn::CAT;
                        currentCatTexture = catLaugh;
                        currentDogTexture = dogIdle;
                        commentaryTexture = renderText("Dog Missed !", font, textColor, renderer);
                        commentaryStartTime = SDL_GetTicks();
                    }
                }
            }

            // Render everything
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background, NULL, NULL);
            SDL_RenderCopy(renderer, wallTexture, NULL, &wallRect); // Draw the wall
            SDL_RenderCopy(renderer, currentCatTexture, NULL, &catRect);
            SDL_RenderCopy(renderer, currentDogTexture, NULL, &dogRect);
            catHealthBar.render(renderer);
            dogHealthBar.render(renderer);


            if (projectileInFlight) {
                SDL_RenderCopy(renderer, projectile.texture, NULL, &projectile.rect);
            }
            if (commentaryTexture != nullptr && SDL_GetTicks() - commentaryStartTime < commentaryDuration) {
                int texW = 0;
                int texH = 0;
                SDL_QueryTexture(commentaryTexture, NULL, NULL, &texW, &texH);
                SDL_Rect dstRect = {SCREEN_WIDTH / 2 - texW / 2, SCREEN_HEIGHT / 2 - texH / 2 - 100, texW, texH};
                SDL_RenderCopy(renderer, commentaryTexture, NULL, &dstRect);
            } else if (SDL_GetTicks() - commentaryStartTime >= commentaryDuration) {
                SDL_DestroyTexture(commentaryTexture);
//                currentDogTexture = dogIdle;
//                currentCatTexture = catIdle;
                commentaryTexture = nullptr;
            }

            SDL_RenderPresent(renderer);
        }
    }
    TTF_CloseFont(font);
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
    TTF_Quit();
    Mix_Quit();
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

    if (TTF_Init() == -1) // Khởi tạo SDL_ttf
        logSDLError(std::cout, "TTF_Init", true);

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

SDL_Texture* renderText(const std::string& message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
    SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
    if (surf == nullptr) {
        logSDLError(std::cout, "TTF_RenderText");
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == nullptr) {
        logSDLError(std::cout, "CreateTexture");
    }
    SDL_FreeSurface(surf);
    return texture;
}

void loadMusic() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        logSDLError(std::cout, "Mix_OpenAudio", true);
    }

    menuMusic = Mix_LoadMUS("menu.mp3");
    if (menuMusic == nullptr) {
        logSDLError(std::cout, "Mix_LoadMUS (menu_music)", true);
    }

    gameMusic = Mix_LoadMUS("ing.mp3");
    if (gameMusic == nullptr) {
        logSDLError(std::cout, "Mix_LoadMUS (game_music)", true);
    }
    endMusic = Mix_LoadMUS("winning.mp3");
    if (endMusic == nullptr) {
        logSDLError(std::cout, "Mix_LoadMUS (end_music)", true);
    }
}

void closeMusic() {
    Mix_FreeMusic(menuMusic);
    Mix_FreeMusic(gameMusic);
    Mix_CloseAudio();
}
