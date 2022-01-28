#pragma once

struct Quaterniond
{
public:
    Quaterniond(){};

    Quaterniond(double InW, double InX, double InY, double InZ)
    {
        W = InW;
        X = InX;
        Y = InY;
        Z = InZ;
    }

    Quaterniond operator * (Quaterniond MultQuad)
    {
        Quaterniond result;
        result.W = W*MultQuad.W - X*MultQuad.X - Y*MultQuad.Y - Z*MultQuad.Z;
        result.X = W*MultQuad.X + X*MultQuad.W + Y*MultQuad.Z - Z*MultQuad.Y;
        result.Y = W*MultQuad.Y - X*MultQuad.Z + Y*MultQuad.W + Z*MultQuad.X;
        result.Z = W*MultQuad.Z + X*MultQuad.Y - Y*MultQuad.X + Z*MultQuad.W;
        return result;
    }

    double NormSquared()
    {
        return W*W + X*X + Y*Y + Z*Z;
    }

    Quaterniond Inverse()
    {
        double Norm = NormSquared();
        Quaterniond result;
        result.W = W/Norm;
        result.X = -X/Norm;
        result.Y = -Y/Norm;
        result.Z = -Z/Norm;
        return result;
    }

    double W;
    double X;
    double Y;
    double Z;


};

class Vector3d
{

};

class MathFunctionLibrary
{
public:
    static Quaterniond GetDeltaQuad(Quaterniond PreQuad, Quaterniond CurrentQuad);

    static Vector3d GetDeltaTranslation(Vector3d PreTranslation, Vector3d CurrentTranslation);

    static MathFunctionLibrary* MathFunLibSingleton;
};