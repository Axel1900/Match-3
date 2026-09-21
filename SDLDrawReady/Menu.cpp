#include "Menu.h"
#include "Gameplay.h"
#include "Config.h"

Menu::Menu()
{
	platform = nullptr;
	manager = nullptr;
	titulo = nullptr;
	subtitulo = nullptr;
}

Menu::~Menu()
{
}

void Menu::Init(Platform* platform, GameStateManager* manager)
{
	this->platform = platform;
	this->manager = manager;

	fondo.LoadImage("Assets/background.png");

	SDL_Color dorado = { 255, 200, 40, 255 };
	SDL_Color blanco = { 255, 255, 255, 255 };
	titulo = new Text(RUTA_FUENTE, 90, "MATCH-3", dorado);
	subtitulo = new Text(RUTA_FUENTE, 22, "Haz lineas de 3 o mas iguales", blanco);
	tituloX = XParaCentrar("MATCH-3", 90);
	subtituloX = XParaCentrar("Haz lineas de 3 o mas iguales", 22);

	botonJugar.Crear("Jugar", ANCHO_PANTALLA / 2, 240);
	botonSalir.Crear("Salir", ANCHO_PANTALLA / 2, 320);
}

void Menu::Draw()
{
	platform->RenderClear();

	platform->RenderImage(&fondo, 0, 0, 0);
	RectanguloOscuro(0, 0, ANCHO_PANTALLA, ALTO_PANTALLA, 130);

	titulo->Display(tituloX, 70);
	subtitulo->Display(subtituloX, 180);
	botonJugar.Dibujar(platform);
	botonSalir.Dibujar(platform);

	platform->RenderPresent();
}

bool Menu::Input(ListaT<int>* keyDowns, ListaT<int>* keyUps, bool* leftclick, float* mouseX, float* mouseY)
{
	for (int i = 0; i < keyDowns->size; i++)
	{
		int tecla = keyDowns->get_at(i)->value;
		if (tecla == SDLK_SPACE)
		{
			manager->PedirPush(new Gameplay());
		}
		if (tecla == SDLK_ESCAPE)
		{
			manager->PedirSalir();
		}
	}

	if (*leftclick == true)
	{
		if (botonJugar.MouseEncima(*mouseX, *mouseY))
		{
			manager->PedirPush(new Gameplay());
		}
		if (botonSalir.MouseEncima(*mouseX, *mouseY))
		{
			manager->PedirSalir();
		}
	}
	return false;
}

void Menu::Update()
{

}

void Menu::Close()
{
	delete titulo;
	titulo = nullptr;
	delete subtitulo;
	subtitulo = nullptr;
}
