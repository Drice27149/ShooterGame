#pragma once

UENUM(BlueprintType)
enum class EHitType : uint8 
{
    NormalHit, // hit without impulse
    HeavyHit,  // hit with impulse
    Block, // hit when guarding
    Parry // hit when parrying
};

UENUM(BlueprintType)
enum class EHitDirection : uint8 
{
    Back,
    Front,
    Left,
    Right
};


