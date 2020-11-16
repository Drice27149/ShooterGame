// Drice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Weapon.generated.h"

class AShooterCharacter;

UCLASS()
class SHOOTERGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
    
    virtual bool CanFire();
    
    /** server only, only called by character on server **/
    virtual void HandleFire();
    
    UFUNCTION(BlueprintImplementableEvent)
    void PlayWeaponFireAnimation();
    
    /** server only **/ 
    void SetOwnerCharacter(AShooterCharacter* NewOwnerCharacter);
    
protected:
    /** replicate for calling cosmetic function on remote client **/
    UPROPERTY(Replicated)
    AShooterCharacter* OwnerCharacter;
};
