#pragma once
#include "../Core/Base.h"
#include "BaseEffect.h"

EFFECTS_NAMESPACE_BEGIN

class ClothEffect : public BaseEffect
{
public:
    ClothEffect(int width, int height);
    ~ClothEffect();
    void prepare() override ;
    void update(float t) override ;
    void render() override;
};

EFFECTS_NAMESPACE_END

