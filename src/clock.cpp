/*

Clock for switch 
ELY M. 

*/



//includes
/*
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
*/

#include <SDL.h>
#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <switch.h>
#include <iostream>
#include <random>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


using namespace std;





const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const char* const months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const char* const weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};



//const float MAX_ANGLE = 3.142 / 5.0;


void Initialize(SDL_Renderer **ren, SDL_Window **win);
void Cleanup(SDL_Renderer **ren, SDL_Window **win);
SDL_Rect charRect[3][26];



//strobe functions 
void NavyScreen(int w, int h, SDL_Window *window, SDL_Renderer *renderer) {

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255);
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect f = {0, 0, w, h};
    SDL_RenderFillRect(renderer, &f);
	SDL_RenderPresent(renderer);
	
}

void CyanScreen(int w, int h, SDL_Window *window, SDL_Renderer *renderer) {

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect f = {0, 0, w, h};
    SDL_RenderFillRect(renderer, &f);
	SDL_RenderPresent(renderer);
	
}

void BlackScreen(int w, int h, SDL_Window *window, SDL_Renderer *renderer) {
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect f = {0, 0, w, h};
    SDL_RenderFillRect(renderer, &f);
	SDL_RenderPresent(renderer);
	
}


void GreenScreen(int w, int h, SDL_Window *window, SDL_Renderer *renderer) {
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect f = {0, 0, w, h};
    SDL_RenderFillRect(renderer, &f);
	SDL_RenderPresent(renderer);
	
}


void GlowScreen(int w, int h, SDL_Window *window, SDL_Renderer *renderer) {
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 213, 236, 153, 255);
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect f = {0, 0, w, h};
    SDL_RenderFillRect(renderer, &f);
	SDL_RenderPresent(renderer);
	
}

void WhiteScreen(int w, int h, SDL_Window *window, SDL_Renderer *renderer) {
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect f = {0, 0, w, h};
    SDL_RenderFillRect(renderer, &f);
	SDL_RenderPresent(renderer);
	
}



void Strobe(int w, int h, SDL_Window *window, SDL_Renderer *renderer) {
	

CyanScreen(w, h, window, renderer);
BlackScreen(w, h, window, renderer);
GreenScreen(w, h, window, renderer);
GlowScreen(w, h, window, renderer);
WhiteScreen(w, h, window, renderer);
NavyScreen(w, h, window, renderer);	

} 






//function to render a texture to a surface at x, y, w, y
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w=-1, int h=-1) {
	
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	
	if (w == -1 || h == -1) {
		SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h); //if w and h not set get texture w and h
	}

	SDL_RenderCopy(ren, tex, NULL, &dest); // render the texture

}






