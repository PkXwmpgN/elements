package com.yegorov.alexey.elements.liquid;

import com.yegorov.alexey.elements.api.ElementsLiquid;

public class Liquid
{
    private int liquidID;

    public Liquid(boolean preview)
    {
        liquidID = ElementsLiquid.create(preview);
    }

    public boolean close()
    {
        return ElementsLiquid.destroy(liquidID);
    }

    public boolean startup(int width, int height, int quality)
    {
        return ElementsLiquid.startup(liquidID, width, height, quality);
    }

    public boolean background(String asset)
    {
        return ElementsLiquid.background(liquidID, asset);
    }

    public boolean color(float r, float g, float b, float a)
    {
        return ElementsLiquid.color(liquidID, r, g, b, a);
    }

    public boolean touch(float x, float y, int action)
    {
        return ElementsLiquid.touch(liquidID, x, y, action);
    }

    public boolean render()
    {
        return ElementsLiquid.render(liquidID);
    }

    public boolean acceleration(float x, float y, float z)
    {
        return ElementsLiquid.acceleration(liquidID, x, y, z);
    }

    public boolean rotation(float x, float y)
    {
        return ElementsLiquid.rotation(liquidID, x, y);
    }
}
