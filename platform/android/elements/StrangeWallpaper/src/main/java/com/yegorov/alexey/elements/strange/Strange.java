package com.yegorov.alexey.elements.strange;

import com.yegorov.alexey.elements.api.ElementsStrange;

public class Strange
{
    private int strangeID;

    public Strange(boolean preview)
    {
        strangeID = ElementsStrange.create(preview);
    }

    public boolean close()
    {
        return ElementsStrange.destroy(strangeID);
    }

    public boolean startup(int width, int height, int textureSize)
    {
        return ElementsStrange.startup(strangeID, width, height, textureSize);
    }

    public boolean colorBackground(float r, float g, float b)
    {
        return ElementsStrange.background(strangeID, r, g, b);
    }

    public boolean colorGradient(float r1, float g1, float b1, float r2, float g2, float b2)
    {
        return ElementsStrange.gradient(strangeID, r1, g1, b1, r2, g2, b2);
    }

    public boolean render()
    {
        return ElementsStrange.render(strangeID);
    }
}
