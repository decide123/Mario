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

enum ZONE
    {
    UNDEFINED_ZONE,
    SKY_ZONE,
    DANGER_ZONE,
    WALKING_ZONE
    };

enum STATE
    {
    HERO_DIE,
    HERO_ALIVE,
    GAME_RESTART,
    GAME_CONTINUE,
    GAME_BREAK
    };



double XCamera = 0;
double YCamera = 0;

struct Hero
    {
    double x, y;
    double vx;
    double vy;
    HDC image;
    };



void FonDraw(HDC fonPicture);
void HeroDraw(Hero hero);
STATE HeroPhysics(Hero* hero, double ay, double DT, double SpeedUp, ZONE Zone);
void DrawAll(const Hero GameObjects[], int AmountOfObjects);
void CameraControle(Hero *hero, HDC fon);
STATE HeroLogic(Hero *hero, ZONE Zone);
void NPC_Physics(Hero GameObjects[], int AmountOfObjects, double DT, double *SpeedUp);

const double MAX_SPEED = 12;

//запуск


//Герой
void HeroDraw(Hero hero)
    {
    txTransparentBlt(txDC(), hero.x - txGetExtentX(hero.image)/2 -XCamera, hero.y - txGetExtentY(hero.image)/2 - YCamera, 0, 0, hero.image, 0, 0, RGB(255, 255, 255));
    }

//Фон
void FonDraw(HDC fonPicture) //не только фон но и карта
    {
    txBitBlt(txDC(), -XCamera, YCamera + (txGetExtentY() - txGetExtentY(fonPicture)), 0, 0, fonPicture);
    }

                                                                                                     //14102, 1300
//Физика марио
STATE HeroPhysics(Hero* hero, double ay, double DT, double SpeedUp, ZONE Zone)
    {
    ASSERT(hero);

    if(Zone != SKY_ZONE)
        {
        hero -> vy = 0;
        }



    if(hero -> vx > MAX_SPEED)
        {
        hero -> vx = MAX_SPEED + SpeedUp;
        }

    hero -> x = hero -> x + hero -> vx * DT;
    hero -> y = hero -> y + hero -> vy * DT;

    hero -> vy = hero -> vy + ay*DT;

    return GAME_CONTINUE;
    }





void CameraControle(Hero* hero, HDC fon)
    {
    if(hero -> x > txGetExtentX() / 2)               XCamera = hero -> x -         txGetExtentX() / 2;
    if(XCamera >= txGetExtentX(fon)- txGetExtentX()) XCamera = txGetExtentX(fon) - txGetExtentX();
    }



STATE HeroLogic(Hero *hero, ZONE Zone)
    {
    /*if(Zone == DANGER_ZONE)
        {
        return HERO_DIE;                                                      //100, 700, 0, 0
        }

    return HERO_ALIVE;*/

    STATE StateHero = (Zone == DANGER_ZONE)? HERO_DIE : HERO_ALIVE;


    return StateHero;
    }


void DrawAll(const Hero GameObjects[], int AmountOfObjects)
    {
    for(int i = 0; i < AmountOfObjects; i++)
        {
        HeroDraw(GameObjects[i]);
        }
    }



void NPC_Physics(Hero GameObjects[], int AmountOfObjects, double DT, double SpeedUp)
    {
    for(int i = 0; i < AmountOfObjects; i++)
        {
        HeroPhysics(&GameObjects[i], 0, DT, SpeedUp, UNDEFINED_ZONE);
        }
    }