int main(int argc, char **argv) {
	
	romfsInit();
	SDL_Init(SDL_INIT_EVERYTHING); //init sdl
	IMG_Init(IMG_INIT_JPG); //init image lib
	
	bool quit = false; // quit variable. if true quits and exits application
	
	//controller
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);
	
	
	int alarmhour = 0; 
	int alarmminute = 0;
	int alarmampm = 0;


	
    //Setup window
    SDL_Window* window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
	
	//Setup renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	
	
	TTF_Init();
    TTF_Font *font = TTF_OpenFont("romfs:/fonts/FSEX300.ttf", 30);
	SDL_Color Aqua = {0, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

	
	
	//Load mice
	//SDL_Surface* miceImage = IMG_Load("romfs:/mymice-500x500.jpg");
	//SDL_Texture* miceTexture = SDL_CreateTextureFromSurface(renderer, miceImage);

		
	
	//alarm texts
	bool alarmset = false; 
	char alarmsettext[100] = "alarm";
	const char* alarmsetonoff = "Off";	
		
	while(appletMainLoop() && !quit) { //main game loop
		
		
		//clear screen every update//
		SDL_RenderClear(renderer);
		
		//Clock
		const char* ampm = "AM";
		char finaltime[100] = "time...";
		time_t unixTime = time(NULL);
		//struct tm* timeStruct = gmtime((const time_t *)&unixTime);
		struct tm* timeStruct = localtime((const time_t *)&unixTime);

		int hours = timeStruct->tm_hour;
		int minutes = timeStruct->tm_min;
		int seconds = timeStruct->tm_sec;
		int day = timeStruct->tm_mday;
		int month = timeStruct->tm_mon;
		int year = timeStruct->tm_year +1900;
		int wday = timeStruct->tm_wday;
		
		
		if (hours <= 12 && hours >= 0) {
		//AM	
		ampm = "AM";
		}
        else if (hours >= 13 && hours <= 24)
        {
		hours = (hours - 12);
		//PM
		ampm = "PM";
        }
		
		
		snprintf(finaltime, 100, "%i:%02d:%02d %s", hours, minutes, seconds, ampm);	
		SDL_Surface* sClock = TTF_RenderText_Solid(font, finaltime, Aqua); 
		SDL_Texture* Clock = SDL_CreateTextureFromSurface(renderer, sClock); //now you can convert it into a texture
		SDL_Rect Clock_rect; //create a rect
		Clock_rect.x = SCREEN_WIDTH / 2;  //controls the rect's x coordinate 
		Clock_rect.y = 10; // controls the rect's y coordinte
		Clock_rect.w = 630; // controls the width of the rect
		Clock_rect.h = 300; // controls the height of the rect
		SDL_FreeSurface(sClock);
		SDL_RenderCopy(renderer, Clock, NULL, &Clock_rect);
		SDL_DestroyTexture(Clock);
		//end of clock  
		
		
		
		
		//alarm//
		char alarmtime[100] = "alarm";
		snprintf(alarmtime, 100, "%i:%02d %i", alarmhour, alarmminute, alarmampm);	
		SDL_Surface* sAlarm = TTF_RenderText_Solid(font, alarmtime, Aqua); 
		SDL_Texture* Alarm = SDL_CreateTextureFromSurface(renderer, sAlarm); //now you can convert it into a texture
		SDL_Rect Alarm_rect; //create a rect
		Alarm_rect.x = SCREEN_WIDTH / 2;  //controls the rect's x coordinate 
		Alarm_rect.y = SCREEN_HEIGHT / 2; // controls the rect's y coordinte
		Alarm_rect.w = 630; // controls the width of the rect
		Alarm_rect.h = 300; // controls the height of the rect
		SDL_FreeSurface(sAlarm);
		SDL_RenderCopy(renderer, Alarm, NULL, &Alarm_rect);
		SDL_DestroyTexture(Alarm);
		
		
		//mice image
		//renderTexture(miceTexture, renderer, 300, 300, SCREEN_WIDTH, SCREEN_HEIGHT); //render the splash screen
		
		
		//scan inputs
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);		
		
		
		//alarm setting
		if (kDown & HidNpadButton_Up) {
		alarmhour++; 
		if (alarmhour >= 24) {
			alarmhour = 0; 
		}
		}
		
		if (kDown & HidNpadButton_Down) {
		alarmhour--; 
		if (alarmhour <= -1) {
			alarmhour = 0; 
		}
		}
		
	
	    if (kDown & HidNpadButton_Right) {
		alarmminute++; 
		if (alarmminute >= 60) {
			alarmminute = 0; 
		}
		}
		
		if (kDown & HidNpadButton_Left) {
		alarmminute--; 
		if (alarmminute <= -1) {
			alarmminute = 0; 
		}
		}
		
		
		
		//alarm set text
		//bool alarmset = false; 
		//char alarmsettext[100] = "alarm";
		//const char* alarmsetonoff = "Off";		
		if (kDown & HidNpadButton_A) {
		alarmsetonoff = "On";	
		alarmset = true; 
		}
		
		if (kDown & HidNpadButton_X) {
		alarmsetonoff = "Off";	
		alarmset = false; 
		}
		
		snprintf(alarmsettext, 100, "Alarm is %s", alarmsetonoff);	
		SDL_Surface* sAlarmSet = TTF_RenderText_Solid(font, alarmsettext, Aqua); 
		SDL_Texture* tAlarmSet = SDL_CreateTextureFromSurface(renderer, sAlarmSet); //now you can convert it into a texture
		SDL_Rect AlarmSet_rect; //create a rect
		AlarmSet_rect.x = SCREEN_WIDTH / 2;  //controls the rect's x coordinate 
		AlarmSet_rect.y = (SCREEN_HEIGHT / 2) + 300; // controls the rect's y coordinte
		AlarmSet_rect.w = 230; // controls the width of the rect
		AlarmSet_rect.h = 103; // controls the height of the rect
		SDL_FreeSurface(sAlarmSet);
		SDL_RenderCopy(renderer, tAlarmSet, NULL, &AlarmSet_rect);
		SDL_DestroyTexture(tAlarmSet);
		
		
		if (alarmset == true) {
	    if (hours == alarmhour && minutes == alarmminute) {
        Strobe(SCREEN_WIDTH, SCREEN_HEIGHT, window, renderer);
		}
		}
			
			
		//quit by press plus button
		if (kDown & HidNpadButton_Plus) {
		quit = true;		
		break; 
		}

		
		
		
		//load everything
		SDL_RenderPresent(renderer); 
			

	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}