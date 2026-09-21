#include "GameOver.h"
#include "Gameplay.h"
#include "Config.h"
#include <string>

GameOver::GameOver(Gameplay* juego)
{
	this->juego = juego;
	platform = nullptr;
	manager = nullptr;
	titulo = nullptr;
	textoPuntaje = nullptr;
}

GameOver::~GameOver()
{
}

void GameOver::Init(Platform* platform, GameStateManager* manager)
{
	this->platform = platform;
	this->manager = manager;

	SDL_Color rojo = { 255, 80, 80, 255 };
	SDL_Color blanco = { 255, 255, 255, 255 };
	std::string mensajePuntaje = "Puntaje final: " + std::to_string(juego->GetPuntaje());
	titulo = new Text(RUTA_FUENTE, 72, "GAME OVER", rojo);
	textoPuntaje = new Text(RUTA_FUENTE, 34, mensajePuntaje, blanco);
	tituloX = XParaCentrar("GAME OVER", 72);
	textoPuntajeX = XParaCentrar(mensajePuntaje, 34);

	botonReiniciar.Crear("Reiniciar", ANCHO_PANTALLA / 2, 220);
	botonMenu.Crear("Menu", ANCHO_PANTALLA / 2, 290);
	botonSalir.Crear("Salir", ANCHO_PANTALLA / 2, 360);
}

void GameOver::Draw()
{
	platform->RenderClear();

	juego->DibujarEscena();  
	RectanguloOscuro(0, 0, ANCHO_PANTALLA, ALTO_PANTALLA, 190);

	titulo->Display(tituloX, 40);
	textoPuntaje->Display(textoPuntajeX, 145);
	botonReiniciar.Dibujar(platform);
	botonMenu.Dibujar(platform);
	botonSalir.Dibujar(platform);

	platform->RenderPresent();
}

bool GameOver::Input(ListaT<int>* keyDowns, ListaT<int>* keyUps, bool* leftclick, float* mouseX, float* mouseY)
{
	if (*leftclick == true)
	{
		if (botonReiniciar.MouseEncima(*mouseX, *mouseY))
		{
			juego->Reiniciar();
			manager->PedirPop(1);   
		}
		else if (botonMenu.MouseEncima(*mouseX, *mouseY))
		{
			manager->PedirPop(2); 
		}
		else if (botonSalir.MouseEncima(*mouseX, *mouseY))
		{
			manager->PedirSalir();
		}
	}
	return false;
}

void GameOver::Update()
{
}

void GameOver::Close()
{
	delete titulo;
	titulo = nullptr;
	delete textoPuntaje;
	textoPuntaje = nullptr;
}
