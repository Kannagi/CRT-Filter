#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


#ifdef __MINGW32__
#undef main
#endif

#include "crt.h"
#include <stdint.h>

uint64_t rdtsc(void) {
  register uint64_t x;
  __asm__ volatile ("rdtsc" : "=A" (x));
  return x;
}


int main(int argc, char** argv)
{

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface *screen;


    SDL_Surface *image,*copy,*image2;
    image2 = IMG_Load("ff6shot080.png");
	copy = SDL_CreateRGBSurface(0,image2->w,image2->h,32,0,0,0,0);
    SDL_BlitSurface(image2,NULL,copy,NULL);


    int mode =3;

    int pitch = copy->w*4;
    int opitch = screen->w*4;

	if(mode == 0)
	{
		screen = SDL_SetVideoMode(image2->w*2, image2->h*2, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
		SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,128,128,128));
		opitch = screen->w*4;


		CRTx22(copy->pixels,screen->pixels,copy->w,copy->h,pitch,opitch);
	}else
	{
		if(mode == 1)
		{
			screen = SDL_SetVideoMode(image2->w*4, image2->h*3, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
			SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,128,128,128));
			opitch = screen->w*4;

			CRTx43(copy->pixels,screen->pixels,copy->w,copy->h,pitch,opitch);
		}else
		if(mode == 2)
		{
			screen = SDL_SetVideoMode(image2->w*4, image2->h*4, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
			SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,128,128,128));
			opitch = screen->w*4;

			CRTx44(copy->pixels,screen->pixels,copy->w,copy->h,pitch,opitch);
		}else
		{
			screen = SDL_SetVideoMode(image2->w*5, image2->h*4, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
			SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,128,128,128));
			opitch = screen->w*4;

			uint64_t start = rdtsc();

			for(int i;i <60;i++)
				CRTx54(copy->pixels,screen->pixels,copy->w,copy->h,pitch,opitch);

			uint64_t end = rdtsc()-start;

			printf("%d\n",end);
		}
	}

	printf("%dx%d\n",screen->w,screen->h);



	int continuer = 1;
	SDL_Event event;


	SDL_SaveBMP(screen,"test.bmp");
	while(continuer)
    {
    	int press = 0;
        while(SDL_PollEvent(&event));
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
            break;
        }

		SDL_Delay(50);
        SDL_Flip(screen);
    }

    SDL_FreeSurface(image);
    SDL_FreeSurface(screen);

    SDL_Quit();

    return 0;

}

