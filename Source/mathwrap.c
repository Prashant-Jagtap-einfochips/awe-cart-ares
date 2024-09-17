#define _Check_return_
#define __CRTDECL
#define _In_
#define _Out_

_Check_return_ float __CRTDECL floorf(_In_ float _X)
{
    return (float)floor(_X);
}

_Check_return_ float __CRTDECL ceilf(_In_ float _X)
{
    return (float)ceil(_X);
}
_Check_return_ float __CRTDECL logf(_In_ float _X)
{
    return (float)log(_X);
}
_Check_return_ float __CRTDECL log10f(_In_ float _X)
{
    return (float)log10(_X);
}
_Check_return_ float __CRTDECL modff(_In_ float _X, _Out_ float* _Y)
{
    double _F, _I;
    _F = modf(_X, &_I);
    *_Y = (float)_I;
    return (float)_F;
}
_Check_return_ float __CRTDECL expf(_In_ float _X)
{
    return (float)exp(_X);
}
_Check_return_ float __CRTDECL sqrtf(_In_ float _X)
{
    return (float)sqrt(_X);
}

