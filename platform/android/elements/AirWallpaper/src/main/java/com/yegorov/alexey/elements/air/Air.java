package com.yegorov.alexey.elements.air;

import com.yegorov.alexey.elements.api.ElementsAir;

public class Air
{
    private int airID;

    public Air(boolean preview)
    {
        airID = ElementsAir.create(preview);
    }

    public boolean close()
    {
        return ElementsAir.destroy(airID);
    }

    public boolean startup(int width, int height, int textureSize)
    {
        return ElementsAir.startup(airID, width, height, textureSize);
    }

    public boolean colors(float r0, float g0, float b0, float r1, float g1, float b1)
    {
        return ElementsAir.colors(airID, r0, g0, b0, r1, g1, b1);
    }


    public boolean touch(float x, float y, int action)
    {
        return ElementsAir.touch(airID, x, y, action);
    }

    public boolean render()
    {
        return ElementsAir.render(airID);
    }
}
