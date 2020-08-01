#include "main.h"

static int TIME = 1;

void cap_framerate(Uint32 start_tick)
{
    if (1000 / FPS > SDL_GetTicks() - start_tick)
    {
        SDL_Delay(1000 / FPS - (SDL_GetTicks() - start_tick));
    }
}

void handleEvent(SDL_Event *event, struct LButton *gButton, char *file)
{
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        int inside = 1;

        if (x < gButton->mPosition.x || x > gButton->mPosition.x + MY_BUTTON_Width || y < gButton->mPosition.y || y > gButton->mPosition.y + MY_BUTTON_Hight)
        {
            inside = 0;
        }
        if (!inside)
        {
            gButton->mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        else
        {
            if (event->type == SDL_MOUSEBUTTONDOWN)
            {
                if (gButton->index == 0)
                {
                    gButton->mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                    char filename[1024];
                    FILE *f = popen("zenity --file-selection", "r");
                    fgets(filename, 1024, f);
                    printf("open input file: %s\n", filename);
                    strcpy(file, filename);
                    
                }
                else if (gButton->index == 2)
                {
                    gButton->mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                    char dirname[1024];
                    FILE *d = popen("zenity --file-selection --directory", "r");
                    fgets(dirname, 1024, d);
                    printf("add output directory: %s\n", dirname);
                    strcpy(file, dirname);
                    
                }
                else
                {
                    TIME++;
                    printf("edit time %d\n", TIME);
                }
            }
        }
    }
}

void button_Init(const Uint32 *color, SDL_Surface *screen, struct LButton *gButton)
{

    SDL_Surface *image = SDL_CreateRGBSurface(0, MY_BUTTON_Width, MY_BUTTON_Hight, 32, 0, 0, 0, 0);

    for (int i = 0; i < MY_TOTAL_Button; i++)
    {
        SDL_Rect rect = image->clip_rect;
        gButton[i].index = i;
        gButton[i].mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        rect.x = gButton[i].mPosition.x = SCREEN_INIT_WIDTH * i / MY_TOTAL_Button;
        rect.y = gButton[i].mPosition.y = SCREEN_INIT_HEIGHT / 6;
        SDL_FillRect(image, NULL, color[i]);
        SDL_BlitSurface(image, NULL, screen, &rect);
    }
}

int main (){
    char inputfile[1024], outputdir[1024];
    int screen_w = SCREEN_INIT_WIDTH, screen_h = SCREEN_INIT_HEIGHT;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window;
    window = SDL_CreateWindow("my attempt", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              screen_w, screen_h,
                              SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        printf("wrong in creating window!\n");
        return 1;
    }
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    Uint32 Button_Color[3] = {SDL_MapRGB(screen->format, 200, 0, 0), SDL_MapRGB(screen->format, 0, 200, 0), SDL_MapRGB(screen->format, 0, 0, 200)};
    Uint32 white = SDL_MapRGB(screen->format, 255, 255, 255);
    SDL_FillRect(screen, NULL, white);

    struct LButton gButton[3];
    button_Init(Button_Color, screen, gButton);
    SDL_UpdateWindowSurface(window);

    SDL_Event event;
    int running = 1;

    //while loop to process the events
    while (running)
    {
        Uint32 start_tick = SDL_GetTicks();
        //Set buttons

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
                break;
            }
            else if (event.type == SDL_WINDOWEVENT)
            {
                //SDL_GetWindowSize(window, &screen_w, &screen_h);
                //SDL_Surface *screen = SDL_GetWindowSurface(window);
                //SDL_FillRect(screen, NULL, white);
                //SDL_FreeSurface(screen);
                //SDL_UpdateWindowSurface(window);
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    if (i == 0)
                    {
                        handleEvent(&event, &(gButton[i]), inputfile);
                    }
                    else if (i == 2)
                    {
                        handleEvent(&event, &(gButton[i]), outputdir);
                    }
                    else
                    {
                        handleEvent(&event, &(gButton[i]), NULL);
                    }
                }
            }
        }

        //render and update the screen
        SDL_UpdateWindowSurface(window);
        cap_framerate(start_tick);
    }

    int argc = 6;

    char argv[1024];
    char *p = strchr(inputfile, '\n');
    *p = '\0';
 
    p = strchr(outputdir, '\n');
    *p = '\0';

    char temp[10];
    strcpy(argv, "ffmpeg ");
    strcat(argv, " -i ");
    strcat(argv, inputfile);

    strcat(argv, " -t ");
    SDL_itoa(TIME, temp, 10);
    strcat(argv, temp);
    strcat(argv, " ");
    strcat(argv, outputdir);
    strcat(argv, "/a.mp4");
    printf("%s\n", argv);
    printf("%s\n", argv);
    printf("%s\n", argv);
    printf("%s\n", argv);
    printf("%s\n", argv);
    popen(argv, "r");



    SDL_FreeSurface(screen);
    SDL_DestroyWindow(window);
    SDL_Quit();
}