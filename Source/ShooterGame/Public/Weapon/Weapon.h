// Drice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Weapon.generated.h"

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

    APawn* OwnerPawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
