package com.yegorov.alexey.elements.light;

import com.yegorov.alexey.elements.api.ElementsLight;

public class Light
{
    private int lightID;

    public Light(boolean preview)
    {
        lightID = ElementsLight.create(preview);
    }

    public boolean close()
    {
        return ElementsLight.destroy(lightID);
    }

    public boolean startup(int width, int height, int quality)
    {
        return ElementsLight.startup(lightID, width, height, quality);
    }

    public boolean background(String asset)
    {
        return ElementsLight.background(lightID, asset);
    }

    public boolean color(float r, float g, float b)
    {
        return ElementsLight.color(lightID, r, g, b);
    }

    public boolean surfaceTouch(float x, float y, int action)
    {
        return ElementsLight.touch(lightID, x, y, action);
    }

    public boolean render()
    {
        return ElementsLight.render(lightID);
    }
}
