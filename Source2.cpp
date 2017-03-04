/* up to date: v0.1
1_Картинка полностью помещается в экран.
2_загрузить картинку с марио.
3_Использовать txGetPixel.
4_Сделать физику для марио(скорости, положение на экране, ускорение (g))
след занятие 20.12 20.00;
   up to date: v0.2
1_Если марио застрял в текстурах как достать его обратно;
   up to date: v0.3
1_Луиджи, звуки, блок краев экранаслд занятие 01 01 1800
   up to date: v0.4
1_ускорение при нажатии shift
2_как заставить марио прыгнуть  слд зан 1800 08 01
   up to date v0.5
1 цвета отвечают зпа ходьбу пржки и смерти
*/


#define ASSERT(something)  if(something == 0)																			  \
								{																						  \
								printf("error on line %d in file %s while checking %s\n", __LINE__, __FILE__, #something);\
								assert(0);																				  \
								}

#include "TXlib.h"
#include "MarioEngine.h"



const COLORREF SKY_COLOR    = RGB(112, 132, 255);
const COLORREF GROUND_COLOR = RGB(0, 255, 0);
const COLORREF DANGER_COLOR = RGB(255, 0, 0);
const int MAX_NPC = 5;



struct Game
    {
    STATE status;
    Hero mario;
    Hero luigi;
    Hero NPC[MAX_NPC];
    int DieCounter;
    HDC fon;
    HDC map;
    };


void GameRun(Game *game);
void Controls(Hero *hero, int up, int left, int right, double* SpeedUp, ZONE Zone);
void BirdRespawn(Hero GameObjects[], int AmountOfObjects);
ZONE HeroZONE(Hero hero);
STATE HeroLogic(Hero *hero, ZONE Zone);


bool GameRestart(Game *game);




//запуск
int main()
    {
    _txWindowStyle = WS_POPUP;
    txCreateWindow(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

    txBegin();

    HDC fonPicture = txLoadImage("FonMario.bmp");
	ASSERT(fonPicture);
	HDC heroMario = txLoadImage("HeroMario.bmp");
	ASSERT(heroMario);
    HDC heroLuigi = txLoadImage("HeroLuigi.bmp");
	ASSERT(heroLuigi);
    HDC mapPicture = txLoadImage("MapMario.bmp");
    ASSERT(mapPicture);
    HDC BirdNPC = txLoadImage("NPCBird.bmp");
    ASSERT(BirdNPC);

    Game game = {GAME_CONTINUE,
                {100, 700, 0, 0, heroMario},
                {140, 700, 0, 0, heroLuigi},
                {{700, 500, 200, 0, BirdNPC}, {600, 600, 100, 0, BirdNPC}, {200, 500, 100, 0, BirdNPC}, {300, 300, 100, 0, BirdNPC}, {700, 100, 100, 0, BirdNPC}},
                0,
                fonPicture,
                mapPicture};

                                                                                             /*

                                                                                             //txGetExtentX(fonPicture),
                                                                                             //txGetExtentY(fonPicture)/2,
                                                                                             700,
                                                                                             500,
                                                                                             100,
                                                                                             0,
                                                                                             BirdNPC
                                                                                             */
    txSetFillColor(TX_NULL);

    GameRun(&game);

    txEnd();

    txDeleteDC(fonPicture);
    txDeleteDC(heroMario);
    txDeleteDC(heroLuigi);
    txDeleteDC(mapPicture);
    txDeleteDC(BirdNPC);
    return 0;
    }

                                                                                                //14102, 1300

//Процесс игры
void GameRun(Game *game)
    {
    double DT = 1;
    double ay = 1;
    double SpeedUp = 10;


    while (!GetAsyncKeyState(VK_ESCAPE))
        {
        FonDraw(game -> map);



        ZONE ZoneMario = HeroZONE(game -> mario);
        ZONE ZoneLuigi = HeroZONE(game -> luigi);

        HeroLogic(& game -> mario, ZoneMario);
        HeroLogic(& game -> luigi, ZoneLuigi);

        STATE MarioState = HeroLogic(&game -> mario, ZoneMario);
        STATE LuigiState = HeroLogic(&game -> luigi, ZoneLuigi);

        if(MarioState == HERO_DIE || LuigiState == HERO_DIE)
            {
            GameRestart(game);
            }

        printf("MARIOZONE = %i \n", ZoneMario);

        if(!GetAsyncKeyState (VK_F12))  FonDraw(game -> fon);

        HeroDraw(game -> mario);
        HeroDraw(game -> luigi);
        DrawAll(game -> NPC, MAX_NPC);

        //printf("BirdX = %lg, BirdY = %lg \n", game -> NPC.x, game -> NPC.y);

        Controls(& game -> mario, VK_SPACE,  VK_LEFT, VK_RIGHT, &SpeedUp, ZoneMario);
        Controls(& game -> luigi, 'W', 'A', 'D', &SpeedUp, ZoneLuigi);
        /////////////////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////////////

        BirdRespawn(game -> NPC, MAX_NPC);

        HeroPhysics(& game -> mario, ay, DT, SpeedUp, ZoneMario);
        HeroPhysics(& game -> luigi, ay, DT, SpeedUp, ZoneLuigi);



        CameraControle(& game -> mario, game -> fon);

        txSleep(20);
        }
    }


// управление
void Controls(Hero* hero, int up, int left, int right, double* SpeedUp, ZONE Zone)
    {
    if (GetAsyncKeyState(up) && Zone == WALKING_ZONE)
        {
        (hero->y) -= 20;
        (hero->vy) -= 20;
        }

    if (GetAsyncKeyState(right))    hero -> vx += *SpeedUp;
    if (GetAsyncKeyState(left))     hero -> vx -= *SpeedUp;

    if (GetAsyncKeyState(VK_SHIFT))
        {
        *SpeedUp = 10;
        }
    else
        {
        *SpeedUp = 1;
        }

    cout << "\n" << hero->vx << "\t";
    }




ZONE HeroZONE(Hero hero)
    {
    //GetPixel
    double xPixel = hero.x - XCamera + 32;
    double yPixel = hero.y - YCamera + 32;

    txSetColor(TX_PINK);

    const int r = 5;

    COLORREF PixelColorRL       = txGetPixel(hero.x - XCamera + 32, hero.y - YCamera + 32); txCircle(hero.x - XCamera + 32, hero.y - YCamera + 32,  r);
    COLORREF PixelColorRH       = txGetPixel(hero.x - XCamera + 32, hero.y - YCamera);      txCircle(hero.x - XCamera + 32, hero.y - YCamera,  r);

    COLORREF PixelColorLL       = txGetPixel(hero.x - XCamera - 32, hero.y - YCamera + 32); txCircle(hero.x - XCamera - 32, hero.y - YCamera + 32, r);
    COLORREF PixelColorLH       = txGetPixel(hero.x - XCamera - 32, hero.y - YCamera);      txCircle(hero.x - XCamera - 32, hero.y - YCamera, r);

    COLORREF PixelColorRHEAD    = txGetPixel(hero.x - XCamera + 32, hero.y - YCamera - 32); txCircle(hero.x - XCamera + 32, hero.y - YCamera - 32, r);
    COLORREF PixelColorLHEAD    = txGetPixel(hero.x - XCamera - 32, hero.y - YCamera - 32); txCircle(hero.x - XCamera - 32, hero.y - YCamera - 32, r);

    COLORREF PixelColorMHEAD    = txGetPixel(hero.x - XCamera,      hero.y - YCamera - 32); txCircle(hero.x - XCamera,      hero.y - YCamera - 32, r);
    COLORREF PixelColorML       = txGetPixel(hero.x - XCamera,      hero.y - YCamera + 32); txCircle(hero.x - XCamera,      hero.y - YCamera + 32, r);

    if(PixelColorRL == SKY_COLOR && PixelColorRH == SKY_COLOR && PixelColorLL == SKY_COLOR && PixelColorLH == SKY_COLOR &&
       PixelColorRHEAD == SKY_COLOR && PixelColorLHEAD == SKY_COLOR && PixelColorMHEAD == SKY_COLOR && PixelColorML == SKY_COLOR)
        {
        return SKY_ZONE;
        }

    if(PixelColorRL    == GROUND_COLOR && PixelColorRH == SKY_COLOR && PixelColorLL    == GROUND_COLOR && PixelColorLH == SKY_COLOR &&
       PixelColorRHEAD == SKY_COLOR && PixelColorLHEAD == SKY_COLOR && PixelColorMHEAD == SKY_COLOR && PixelColorML    == GROUND_COLOR)
        {
        return WALKING_ZONE;
        }

    if(PixelColorRL    == DANGER_COLOR || PixelColorRH    == DANGER_COLOR || PixelColorLL    == DANGER_COLOR || PixelColorLH    == DANGER_COLOR ||
       PixelColorRHEAD == DANGER_COLOR || PixelColorLHEAD == DANGER_COLOR || PixelColorMHEAD == DANGER_COLOR || PixelColorML    == DANGER_COLOR)
        {
        return DANGER_ZONE;
        }

    return UNDEFINED_ZONE;
    }



bool GameRestart(Game *game)
    {
    game -> mario.x = txGetExtentX()/2;
    game -> mario.y = 700;
    game -> mario.vx = 0;
    game -> mario.vy = 0;

    game -> luigi.x = txGetExtentX()/1.5;
    game -> luigi.y = 700;
    game -> luigi.vx = 0;
    game -> luigi.vy = 0;
    return true;
    }


void BirdRespawn(Hero GameObjects[], int AmountOfObjects)
    {
    for(int i = 0; i < AmountOfObjects; i++)
        {
        if(GameObjects[i].x >= 14102/*конец картинки.*/) GameObjects[i].x = -200;
        }
    }








