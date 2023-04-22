#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>

#ifdef __MINGW32__
#undef main
#endif

#include "crt.h"
#include <stdint.h>

uint64_t rdtsc(void) {
  register uint64_t x;
  __asm__ volatile ("lfence");
  __asm__ volatile ("rdtsc" : "=A" (x));
  __asm__ volatile ("lfence");
  return x;
}


int main(int argc, char** argv)
{

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface *screen;

    SDL_Surface *image,*copy;
    image = IMG_Load("som2shot09.png");
	copy = SDL_CreateRGBSurface(0,image->w,image->h,32,0,0,0,0);
    SDL_BlitSurface(image,NULL,copy,NULL);


    int mode =3;

    int pitch = copy->w*4;
    int opitch = screen->w*4;

    SDL_Surface *screen2 = SDL_SetVideoMode(1400, 900, 32,SDL_OPENGL);
	if ( !screen2 ) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
	}


	glClearColor( 0, 0, 0, 0 );

	glEnable( GL_TEXTURE_2D ); // Need this to display a texture

    //glViewport( 0, 0, 640, 480 );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

   // glOrtho( 0, 640, 480, 0, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();


	screen = SDL_CreateRGBSurface(0,image->w*2,image->h*2,32,0,0,0,0);
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,128,128,128));
	opitch = screen->w*4;


	CRTx22fast(copy->pixels,screen->pixels,copy->w,copy->h,pitch,opitch);


	GLuint texture;
	glGenTextures( 1, &texture );

	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, texture );

	// Set the texture's stretching properties
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );




	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, 4, screen->w, screen->h, 0,
				  GL_BGRA, GL_UNSIGNED_BYTE, screen->pixels );







	uint64_t start = rdtsc();

	for(int i;i <100;i++)
		CRTx22fast(copy->pixels,screen->pixels,copy->w,copy->h,0>>1,0>>1);

	uint64_t end = rdtsc()-start;
	printf("%d\n",end);



	printf("%dx%d\n",screen->w,screen->h);



	int continuer = 1;
	SDL_Event event;


	glBindTexture( GL_TEXTURE_2D, texture );

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

        glClear( GL_COLOR_BUFFER_BIT );

        float va[8];
		float vt[8];

		float px = 100;
		float py = 200;
		float w = 256;
		float h = 128;


		vt[0] = 0;
		vt[1] = 1;

		vt[2] = 0;
		vt[3] = 0;

		vt[4] = 1;
		vt[5] = 0;

		vt[6] = 1;
		vt[7] = 1;


		va[0] = -1;
		va[1] = -1;

		va[2] = -1;
		va[3] = 1;

		va[4] = 1;
		va[5] = 1;

		va[6] = 1;
		va[7] = -1;


		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );

		glVertexPointer(2,GL_FLOAT,0,va);
		glTexCoordPointer(2,GL_FLOAT,0,vt);

		glDrawArrays( GL_QUADS, 0,4);

		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glDisableClientState( GL_VERTEX_ARRAY );


		SDL_Delay(50);
		SDL_GL_SwapBuffers();
    }

    SDL_FreeSurface(image);
    SDL_FreeSurface(screen);

    SDL_Quit();

    return 0;

}




int main1(int argc, char** argv)
{

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface *screen;


    SDL_Surface *image,*copy,*image2;
    image2 = IMG_Load("ff6shot172.png");
	copy = SDL_CreateRGBSurface(0,image2->w,image2->h,32,0,0,0,0);
    SDL_BlitSurface(image2,NULL,copy,NULL);


    int mode =3;

    int pitch = copy->w*4;
    int opitch = screen->w*4;

	screen = SDL_SetVideoMode(image2->w*2, image2->h*2, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,128,128,128));
	opitch = screen->w*4;


	uint64_t start = rdtsc();

			for(int i;i <60;i++)
				CRTx22fast(copy->pixels,screen->pixels,copy->w,copy->h,pitch,opitch);

			uint64_t end = rdtsc()-start;
			printf("%d\n",end);


/*
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


			uint64_t start = rdtsc();

			for(int i;i <60;i++)
				CRTx43(copy->pixels,screen->pixels,copy->w,copy->h,pitch,opitch);

			uint64_t end = rdtsc()-start;
			printf("%d\n",end);
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
	*/

	printf("%dx%d\n",screen->w,screen->h);

	int continuer = 1;
	SDL_Event event;


	//SDL_SaveBMP(screen,"test.bmp");
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

