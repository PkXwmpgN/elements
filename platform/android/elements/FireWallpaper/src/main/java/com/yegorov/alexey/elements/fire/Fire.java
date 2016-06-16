package com.yegorov.alexey.elements.fire;

import com.yegorov.alexey.elements.api.ElementsFire;

public class Fire
{
    private int fireID;

    public Fire(boolean preview)
    {
        fireID = ElementsFire.create(preview);
    }

    public boolean close()
    {
        return ElementsFire.destroy(fireID);
    }

    public boolean startup(int width, int height, int textureSize)
    {
        return ElementsFire.startup(fireID, width, height, textureSize);
    }

    public boolean background(String asset)
    {
        return ElementsFire.background(fireID, asset);
    }

    public boolean colorHot(float r, float g, float b)
    {
        return ElementsFire.colorHot(fireID, r, g, b);
    }

    public boolean colorCold(float r, float g, float b)
    {
        return ElementsFire.colorCold(fireID, r, g, b);
    }

    public boolean touch(float x, float y, int action)
    {
        return ElementsFire.touch(fireID, x, y, action);
    }

    public boolean render()
    {
        return ElementsFire.render(fireID);
    }
}
