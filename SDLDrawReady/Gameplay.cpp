#include "Gameplay.h"
#include "Boton.h"
#include "Config.h"
#include <string>

Gameplay::Gameplay()
{
	platform = nullptr;
	manager = nullptr;
	tablero = nullptr;
	tituloPuntos = nullptr;
	textoPuntos = nullptr;
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
	cursor.LoadImage("Assets/cursor.png");

	SDL_Color dorado = { 255, 200, 40, 255 };
	SDL_Color blanco = { 255, 255, 255, 255 };
	tituloPuntos = new Text(RUTA_FUENTE, 22, "PUNTOS", dorado);
	textoPuntos = new Text(RUTA_FUENTE, 30, "0", blanco);
	textoAyuda = new Text(RUTA_FUENTE, 18, "ESC = Menu", blanco);

	tablero = new Tablero();

	puntaje = 0;
	combo = 1;
	haySeleccion = false;
	vieneDeSwap = false;
	framesEspera = 0;
	puntosMostrados = -1; 
}

bool Gameplay::Input(ListaT<int>* keyDowns, ListaT<int>* keyUps, bool* leftclick, float* mouseX, float* mouseY)
{
	for (int i = 0; i < keyDowns->size; i++)
	{
		int tecla = keyDowns->get_at(i)->value;
		if (tecla == SDLK_ESCAPE)
		{
			manager->PedirPop(1);  
			return false;
		}
	}

	if (*leftclick == true && TableroQuieto())
	{
		float mx = *mouseX;
		float my = *mouseY;

		bool dentroX = mx >= TABLERO_X && mx < TABLERO_X + COLUMNAS * TAM;
		bool dentroY = my >= TABLERO_Y && my < TABLERO_Y + FILAS * TAM;

		if (dentroX && dentroY)
		{
			int col = (int)((mx - TABLERO_X) / TAM);
			int fila = (int)((my - TABLERO_Y) / TAM);
			ClickEnCelda(fila, col);
		}
	}
	return false;
}

void Gameplay::ClickEnCelda(int fila, int col)
{
	if (!haySeleccion)
	{
		haySeleccion = true;
		selFila = fila;
		selCol = col;
		return;
	}

	if (fila == selFila && col == selCol)
	{
		haySeleccion = false;
		return;
	}

	if (!tablero->SonVecinas(selFila, selCol, fila, col))
	{
		selFila = fila;
		selCol = col;
		return;
	}

	haySeleccion = false;
	tablero->Intercambiar(selFila, selCol, fila, col);

	vieneDeSwap = true;
	swapFila1 = selFila;
	swapCol1 = selCol;
	swapFila2 = fila;
	swapCol2 = col;
	framesEspera = 8; 
	eventos.Enqueue(EVENTO_REVISAR_MATCHES);
}

bool Gameplay::TableroQuieto()
{
	if (tablero->HayAnimacion()) return false;
	if (framesEspera > 0) return false;
	if (!eventos.IsEmpty()) return false;
	return true;
}

void Gameplay::Update()
{
	ActualizarTextos();

	if (tablero->HayAnimacion())
	{
		tablero->AvanzarAnimacion(0.2f);
		return;
	}

	if (framesEspera > 0)
	{
		framesEspera--;
		return;
	}

	if (!eventos.IsEmpty())
	{
		ProcesarSiguienteEvento();
		return;
	}

	combo = 1;
}

void Gameplay::ProcesarSiguienteEvento()
{
	int evento = eventos.Dequeue();

	if (evento == EVENTO_REVISAR_MATCHES)
	{
		int cuantas = tablero->MarcarMatches();

		if (cuantas > 0)
		{
			puntaje = puntaje + cuantas * PUNTOS_POR_GEMA * combo;
			combo++; 
			vieneDeSwap = false;
			eventos.Enqueue(EVENTO_DESTRUIR);
		}
		else
		{
			if (vieneDeSwap)
			{
				tablero->Intercambiar(swapFila1, swapCol1, swapFila2, swapCol2);
				vieneDeSwap = false;
				framesEspera = 8;
			}
		}
	}
	else if (evento == EVENTO_DESTRUIR)
	{
		framesEspera = 20; 
		eventos.Enqueue(EVENTO_CAER);
	}
	else if (evento == EVENTO_CAER)
	{
		tablero->QuitarMarcadasYBajar();
		eventos.Enqueue(EVENTO_REVISAR_MATCHES);  
	}
}

void Gameplay::ActualizarTextos()
{
	if (puntaje != puntosMostrados)
	{
		textoPuntos->Update(std::to_string(puntaje));
		puntosMostrados = puntaje;
	}
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
			float y = TABLERO_Y + (fila - g.caida) * TAM + 2;

			if (y < TABLERO_Y - TAM) continue;  

			int alpha = 255;
			if (g.marcada && framesEspera > 0)
			{
				alpha = framesEspera * 12;
			}

			SDL_SetTextureAlphaMod(gemas[g.tipo].GetTexture(), alpha);
			platform->RenderImage(&gemas[g.tipo], x, y, 0);
			SDL_SetTextureAlphaMod(gemas[g.tipo].GetTexture(), 255);
		}
	}

	if (haySeleccion)
	{
		platform->RenderImage(&cursor, TABLERO_X + selCol * TAM, TABLERO_Y + selFila * TAM, 0);
	}

	RectanguloOscuro(520, 30, 200, 300, 150);
	tituloPuntos->Display(540, 45);
	textoPuntos->Display(540, 72);
	textoAyuda->Display(540, 440);

	platform->RenderPresent();
}

void Gameplay::Close()
{
	delete tablero;
	tablero = nullptr;
	delete tituloPuntos;
	delete textoPuntos;
	delete textoAyuda;
	tituloPuntos = nullptr;
	textoPuntos = nullptr;
	textoAyuda = nullptr;
}
