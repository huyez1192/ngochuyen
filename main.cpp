#include "CommonFunc.h"
#include "BaseObject.h"
#include "Timer.h"
#include "Character.h"


#undef main
SDL_Window* g_window;
SDL_Renderer* g_screen;
const int fps = 50;


int SPEED = 10;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int rnd(int l,int r)
{
    return l+rng()%(r-l+1);
}

bool Init()
{
    bool success = 1;
    int ret = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );
    if(ret < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");

    g_window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawBlendMode(g_screen, SDL_BLENDMODE_BLEND);
        if(g_screen == NULL)
        {
            success = false;
        }
        else
        {
            SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags)&&imgFlags))
            {
                success = 0;
            }
            if(TTF_Init() == -1)
            {
                success = 0;
            }

        }
    }
    return success;
}

TTF_Font* font = NULL;
bool overlap(SDL_Rect a, SDL_Rect b)
{
    if(a.x > b.x + b.w - 1 || b.x > a.x + a.w -1) return false;
    if(a.y + a.h - 1 < b.y || b.y + b.h - 1 <a.y) return false;
    return true;
}

BaseObject background, menu, button;
Character dog;

void LoadMedia()
{
    background.LoadImg("map/background.png", g_screen);
    dog.LoadImg("character/dog-0.png", g_screen);
    menu.LoadImg("map/menu.png", g_screen);
    button.LoadImg("map/button.png", g_screen);

}
int can_gen = 1;

BaseObject threat;
int scene = 0;

int main(int argc, char *argv[])
{
    if(Init() == false) return -1;
    LoadMedia();
    bool quit = 0;
    SDL_Event e;
    Timer time;
    int scrollingOffset = 0;
    int x,y;
    int score = 0;
    while(!quit)
    {
        time.start();

        if(scene == 0)
        {

            while(SDL_PollEvent(&e) != 0)
            {
                if(e.type == SDL_QUIT)
                {
                    quit = true;
                }
                dog.HandleInputAction(e);
            }
            const Uint8* keys = SDL_GetKeyboardState(NULL);
            if(keys[SDL_SCANCODE_SPACE])
            {
                dog.up = 1;
            }
            SDL_SetRenderDrawColor( g_screen, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( g_screen );


            scrollingOffset -= SPEED;
            if( scrollingOffset < -background.rect_.w )
            {
                scrollingOffset = 0;
            }
            background.SetPos(scrollingOffset, 0);
            background.Render(g_screen);
            background.SetPos(scrollingOffset + background.rect_.w, 0);
            background.Render(g_screen);


            if(can_gen == 1)
            {
                int x = rnd(1,2);
                string path = "map/tree0" + to_string(x) + ".png";

                threat.LoadImg(path, g_screen);
                threat.SetPos(1300, 470);
                can_gen = 0;
            }

            threat.rect_.x -= SPEED;
            if(threat.rect_.x < -threat.rect_.w)
            {
                can_gen = 1;
            }
            threat.Render(g_screen);

            dog.Logic();
            dog.Show(g_screen);

            if(overlap(dog.rect_, threat.rect_))
            {
                scene = 1;
            }
        }
        else
        {
            SDL_SetRenderDrawColor( g_screen, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( g_screen );
            while(SDL_PollEvent(&e) != 0)
            {
                if(e.type == SDL_QUIT)
                {
                    quit = true;
                }
                if(e.type == SDL_MOUSEMOTION) x=e.motion.x, y=e.motion.y;
                if(x>=button.rect_.x && x<=button.rect_.x+button.rect_.w && y>=button.rect_.y && y<=button.rect_.y+button.rect_.h)
                {
                    if(e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        scene = 0;
                        dog.Reset();
                        threat.SetPos(1100, 475);
                    }
                }
            }
            background.SetPos(scrollingOffset, 0);
            background.Render(g_screen);
            background.SetPos(scrollingOffset + background.rect_.w, 0);
            background.Render(g_screen);

            menu.SetPos((SCREEN_WIDTH - menu.rect_.w)/2, (SCREEN_HEIGHT - menu.rect_.h)/2);
            menu.Render(g_screen);

            button.SetPos((SCREEN_WIDTH - button.rect_.w)/2, (SCREEN_HEIGHT - button.rect_.h)/2 + 70);
            button.Render(g_screen);
        }


        SDL_RenderPresent(g_screen);

        int pass_tick = time.getTicks();
        int time_per_frame = 1000/fps;//ms
        if(pass_tick < time_per_frame)
        {
            SDL_Delay(time_per_frame - pass_tick);
        }
    }
}
