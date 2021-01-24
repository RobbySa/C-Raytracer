//
// Created by Roberto Sasso on 11/11/2020.
//

#ifndef LAB3_MATERIAL_H
#define LAB3_MATERIAL_H

class Material {
public:
    float ambientConstant[3];
    float diffuseConstant[3];
    float specularConstant[3];

    float specularN;
    float reflect;
    float refractionIndex;

    Material(float a[3], float d[3], float s[3], float sN = 100, float rN = 0.0, float rfI = 1.52)
    {
        for (int i = 0; i < 3; i++) {
            ambientConstant[i] = a[i];
            diffuseConstant[i] = d[i];
            specularConstant[i] = s[i];

            if (a[i] + d[i] + s[i] > 1.0) {
                ambientConstant[i] = 0.33f;
                diffuseConstant[i] = 0.33f;
                specularConstant[i] = 0.33f;
            }
        }

        specularN = sN;
        reflect = rN;
        refractionIndex = rfI;
    }
};

#endif //LAB3_MATERIAL_H
