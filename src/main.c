#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pd_api.h"

const int FPS = 50;
const int frameMS = 1000/FPS;

const char* location = "DEVICE";

#define samplePixel(data, x, y, rowbytes) (((data[(y)*rowbytes+(x)/8] & (1 << (uint8_t)(7 - ((x) % 8)))) != 0) ? kColorWhite:kColorBlack);

int W;
int H;
int CW;
int CH;
int testImageBitmapDataRowBytes;

PlaydateAPI* pd;
LCDBitmap* testImage;
uint8_t* testImageBitmapData;
LCDBitmap* testBack;
LCDBitmap* testSprite;
LCDSprite* playerSprite;


typedef void (*TestLambda)(void);
typedef struct TestLambdaNamed TestLambdaNamed;
struct TestLambdaNamed {
	char* name;
	TestLambda lambda;
};

void fNotImplemented(void){};
void fNil(void){};

void fDrawLineDiagonal(void)
{ 
	pd->graphics->drawLine(0,0,W,H, 1, kColorBlack);
}

void fDrawLineHorizontal(void) 
{
    pd->graphics->drawLine(0,CH,W,CH, 1, kColorBlack);
}

void fDrawLineVertical(void) 
{
    pd->graphics->drawLine(CW,0,CW,H, 1, kColorBlack);
}

void fDrawLineRandomDiagonal(void) 
{
    pd->graphics->drawLine(0,rand() % H,0,rand() % H, 1, kColorBlack);
}

void fDrawLineFillRect(void) 
{
    pd->graphics->fillRect(0,CH,W,1, kColorBlack);
}

void fDrawLineDrawRect(void) 
{
    pd->graphics->drawRect(0,CH,W,1, kColorBlack);
}


void fMathRandomSugar(void) 
{
    rand() % 999;
}

void fMathRandomProc(void) 
{
	rand() % 999;
}

void fMathSinSugar(void) 
{
	sin(1.5f);
}

void fMathSinRandom(void)
{
	sin(rand() / RAND_MAX * 6);
}

void fMathCos(void)
{
	cos(1.5f);
}
    
void fMathCosRandom(void)
{
	cos(rand() / RAND_MAX * 6);
}

void fMathFloor(void)
{
	floor(1.23f);  
}

void fImageSample(void) 
{
	int rowbytes;
	uint8_t *bitmapdata;
	pd->graphics->getBitmapData(testImage, NULL, NULL, &rowbytes, NULL, &bitmapdata);
	samplePixel(bitmapdata, 0, 0, rowbytes);
}

void fDrawText(void) 
{
	pd->graphics->drawText("TEST!", 5, kASCIIEncoding, CW-22,CH+50);
}

void fDrawRect(void) 
{
    pd->graphics->drawRect(CW-50,CH-50,100,100, kColorBlack);
}

void fFillRect(void)
{
    pd->graphics->fillRect(CW-50,CH-50,100,100, kColorBlack);
}

void fDrawEllipse(void)
{
    pd->graphics->drawEllipse(CW-50,CH-50,100,100, 1, 0.0f, 0.0f, kColorBlack);
}

void fFillEllipse(void)
{
    pd->graphics->fillEllipse(CW-50,CH-50,100,100, 0.0f, 0.0f, kColorBlack);
}

void fSpriteMoveToStatic(void)
{
    pd->sprite->setVisible(playerSprite, true);
    pd->sprite->moveTo(playerSprite, (float) CW, (float) CH);
}

void fSpriteMoveToRandom(void)
{
    pd->sprite->setVisible(playerSprite, true);
    pd->sprite->moveTo(playerSprite, (float) (rand() % W), (float) (rand() % H));
}

void fSpriteSetImage(void)
{
    pd->sprite->setImage(playerSprite, testSprite, kBitmapUnflipped);
}

void fSpriteSetCenterStatic(void)
{
	pd->sprite->setCenter(playerSprite, 0, 0); 
}

void fSpriteSetCenterToggle(void)
{
	float x,y;
	pd->sprite->getPosition(playerSprite, &x, &y);
	pd->sprite->setCenter(playerSprite, (int)x & 400 | 0, 0);
}

void fSpriteSetCenterRandom(void)
{
	pd->sprite->setCenter(playerSprite, rand() % 400, 0);
}

void fSpriteSetZIndex(void) 
{
	pd->sprite->setZIndex(playerSprite, 1);
}

void fDraw(void)
{
	pd->graphics->drawBitmap(testSprite, 0, 0, kBitmapUnflipped);
}

void fDrawPushContext(void)
{
    pd->graphics->pushContext(testBack);
    pd->graphics->drawBitmap(testSprite, 0, 0, kBitmapUnflipped);
    pd->graphics->popContext();
}

void fImageSampleBitmapData(void)
{
	samplePixel(testImageBitmapData, 0, 0, testImageBitmapDataRowBytes);
}


