package com.yegorov.alexey.elements.character;

import com.yegorov.alexey.elements.api.ElementsCharacter;

public class Character
{
    private int characterID;

    public Character(boolean preview)
    {
        characterID = ElementsCharacter.create(preview);
    }

    public boolean close()
    {
        return ElementsCharacter.destroy(characterID);
    }

    public boolean startup(int width, int height)
    {
        return ElementsCharacter.startup(characterID, width, height);
    }

    public boolean render()
    {
        return ElementsCharacter.render(characterID);
    }

    public boolean rotation(float x, float y, float z)
    {
        return ElementsCharacter.rotation(characterID, x, y, z);
    }

    public boolean model(String asset)
    {
        return ElementsCharacter.model(characterID, asset);
    }
}
