#include"library.h"
#include <math.h>
#define WIDTH 800
#define HEIGHT 600
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define AI -1
//#define DEBUG
SDL_Texture* loadTexture(std::string path);
bool Init();
void free();
void loadstuff();
void MOV(int _dir,SDL_Rect& _position,const int step);
void MOVBall(int& _stepx,int& _stepy);
bool Collision(SDL_Rect _rect1,SDL_Rect _rect2,int ups);
void Reset();
SDL_Texture* LoadText(std::string,SDL_Color);
SDL_Window* window=NULL;
SDL_Renderer* renderer=NULL;
SDL_Texture* bg=NULL;
TTF_Font*gFont=NULL;
Mix_Music*gMusic=NULL;
Mix_Music*gBorder=NULL;
Mix_Chunk*gPlayerSOUND=NULL;
Mix_Chunk*gBorderSOUND=NULL;
Mix_Chunk*gDeathSOUND=NULL;
float ratax=WIDTH/800.0;
float ratay=HEIGHT/600.0;
SDL_Rect player1= {30,HEIGHT/2 - 80,25*ratax,160*ratay};
SDL_Rect player2= {WIDTH-25*ratay-30,HEIGHT/2 - 80,25*ratax,160*ratay};
SDL_Rect ball= {WIDTH/2,HEIGHT/2,25*ratax,25*ratay};
SDL_Rect BORDERUP= {0,0,WIDTH,20};
SDL_Rect BORDERDOWN= {0,HEIGHT-20,WIDTH,200};
int score1=0;
int score2=0;
int ballspdx=5,ballspdy=5;
int spd=9;
int main(int argc,char* argv[])
{
    if(!Init())
    {
        fprintf(stderr,"Not quite what was planned!\n");
    }
    else
    {
        bool quit=false;
        SDL_Event handler;
        const Uint8* MagieNeagra;
        loadstuff();
        Mix_PlayMusic( gMusic, -1 );
        while(!quit)
        {
            SDL_PollEvent(&handler);
            MagieNeagra=SDL_GetKeyboardState(NULL);
            if(handler.type==SDL_QUIT)
            {
                quit=1;
            }
            if(MagieNeagra[SDL_SCANCODE_ESCAPE])
            {
                quit=1;
            }
            if(MagieNeagra[SDL_SCANCODE_UP])
            {
                MOV(UP,player1,spd);
            }
            if(MagieNeagra[SDL_SCANCODE_DOWN])
            {
                MOV(DOWN,player1,spd);
            }
            if(MagieNeagra[SDL_SCANCODE_R])
            {
                Reset();
            }
            /*if(MagieNeagra[SDL_SCANCODE_LEFT])
            {
                MOV(LEFT,player1,spd);
            }
            if(MagieNeagra[SDL_SCANCODE_RIGHT])
            {
                MOV(RIGHT,player1,spd);
            }*/
            /*if(MagieNeagra[SDL_SCANCODE_W])
            {
                MOV(UP,player2,spd);
            }
            if(MagieNeagra[SDL_SCANCODE_S])
            {
                MOV(DOWN,player2,spd);
            }*/
            /*if(MagieNeagra[SDL_SCANCODE_A])
            {
                MOV(LEFT,player2,spd);
            }
            if(MagieNeagra[SDL_SCANCODE_D])
            {
                MOV(RIGHT,player2,spd);
            }*/
            MOVBall(ballspdx,ballspdy);
            MOV(AI,player2,spd+8);
            SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,0xFF);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer,bg,NULL,NULL);
            SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
            SDL_RenderFillRect(renderer,&player1);
            SDL_RenderFillRect(renderer,&player2);
            SDL_RenderFillRect(renderer,&ball);
            std::stringstream nani;
            SDL_Color culuareScor= {0xFF,0xFF,0xFF};
            SDL_Rect TabelaScor= {240, 40, 340, 80};
            nani<<score1<<" "<<score2;
            SDL_Texture*TexturaScor=LoadText(nani.str(),culuareScor);
            SDL_RenderCopy(renderer,TexturaScor,NULL,&TabelaScor);
            SDL_DestroyTexture(TexturaScor);
            TexturaScor=NULL;

#ifdef DEBUG
            SDL_RenderFillRect(renderer,&BORDERUP);
            SDL_RenderFillRect(renderer,&BORDERDOWN);
            SDL_SetRenderDrawColor(renderer, 0x00, 0x80, 0x80, 0xFF);
#endif // DEBUG
#ifdef DEBUG
            SDL_Color cul= {0xFF,0xFF,0xFF,0xFF};
            SDL_Rect textpos= {0,0,200,100};
            std::stringstream ss;
            ss<<ball.x<<" "<<ball.y<<" "<<player2.x<<" "<<player2.y;
            SDL_RenderCopy(renderer,LoadText(ss.str(),cul),NULL,&textpos);
#endif // DEBUG
            SDL_RenderPresent(renderer);
        }
    }
    return 0;
}
bool Init()
{
    bool success=true;
    if(SDL_Init(SDL_INIT_EVERYTHING)<0)
    {
        fprintf(stderr,"SDL failed to initialize! SDL Error:%s",SDL_GetError());
        success=false;
    }
    else
    {
        window=SDL_CreateWindow("joj",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
        if(window==NULL)
        {
            fprintf(stderr,"Window failed to create! SDL Error:%s",SDL_GetError());
            success=false;
        }
        else
        {
            //SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
            renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(renderer==NULL)
            {
                fprintf(stderr,"Renderer failed to create! SDL Error:%s",SDL_GetError());
                success=false;
            }
            else
            {
                SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
                SDL_RenderClear(renderer);
                if(!IMG_Init(IMG_INIT_PNG))
                {
                    fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success=false;
                }
                if(TTF_Init()==-1)
                {
                    fprintf(stderr,"SDL_ttf could not initialize! SDL_ttf Error: %s\n",TTF_GetError());
                    success=false;
                }
                if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0)
                {
                    fprintf(stderr,"SDL_mixer could not initialize! SDL_mixer Error:%s\n",Mix_GetError());
                    success=false;
                }
            }
        }

    }
    return success;
}
SDL_Texture* loadTexture(std::string path)
{
    SDL_Texture* rar;
    SDL_Surface* loadedSurface=IMG_Load(path.c_str());
    rar=SDL_CreateTextureFromSurface(renderer,loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return rar;

}
void MOV(int _dir,SDL_Rect& _position,const int _step)
{
    switch(_dir)
    {
    case UP:
        _position.y-=_step;
        if(Collision(BORDERUP,_position,-1))
        {
            _position.y+=_step;
        }
        break;
    case DOWN:
        _position.y+=_step;
        if(Collision(BORDERDOWN,_position,1))
        {
            _position.y-=_step;
        }
        break;
    case AI:
            if(ball.y+ball.h-_step<=_position.y)
            {
                MOV(UP,_position,_step);
            }
            if(ball.y+_step>=_position.y+_position.h)
            {
                MOV(DOWN,_position,_step);
            }
        break;
    }
}
SDL_Texture* LoadText(std::string _text,SDL_Color colour)
{
    SDL_Surface*textsurface=TTF_RenderText_Solid(gFont,_text.c_str(),colour);
    if(textsurface==NULL)
    {
        fprintf(stderr,"Unable to render text surface! TTF error:%s",TTF_GetError());
    }
    else
    {
        SDL_Texture* joj=SDL_CreateTextureFromSurface(renderer,textsurface);
        SDL_FreeSurface(textsurface);
        return joj;
    }
    return NULL;
}
void free()
{
    Mix_FreeMusic(gMusic);
    gMusic=NULL;
    Mix_FreeChunk(gPlayerSOUND);
    gPlayerSOUND=NULL;
    Mix_FreeChunk(gDeathSOUND);
    gDeathSOUND=NULL;
    Mix_FreeChunk(gBorderSOUND);
    gBorderSOUND=NULL;
    Mix_CloseAudio();
    TTF_CloseFont(gFont);
    SDL_DestroyTexture(bg);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
}
void loadstuff()
{
    bg=loadTexture("Assets/pics/masa.png");
    gFont=TTF_OpenFont("Assets/fonts/consola.TTF",10);
    gMusic=Mix_LoadMUS("Assets/musics/Edison.mp3");
    gPlayerSOUND=Mix_LoadWAV("Assets/musics/effects/player.wav");
    gDeathSOUND=Mix_LoadWAV("Assets/musics/effects/death.wav");
    gBorderSOUND=Mix_LoadWAV("Assets/musics/effects/border.wav");
}
void MOVBall(int& _stepx,int& _stepy)
{
    ball.x+=_stepx;
    ball.y+=_stepy;
    if((Collision(player1,ball,_stepx))||Collision(player2,ball,_stepx))
    {
        ball.x-=_stepx;
        ball.y-=_stepy;
        _stepx=-_stepx;
        _stepx>0?_stepx+=2:_stepx-=2;
        _stepy>0?_stepy+=2:_stepy-=2;
        Mix_PlayChannel(-1,gPlayerSOUND,0);
        printf("Hit player!\n");
    }
    if((Collision(BORDERDOWN,ball,_stepy) && _stepy>0) ||(Collision(BORDERUP,ball,_stepy))&& _stepy<0)
    {
        ball.x-=_stepx;
        ball.y-=_stepy;
        _stepy=-_stepy;
        Mix_PlayChannel(-1,gBorderSOUND,0);
        printf("Hit Border!\n");
    }
    if(ball.x < player1.x + player1.w || ball.x + ball.w > player2.x)
    {
        ball.x=WIDTH/2;
        ball.y=HEIGHT/2;
        _stepx>0?_stepx=-5:_stepx=5;
        _stepy=5;
        player1.h-=5;
        player2.h-=5;
        Mix_PlayChannel(-1,gDeathSOUND,0);
        _stepx>0?score2++:score1++;
        if (score2 == 10 || score1 == 10)
        {
            score1 = 0;
            score2 = 0;
            player1.h = 160 * ratay;
            player2.h = 160 * ratay;
            player1.y = HEIGHT/2-80 ;
            player2.y = HEIGHT/2-80;
        }
        printf("Dead!\n");
    }
}
bool Collision(SDL_Rect _rect1,SDL_Rect _rect2,int ups)
{
    /*if(ups>0)
    {
        return (_rect1.x<=_rect2.x+_rect2.w) && (_rect1.y<=_rect2.y+_rect2.h);
    }*/
    return ((_rect2.x<=_rect1.x+_rect1.w) && (_rect2.y<=_rect1.y+_rect1.h))&&((_rect1.x<=_rect2.x+_rect2.w) && (_rect1.y<=_rect2.y+_rect2.h));
}
void Reset()
{
    ballspdx=5;
    ballspdx=5;
    ball.x=WIDTH/2;
    ball.y=HEIGHT/2;
    score1 = 0;
    score2 = 0;
    player1.h = 160 * ratay;
    player2.h = 160 * ratay;
    player1.y = HEIGHT/2-80 ;
    player2.y = HEIGHT/2-80;
}
