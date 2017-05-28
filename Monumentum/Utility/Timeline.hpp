#pragma once

#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>

// Overload GetValue(pastFrame, factor) for custom interpolation/extrapolation

namespace Monumentum {

enum TIMELINE_METHOD {
    DIRECT,
    NEAREST_NEIGHBOUR,
    LINEAR,
    COSINE,
    CUBIC,
    HERMITE,
    CUSTOM
};

template <typename T>
class Timeline {
public:
    Timeline(const int bufferSize = 64) : BufferSize(size),
        CurrentFrame(curFrame) {
        if (bufferSize < 0)
            size = 0;
        else
            size = bufferSize;
        buffer = new T[size];
        curFrame = 0;
        Count = 0;
        Method = TIMELINE_METHOD::LINEAR;
        Factor = 0.0f;
        ClampFactor = false;
        HermiteTension = 0.0f;
        HermiteBias = 0.0f;
        usedMethod = pastFrameAl = 0;
    }
    
    virtual ~Timeline() {
        delete[] buffer;
    }

    void Resize(const int bufferSize) {
        if (bufferSize > 0 && bufferSize != size) {
            size = bufferSize;
            delete[] buffer;
            buffer = new T[size];
        }
    }
    
    T GetStrict(const int frame) {
        frame %= size;
        if (frame < 0)
            frame += size;
        return buffer[frame];
    }

    void Set(const T &value) {
        if (Count > 0) {
            if (++curFrame >= size)
                curFrame = 0;
        }
        buffer[curFrame] = value;   
        Factor = 0.0f;
        if (Count < size)
            Count++;
    }
    
    T GetWithOffset(const int offset, const float factor, const float advance) {
        return Get(CurrentFrame - offset, factor, advance);
    }
    
    T Get(const int pastFrame, const float factor, const float advance = 0.0f) {
        if (Count == 0) {
            return dummy;
        }
        else if (Count == 1)
            return buffer[curFrame];
        else if ((Method == TIMELINE_METHOD::CUBIC ||
            Method == TIMELINE_METHOD::HERMITE) && Count < 3)
            usedMethod = TIMELINE_METHOD::LINEAR;
        else
            usedMethod = Method;
        pastFrameAl = AlignFrame(pastFrame);
        if (pastFrameAl == curFrame)
            return buffer[curFrame];
        else if (pastFrameAl > Count)
            return buffer[curFrame];
        float f = factor;
        if (ClampFactor) {
            if (f < 0.0f)
                f = 0.0f;
            else if (f > 1.0f)
                f = 1.0f;
        }
        Factor = f + advance;
        switch (usedMethod) {
        case TIMELINE_METHOD::DIRECT:
            return buffer[pastFrameAl];
        case TIMELINE_METHOD::NEAREST_NEIGHBOUR:
            if (f < 0.5f)
                return buffer[pastFrameAl];
            else
                return buffer[pastFrameAl + 1 % size];
        case TIMELINE_METHOD::LINEAR:
            return Normalize(Lerp(buffer[pastFrameAl], buffer[(pastFrameAl + 1) % size], f));
        case TIMELINE_METHOD::COSINE:
            return Normalize(CosineInterpolate(buffer[pastFrameAl], buffer[(pastFrameAl + 1) % size], f));
        case TIMELINE_METHOD::CUBIC:
            if (pastFrameAl == AlignFrame(curFrame - 1))
                return Normalize(CubicInterpolate(buffer[AlignFrame(pastFrameAl - 1)], buffer[pastFrameAl],
                    buffer[curFrame], Lerp(buffer[pastFrameAl], buffer[curFrame], 2.0f), f));
            else
                return Normalize(CubicInterpolate(buffer[AlignFrame(pastFrameAl - 1)], buffer[pastFrameAl],
                    buffer[pastFrameAl % size], buffer[(pastFrameAl + 2) % size], f));
        case TIMELINE_METHOD::HERMITE:
            if (pastFrameAl == AlignFrame(curFrame - 1))
                return Normalize(HermiteInterpolate(buffer[AlignFrame(pastFrameAl - 1)], buffer[pastFrameAl],
                    buffer[curFrame], Lerp(buffer[pastFrameAl], buffer[curFrame], 2.0f), f, HermiteTension,
                    HermiteBias));
            else
                return Normalize(HermiteInterpolate(buffer[AlignFrame(pastFrameAl - 1)], buffer[pastFrameAl],
                    buffer[(pastFrameAl + 1) % size], buffer[(pastFrameAl + 2) % size], f, HermiteTension,
                    HermiteBias));
        default: // Custom implementations
            return GetValue(pastFrameAl, f);
        }
    }
    
    int Method, Count;
    float Factor;
    bool ClampFactor;
    float HermiteTension, HermiteBias;
    
    const int &CurrentFrame;
    const int &BufferSize;
protected:
    T *buffer;
    T dummy;
    int size, curFrame;

    inline virtual T GetValue(const int pastFrame, const float factor) {
        return buffer[pastFrame];
    }
    
    inline virtual T Add(const T &a, const T &b) {
        return a + b;
    }
    
    inline virtual T Sub(const T &a, const T &b) {
        return a - b;
    }
    
    inline virtual T Mul(const T &a, const float b) {
        return a * b;
    }
    
    
    virtual T Normalize(const T &a) {
        return a;
    }
    
    int AlignFrame(const int frame) {
        int f = frame;
        f %= size;
        if (f < 0)
            f += size;
        return f;
    }
    
    T Lerp(const T &a, const T &b, const float factor) {
        return Add(Mul(a, 1.0f - factor), Mul(b, factor));
    }
    
    T CosineInterpolate(const T &a, const T &b, const float factor) {
        float factor2 = (1.0f - (float)cosf(factor * (float)M_PI)) * 0.5f;
        return Add(Mul(a, 1.0f - factor2), Mul(b, factor2));
    }
    
    T CubicInterpolate(const T &aPrev, const T &a, const T &b, const T &bNext, const float factor) {
        T x0, x1, x2, x3;
        float factor2 = factor * factor;
        x0 = Add(Sub(Sub(bNext, b), aPrev) ,a);
        x1 = Sub(Sub(aPrev, a), x0);
        x2 = Sub(b, aPrev);
        x3 = a;
        return Add(Add(Add(Mul(x0, factor * factor2), Mul(x1, factor2)), Mul(x2, factor)), x3);
    }
    
    T HermiteInterpolate(const T &aPrev, const T &a, const T &b, const T &bNext, const float factor,
        const float tension, const float bias) {
        T m0, m1;
        float factor2, factor3, a0, a1, a2, a3;
        factor2 = factor * factor;
        factor3 = factor2 * factor;
        m0 = Mul(Sub(a, aPrev), (1.0f + bias) * (1.0f - tension) * 0.5f);
        m0 = Mul(Add(m0, Sub(b, a)), (1.0f - bias) * (1.0f - tension) * 0.5f);
        m1 = Mul(Sub(b, a), (1.0f + bias) * (1.0f - tension) * 0.5f);
        m1 = Mul(Add(m1, Sub(bNext, b)), (1.0f - bias) * (1.0f - tension) * 0.5f);
        a0 = 2.0f * factor3 - 3.0f * factor2 + 1.0f;
        a1 = factor3 - 2.0f * factor2 + factor;
        a2 = factor3 - factor2;
        a3 = -2.0f * factor3 + 3.0f * factor2;
        return Add(Add(Add(Mul(a, a0), Mul(m0, a1)), Mul(m1, a2)), Mul(b, a3));
    }
private:
    int usedMethod, pastFrameAl;
};

}
