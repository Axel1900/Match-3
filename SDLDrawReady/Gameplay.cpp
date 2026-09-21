#include "Gameplay.h"
#include "Pausa.h"
#include "GameOver.h"
#include "Config.h"
#include <string>
#include <cmath>  
#include <cstdlib>  

Gameplay::Gameplay()
{
	platform = nullptr;
	manager = nullptr;
	tablero = nullptr;
	historial = nullptr;
	particulas = nullptr;
	tituloPuntos = nullptr;
	textoPuntos = nullptr;
	tituloMovimientos = nullptr;
	textoMovimientos = nullptr;
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
	tituloMovimientos = new Text(RUTA_FUENTE, 22, "MOVIMIENTOS", dorado);
	textoMovimientos = new Text(RUTA_FUENTE, 30, "0", blanco);
	textoAyuda = new Text(RUTA_FUENTE, 18, "ESC = Pausa", blanco);
	botonDeshacer.Crear("Deshacer (0)", 620, 250);

	tablero = new Tablero();
	historial = new RingBuffer<Jugada>(JUGADAS_PARA_DESHACER);
	particulas = new ObjectPool<Particula>(MAX_PARTICULAS);

	Reiniciar();
}

void Gameplay::Reiniciar()
{
	tablero->Llenar();
	eventos.Clear();
	historial->Clear();
	particulas->LiberarTodos();

	puntaje = 0;
	movimientos = MOVIMIENTOS_INICIALES;
	combo = 1;
	yaSeAcabo = false;
	haySeleccion = false;
	vieneDeSwap = false;
	framesEspera = 0;

	puntosMostrados = -1; 
	movimientosMostrados = -1;
	deshacerMostrados = -1;
}

int Gameplay::GetPuntaje()
{
	return puntaje;
}

