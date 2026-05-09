

#include "Level01.h"

void Level01::LoadContent(ResourceManager& resourceManager)
{
    // Setup background
    SetBackground(resourceManager.Load<Texture>("Textures\\SpaceBackground01.png"));

    // Let the base Level class handle the player and the new random Wave Manager
    Level::LoadContent(resourceManager);
}
