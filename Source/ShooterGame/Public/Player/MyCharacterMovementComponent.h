// Drice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UMyCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
    virtual float GetMaxSpeed() const override;
};
