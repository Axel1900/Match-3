#include "Gameplay.h"
#include "Boton.h"
#include "Config.h"
#include <string>

Gameplay::Gameplay()
{
	platform = nullptr;
	manager = nullptr;
	tablero = nullptr;
	textoAyuda = nullptr;
}

Gameplay::~Gameplay()
{
}

void Gameplay::Init(Platform* platform, GameStateManager* manager)
{
	this->platform = platform;
	this->manager = manager;

	fondo.LoadImage("Assets/background.png");

	std::string nombres[TIPOS_DE_GEMA] = { "Icecream", "Pizza", "Hotdog", "Water", "Frenchfires" };
	for (int i = 0; i < TIPOS_DE_GEMA; i++)
	{
		gemas[i].LoadImage("Assets/" + nombres[i] + ".png");
	}

	SDL_Color blanco = { 255, 255, 255, 255 };
	textoAyuda = new Text(RUTA_FUENTE, 18, "ESC = Menu", blanco);

	tablero = new Tablero();
}

void Gameplay::Draw()
{
	platform->RenderClear();

	platform->RenderImage(&fondo, 0, 0, 0);

	for (int fila = 0; fila < FILAS; fila++)
	{
		for (int col = 0; col < COLUMNAS; col++)
		{
			Gema g = tablero->GetGema(fila, col);

			float x = TABLERO_X + col * TAM + 2;
			float y = TABLERO_Y + fila * TAM + 2;

			platform->RenderImage(&gemas[g.tipo], x, y, 0);
		}
	}

	RectanguloOscuro(520, 30, 200, 300, 150);
	textoAyuda->Display(540, 440);

	platform->RenderPresent();
}

bool Gameplay::Input(ListaT<int>* keyDowns, ListaT<int>* keyUps, bool* leftclick, float* mouseX, float* mouseY)
{
	for (int i = 0; i < keyDowns->size; i++)
	{
		int tecla = keyDowns->get_at(i)->value;
		if (tecla == SDLK_ESCAPE)
		{
			manager->PedirPop(1); 
		}
	}
	return false;
}

void Gameplay::Update()
{
	
}

void Gameplay::Close()
{
	delete tablero;
	tablero = nullptr;
	delete textoAyuda;
	textoAyuda = nullptr;
}
