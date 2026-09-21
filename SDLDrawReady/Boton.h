#pragma once
#include <string>
#include "Platform.h"
#include "Image.h"
#include "Text.h"

class Boton
{
private:
	float x;
	float y;
	Image imagen;
	Text* texto;
	float textoX;
	float textoY;

	void CentrarTexto(std::string mensaje);

public:
	Boton();
	~Boton();
	void Crear(std::string mensaje, float centroX, float posY);
	void CambiarTexto(std::string mensaje);
	void Dibujar(Platform* platform);
	bool MouseEncima(float mouseX, float mouseY);
};

void MedirTexto(std::string mensaje, int tamano, float& ancho, float& alto);

float XParaCentrar(std::string mensaje, int tamano);

void RectanguloOscuro(float x, float y, float ancho, float alto, int opacidad);
