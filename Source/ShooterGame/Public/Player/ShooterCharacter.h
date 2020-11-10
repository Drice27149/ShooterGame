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

    /** handle player input **/

    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnStartFire();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
	void OnMoveForward(float Value);

    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
	void OnMoveRight(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnEquipDefaultWeapon();

private:
    UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
    AWeapon* CurrentWeapon;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class AWeapon> DefaultWeaponClass;

    UFUNCTION()
    void OnRep_CurrentWeapon();
    
    UFUNCTION(Reliable, Server)
    void ServerStartFire();
    
    UFUNCTION(Reliable, NetMulticast)
    void AllClientStartFire();
    
    UFUNCTION(Reliable, Server)
    void ServerEquipDefaultWeapon();
    
protected:
    UFUNCTION(BlueprintImplementableEvent)
    void UpdateWeaponMesh(AWeapon* MyWeapon);

    UFUNCTION(BlueprintImplementableEvent)
    void PlayWeaponChangedAnimation(AWeapon* MyWeapon);

    UFUNCTION(BlueprintImplementableEvent)
    void PlayWeaponFireAnimation(AWeapon* MyWeapon);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};


