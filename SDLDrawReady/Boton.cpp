#include "Boton.h"
#include "Config.h"
#include "SDL3_ttf/SDL_ttf.h"

const int TAMANO_TEXTO_BOTON = 26;
const float ANCHO_BOTON = 200;
const float ALTO_BOTON = 55;

Boton::Boton()
{
	x = 0;
	y = 0;
	texto = nullptr;
	textoX = 0;
	textoY = 0;
}

Boton::~Boton()
{
	delete texto;
	texto = nullptr;
}

void Boton::Crear(std::string mensaje, float centroX, float posY)
{
	imagen.LoadImage("Assets/boton.png");
	x = centroX - ANCHO_BOTON / 2; 
	y = posY;

	SDL_Color blanco = { 255, 255, 255, 255 };
	texto = new Text(RUTA_FUENTE, TAMANO_TEXTO_BOTON, mensaje, blanco);
	CentrarTexto(mensaje);
}

void Boton::CambiarTexto(std::string mensaje)
{
	texto->Update(mensaje);
	CentrarTexto(mensaje); 
}

void Boton::CentrarTexto(std::string mensaje)
{
	float ancho = 0;
	float alto = 0;
	MedirTexto(mensaje, TAMANO_TEXTO_BOTON, ancho, alto);

	textoX = x + (ANCHO_BOTON - ancho) / 2;
	textoY = y + (ALTO_BOTON - alto) / 2;
}

void Boton::Dibujar(Platform* platform)
{
	if (MouseEncima(platform->lastmouseX, platform->lastmouseY))
	{
		SDL_SetTextureColorMod(imagen.GetTexture(), 255, 255, 255);
	}
	else
	{
		SDL_SetTextureColorMod(imagen.GetTexture(), 210, 210, 210);
	}
	platform->RenderImage(&imagen, x, y, 0);
	texto->Display(textoX, textoY);
}

bool Boton::MouseEncima(float mouseX, float mouseY)
{
	if (mouseX < x) return false;
	if (mouseX > x + ANCHO_BOTON) return false;
	if (mouseY < y) return false;
	if (mouseY > y + ALTO_BOTON) return false;
	return true;
}

void MedirTexto(std::string mensaje, int tamano, float& ancho, float& alto)
{
	TTF_Init(); 

	TTF_Font* fuente = TTF_OpenFont(RUTA_FUENTE.c_str(), (float)tamano);
	if (fuente == nullptr)
	{
		ancho = 0;
		alto = 0;
		return;
	}

	int w = 0;
	int h = 0;
	TTF_GetStringSize(fuente, mensaje.c_str(), 0, &w, &h);
	TTF_CloseFont(fuente);

	ancho = (float)w;
	alto = (float)h;
}

float XParaCentrar(std::string mensaje, int tamano)
{
	float ancho = 0;
	float alto = 0;
	MedirTexto(mensaje, tamano, ancho, alto);
	return (ANCHO_PANTALLA - ancho) / 2;
}

void RectanguloOscuro(float x, float y, float ancho, float alto, int opacidad)
{
	SDL_SetRenderDrawBlendMode(Platform::renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Platform::renderer, 0, 0, 0, opacidad);
	SDL_FRect rect = { x, y, ancho, alto };
	SDL_RenderFillRect(Platform::renderer, &rect);
}
