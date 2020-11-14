// Drice


#include "Player/MyCharacterMovementComponent.h"
#include "Player/ShooterCharacter.h"

float UMyCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

    AShooterCharacter* ShooterCharacterOwner = Cast<AShooterCharacter>(PawnOwner);
	if (ShooterCharacterOwner)
	{
		if (ShooterCharacterOwner->IsRunning())
		{
			MaxSpeed *= ShooterCharacterOwner->GetRunSpeedMultiplier();
		}
		else
        {
            MaxSpeed *= ShooterCharacterOwner->GetWalkSpeedMultiplier();
        }
	}

	return MaxSpeed;
}
