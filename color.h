//
// Created by Roberto Sasso on 18/11/2020.
//

#ifndef LAB3_COLOR_H
#define LAB3_COLOR_H

class Color {
public:
    float r;
    float g;
    float b;
    float a;

    Color()
    {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
        a = 1.0f;
    }

    Color(float red, float green, float blue, float alpha = 1.0)
    {
        r = legalColor(red);
        g = legalColor(green);
        b = legalColor(blue);
        a = legalColor(alpha);
    }

    void add(Color c) {
        r = legalColor(r + c.r);
        g = legalColor(g + c.g);
        b = legalColor(b + c.b);
    }

    void scale(float x) {
        r *= x;
        g *= x;
        b *= x;
    }

    float legalColor(float toInspect)
    {
        if (toInspect < 0.0) {
            return 0.0;
        } else if (toInspect > 1.0) {
            return 1.0;
        } else {
            return toInspect;
        }
    }
};

#endif //LAB3_COLOR_H
