package com.yegorov.alexey.elements.demo;
import com.yegorov.alexey.elements.api.ElementsDemo;

public class Demo
{
    private int tankID;

    public Demo()
    {
        tankID = ElementsDemo.create();
    }

    public boolean close()
    {
        return ElementsDemo.destroy(tankID);
    }

    public boolean startup(int width, int height)
    {
        return ElementsDemo.startup(tankID, width, height);
    }

    public boolean touch(float x, float y, int action, int finger)
    {
        return ElementsDemo.touch(tankID, x, y, action, finger);
    }

    public boolean render()
    {
        return ElementsDemo.render(tankID);
    }

    public boolean rotation(float x, float y, float z)
    {
        return ElementsDemo.rotation(tankID, x, y, z);
    }
}
