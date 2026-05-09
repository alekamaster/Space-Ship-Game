#include "Level02.h"

void Level02::LoadContent(ResourceManager& resourceManager)
{
	// Cargamos el fondo específico para el nivel 2 
	// (Asegúrate de poner el nombre correcto del fondo que uses para este nivel)
	SetBackground(resourceManager.Load<Texture>("Textures\\SpaceBackground02.png"));

	// Dejamos que la clase base (Level.cpp) se encargue de invocar al jugador y las oleadas
	Level::LoadContent(resourceManager);
}