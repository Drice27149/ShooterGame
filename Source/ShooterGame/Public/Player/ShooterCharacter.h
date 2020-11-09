// Copyright Drice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/Weapon.h"
#include "ShooterCharacter.generated.h"

UCLASS(config=Game)
class AShooterCharacter : public ACharacter
{
	GENERATED_BODY()
public:

	AShooterCharacter();

protected:

    //handle player input
    
    void OnStartFire();
    
	void OnMoveForward(float Value);

	void OnMoveRight(float Value);

private:
    UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
    AWeapon* CurrentWeapon;

    UFUNCTION()
    void OnRep_CurrentWeapon();
    
    UFUNCTION(Reliable, Server)
    void ServerStartFire();
    
    UFUNCTION(Reliable, NetMulticast)
    void AllClientStartFire();
    
protected:
    UFUNCTION(BlueprintImplementableEvent)
    void UpdateWeaponMesh(AWeapon* MyWeapon);

    UFUNCTION(BlueprintImplementableEvent)
    void PlayWeaponChangedAnimation(AWeapon* MyWeapon);

    UFUNCTION(BlueprintImplementableEvent)
    void PlayWeaponFireAnimation(AWeapon* MyWeapon);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};