bool Gameplay::Input(ListaT<int>* keyDowns, ListaT<int>* keyUps, bool* leftclick, float* mouseX, float* mouseY)
{
	for (int i = 0; i < keyDowns->size; i++)
	{
		int tecla = keyDowns->get_at(i)->value;
		if (tecla == SDLK_ESCAPE)
		{
			manager->PedirPush(new Pausa(this));
			return false;
		}
	}

	if (*leftclick == true && TableroQuieto())
	{
		float mx = *mouseX;
		float my = *mouseY;

		if (botonDeshacer.MouseEncima(mx, my))
		{
			if (!historial->IsEmpty())
			{
				Deshacer();
			}
			return false;
		}

		bool dentroX = mx >= TABLERO_X && mx < TABLERO_X + COLUMNAS * TAM;
		bool dentroY = my >= TABLERO_Y && my < TABLERO_Y + FILAS * TAM;

		if (dentroX && dentroY && movimientos > 0)
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
	GuardarJugada();
	tablero->Intercambiar(selFila, selCol, fila, col);

	bool bomba1 = tablero->GetGema(fila, col).esBomba;
	bool bomba2 = tablero->GetGema(selFila, selCol).esBomba;

	if (bomba1 || bomba2)
	{
		if (bomba1) tablero->ExplotarBomba(fila, col);
		if (bomba2) tablero->ExplotarBomba(selFila, selCol);

		puntaje = puntaje + tablero->ContarMarcadas() * PUNTOS_POR_GEMA;
		movimientos--;
		eventos.Enqueue(EVENTO_DESTRUIR);
	}
	else
	{
		vieneDeSwap = true;
		swapFila1 = selFila;
		swapCol1 = selCol;
		swapFila2 = fila;
		swapCol2 = col;
		framesEspera = 8;  
		eventos.Enqueue(EVENTO_REVISAR_MATCHES);
	}
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
	ActualizarParticulas(); 

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

	if (movimientos <= 0 && !yaSeAcabo)
	{
		yaSeAcabo = true;
		manager->PedirPush(new GameOver(this));
		return;
	}

	if (!tablero->HayMovimientoPosible())
	{
		tablero->Llenar();
	}
}

void Gameplay::ProcesarSiguienteEvento()
{
	int evento = eventos.Dequeue();

	if (evento == EVENTO_REVISAR_MATCHES)
	{
		int cuantas = tablero->MarcarMatches(true);

		if (cuantas > 0)
		{
			puntaje = puntaje + cuantas * PUNTOS_POR_GEMA * combo;
			combo++;

			if (vieneDeSwap)
			{
				movimientos--;
				vieneDeSwap = false;
			}
			eventos.Enqueue(EVENTO_DESTRUIR);
		}
		else
		{
			if (vieneDeSwap)
			{
				tablero->Intercambiar(swapFila1, swapCol1, swapFila2, swapCol2);
				vieneDeSwap = false;
				framesEspera = 8;
				historial->PopBack();
			}
		}
	}
	else if (evento == EVENTO_DESTRUIR)
	{
		CrearParticulas();           
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
	if (movimientos != movimientosMostrados)
	{
		textoMovimientos->Update(std::to_string(movimientos));
		movimientosMostrados = movimientos;
	}
	if (historial->GetSize() != deshacerMostrados)
	{
		deshacerMostrados = historial->GetSize();
		botonDeshacer.CambiarTexto("Deshacer (" + std::to_string(deshacerMostrados) + ")");
	}
}

void Gameplay::DibujarEscena()
{
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

			if (g.esBomba)
			{
				float segundos = SDL_GetTicks() / 1000.0f;
				float brillo = (sinf(segundos * 6.0f) + 1.0f) / 2.0f; 
				int opacidad = (int)(100 + 155 * brillo);

				SDL_SetRenderDrawBlendMode(Platform::renderer, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawColor(Platform::renderer, 255, 200, 0, opacidad);
				for (int grosor = 0; grosor < 3; grosor++)
				{
					SDL_FRect marco = { x - 2 - grosor, y - 2 - grosor, TAM_GEMA + 4 + grosor * 2, TAM_GEMA + 4 + grosor * 2 };
					SDL_RenderRect(Platform::renderer, &marco);
				}
			}
		}
	}

	DibujarParticulas();

	if (haySeleccion)
	{
		platform->RenderImage(&cursor, TABLERO_X + selCol * TAM, TABLERO_Y + selFila * TAM, 0);
	}

	RectanguloOscuro(520, 30, 200, 300, 150);
	tituloPuntos->Display(540, 45);
	textoPuntos->Display(540, 72);
	tituloMovimientos->Display(540, 135);
	textoMovimientos->Display(540, 162);
	botonDeshacer.Dibujar(platform);
	textoAyuda->Display(540, 440);
}

void Gameplay::Draw()
{
	platform->RenderClear();
	DibujarEscena();
	platform->RenderPresent();
}

void Gameplay::GuardarJugada()
{
	Jugada foto;
	for (int fila = 0; fila < FILAS; fila++)
	{
		for (int col = 0; col < COLUMNAS; col++)
		{
			foto.gemas[fila][col] = tablero->GetGema(fila, col);
		}
	}
	foto.puntaje = puntaje;
	foto.movimientos = movimientos;

	historial->PushBack(foto); 
}

void Gameplay::Deshacer()
{
	Jugada foto = historial->PopBack(); 

	for (int fila = 0; fila < FILAS; fila++)
	{
		for (int col = 0; col < COLUMNAS; col++)
		{
			Gema g = foto.gemas[fila][col];
			g.caida = 0;
			g.marcada = false;
			tablero->SetGema(fila, col, g);
		}
	}
	puntaje = foto.puntaje;
	movimientos = foto.movimientos;
	haySeleccion = false;
}

void Gameplay::CrearParticulas()
{
	for (int fila = 0; fila < FILAS; fila++)
	{
		for (int col = 0; col < COLUMNAS; col++)
		{
			Gema g = tablero->GetGema(fila, col);
			if (!g.marcada) continue;

			float centroX = TABLERO_X + col * TAM + TAM / 2;
			float centroY = TABLERO_Y + fila * TAM + TAM / 2;

			for (int k = 0; k < PARTICULAS_POR_GEMA; k++)
			{
				int indice = particulas->Obtener();
				if (indice == -1) return; 

				Particula& p = particulas->Get(indice);
				p.x = centroX;
				p.y = centroY;
				p.vx = (rand() % 100 - 50) / 10.0f;
				p.vy = -(rand() % 60 + 20) / 10.0f;
				p.giro = (float)(rand() % 360);
				p.tipo = g.tipo;
				p.vida = 40;
			}
		}
	}
}

void Gameplay::ActualizarParticulas()
{
	for (int i = 0; i < particulas->GetCapacidad(); i++)
	{
		if (!particulas->EstaEnUso(i)) continue;

		Particula& p = particulas->Get(i);
		p.x = p.x + p.vx;
		p.y = p.y + p.vy;
		p.vy = p.vy + 0.35f;
		p.giro = p.giro + 8;
		p.vida--;

		if (p.vida <= 0)
		{
			particulas->Liberar(i);
		}
	}
}

void Gameplay::DibujarParticulas()
{
	for (int i = 0; i < particulas->GetCapacidad(); i++)
	{
		if (!particulas->EstaEnUso(i)) continue;

		Particula& p = particulas->Get(i);

		SDL_Texture* textura = gemas[p.tipo].GetTexture();
		SDL_FRect destino = { p.x - 8, p.y - 8, 16, 16 };
		int alpha = p.vida * 255 / 40;

		SDL_SetTextureAlphaMod(textura, alpha);
		SDL_RenderTextureRotated(Platform::renderer, textura, NULL, &destino, p.giro, NULL, SDL_FLIP_NONE);
		SDL_SetTextureAlphaMod(textura, 255);
	}
}

void Gameplay::Close()
{
	delete tablero;
	tablero = nullptr;
	delete historial;
	historial = nullptr;
	delete particulas;
	particulas = nullptr;

	delete tituloPuntos;
	delete textoPuntos;
	delete tituloMovimientos;
	delete textoMovimientos;
	delete textoAyuda;
	tituloPuntos = nullptr;
	textoPuntos = nullptr;
	tituloMovimientos = nullptr;
	textoMovimientos = nullptr;
	textoAyuda = nullptr;
}