TestLambdaNamed funcs[] = {
    { "nil", fNil},
    { "drawDiagonal", fDrawLineDiagonal},
    { "drawHorizontal", fDrawLineHorizontal},
    { "drawVertical", fDrawLineVertical},
    { "drawRandomDiagonal", fDrawLineRandomDiagonal},
    { "drawLineFillRect", fDrawLineFillRect},
    { "drawLineDrawRect", fDrawLineDrawRect},
    { "mathRandomSugar", fMathRandomSugar},
    { "mathRandomProc", fMathRandomProc},
    { "mathSin", fMathSinSugar},
    { "mathSinRandom", fMathSinRandom},
    { "mathCos", fMathCos},
    { "mathCosRandom", fMathCosRandom},
    { "mathFloor", fMathFloor},
    { "imageSample - Fast", fImageSampleBitmapData},
    { "drawText", fDrawText},
    { "drawTextInRect not in C API", fNotImplemented},
    { "drawRect", fDrawRect},
    { "fillRect", fFillRect},
    { "drawEllipse", fDrawEllipse},
    { "fillEllipse", fFillEllipse},
    { "drawEllipse", fDrawEllipse},
    { "fillEllipse", fFillEllipse},
    { "spriteMoveToStatic", fSpriteMoveToStatic},
    { "spriteMoveToRandom", fSpriteMoveToRandom},
    { "spriteSetImage", fSpriteSetImage},
    { "spriteSetCenterStatic", fSpriteSetCenterStatic},
    { "spriteSetCenterToggle", fSpriteSetCenterToggle},
    { "spriteSetCenterRandom", fSpriteSetCenterRandom},
    { "spriteSetZIndex", fSpriteSetZIndex},
    { "draw", fDraw},
    { "drawLockedLocal - lockFocus not implemented in C", fNotImplemented},
    { "drawLockedLocal - local is a lua-concept", fNotImplemented},
    { "drawPushContext", fDrawPushContext},
    { "imageSample - Slow", fImageSample}
};



int start = 0;
int count = 0;
bool done = false;
int cmd = 0;// # different from lua because 0-based indexing in Nim / C
int max = sizeof(funcs) / sizeof(funcs[0]);
char * logLines[100]; 

void init(void)
{
    pd->display->setRefreshRate(FPS);
    W = pd->display->getWidth();
    H = pd->display->getHeight();
    CW = (int)W/2;
    CH = (int)H/2;
	const char *outerr;
    testImage = pd->graphics->loadBitmap("images/background", &outerr);
    pd->graphics->getBitmapData(testImage, NULL, NULL, &testImageBitmapDataRowBytes, NULL, &testImageBitmapData);
    testBack = pd->graphics->loadBitmap( "images/background", &outerr);
    testSprite = pd->graphics->loadBitmap("images/playerImage", &outerr);

    playerSprite = pd->sprite->newSprite();
    pd->sprite->setImage(playerSprite, testSprite, kBitmapUnflipped);
    pd->sprite->moveTo(playerSprite, (float)CW,(float)CH);
    pd->sprite->setVisible(playerSprite, false);
	pd->sprite->addSprite(playerSprite);

    LCDBitmap* backgroundImage = pd->graphics->loadBitmap("images/background", &outerr );

    pd->graphics->pushContext(backgroundImage);
	pd->graphics->drawText("*BENCH*", 7, kASCIIEncoding, CW-50,CH-50);
    pd->graphics->popContext();

    LCDSprite* bgSprite = pd->sprite->newSprite();
    pd->sprite->setImage(bgSprite, backgroundImage, kBitmapUnflipped);
    pd->sprite->setCenter(bgSprite, 0.0f, 0.0f); //DID NOT EXIST IN NIM
	pd->sprite->moveTo(bgSprite, 0.0f,0.0f);
    pd->sprite->setVisible(bgSprite, true);
    pd->sprite->setZIndex(bgSprite, -32768); //# lowest possible
    pd->sprite->setIgnoresDrawOffset(bgSprite, true);
	pd->sprite->addSprite(bgSprite);

    cmd = 0;
    count = 0;
    done = false;
}

int update(void* ud)
{
    pd->sprite->drawSprites();

    if (cmd == 0)
	{
		pd->system->logToConsole("frameMS: %d\n#, BENCH, CALL\n", frameMS);
	}

    if ((cmd >= 0) && (cmd < max))
	{
        TestLambdaNamed namedLambda = funcs[cmd];
        int endTime = pd->system->getCurrentTimeMilliseconds() + frameMS;
        while (pd->system->getCurrentTimeMilliseconds() < endTime)
		{
            namedLambda.lambda();
            count += 1;
		}

        done = true;
	}

    if (done && (cmd < max))
	{
        TestLambdaNamed namedLambda = funcs[cmd];
        int effectiveCount = (namedLambda.lambda == fNotImplemented ? 0 : count);
		pd->system->formatString(&logLines[cmd], "%d,%s,%d\n", cmd, namedLambda.name, effectiveCount);
		done = false;
        count = 0;
        cmd += 1;
	}

    if((cmd >= max) && !done)
	{
        pd->graphics->clear(kColorWhite);
		for (int i = 0; i < max; i++)
		{
			pd->sprite->setVisible(playerSprite, false);
			pd->system->logToConsole(logLines[i]);
			pd->system->realloc(logLines[i], 0);
		}

        done = true;

		return 0;
	}

    return 1;
}

#ifdef _WINDLL
__declspec(dllexport)
#endif

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{

	if ( event == kEventInit )
	{
		pd = playdate;
		init();
		playdate->system->setUpdateCallback(update, NULL);
	}
	
	return 0;
}
