#include "Pausa.h"
#include "Gameplay.h"
#include "Config.h"

Pausa::Pausa(Gameplay* juego)
{
	this->juego = juego;
	platform = nullptr;
	manager = nullptr;
	titulo = nullptr;
}

Pausa::~Pausa()
{
}

void Pausa::Init(Platform* platform, GameStateManager* manager)
{
	this->platform = platform;
	this->manager = manager;

	SDL_Color blanco = { 255, 255, 255, 255 };
	titulo = new Text(RUTA_FUENTE, 64, "PAUSA", blanco);
	tituloX = XParaCentrar("PAUSA", 64);

	botonContinuar.Crear("Continuar", ANCHO_PANTALLA / 2, 140);
	botonReiniciar.Crear("Reiniciar", ANCHO_PANTALLA / 2, 210);
	botonMenu.Crear("Menu", ANCHO_PANTALLA / 2, 280);
	botonSalir.Crear("Salir", ANCHO_PANTALLA / 2, 350);
}

void Pausa::Draw()
{
	platform->RenderClear();

	juego->DibujarEscena(); 
	RectanguloOscuro(0, 0, ANCHO_PANTALLA, ALTO_PANTALLA, 170);  

	titulo->Display(tituloX, 45); 
	botonContinuar.Dibujar(platform);
	botonReiniciar.Dibujar(platform);
	botonMenu.Dibujar(platform);
	botonSalir.Dibujar(platform);

	platform->RenderPresent();
}

bool Pausa::Input(ListaT<int>* keyDowns, ListaT<int>* keyUps, bool* leftclick, float* mouseX, float* mouseY)
{
	for (int i = 0; i < keyDowns->size; i++)
	{
		if (keyDowns->get_at(i)->value == SDLK_ESCAPE)
		{
			manager->PedirPop(1);
		}
	}

	if (*leftclick == true)
	{
		if (botonContinuar.MouseEncima(*mouseX, *mouseY))
		{
			manager->PedirPop(1);
		}
		else if (botonReiniciar.MouseEncima(*mouseX, *mouseY))
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

void Pausa::Update()
{

}

void Pausa::Close()
{
	delete titulo;
	titulo = nullptr;
}
