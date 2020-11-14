// Copyright Drice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/Weapon.h"
#include "ShooterCharacter.generated.h"

UCLASS(config=Game)
class AShooterCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "StateMachine")
    bool IsRunning();
    
    UFUNCTION(BlueprintCallable, Category = "StateMachine")
    bool IsJumping();
    
    UFUNCTION(BlueprintCallable, Category = "StateMachine")
    bool IsCrouching();
    
    UFUNCTION(BlueprintCallable, Category = "StateMachine")
    bool IsTurning();
    
    UFUNCTION(BlueprintCallable, Category = "StateMachine")
    bool IsViewMode();
    
    UFUNCTION(BlueprintCallable, Category = "StateMachine")
    FRotator GetAimOffset();
    
    float GetWalkSpeedMultiplier();
    
    float GetRunSpeedMultiplier();

protected:

    /** handle player input **/

    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnCrouchStart();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnCrouchEnd();

    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnEquipDefaultWeapon();

    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnJumpStart();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnJumpEnd();

    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
	void OnMoveForward(float Value);

    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
	void OnMoveRight(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnTurn(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnLookUp(float Value);

    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnRunStart();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnRunEnd();

    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnStartFire();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnViewModeStart();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnViewModeEnd();
    
private:
    UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
    AWeapon* CurrentWeapon;

    UPROPERTY(Replicated)
    bool bRunning = false;
    
    UPROPERTY(Replicated)
    bool bCrouching = false;
    
    UPROPERTY(Replicated)
    bool bJumping = false;
    
    UPROPERTY(Replicated)
    bool bTurning = false;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class AWeapon> DefaultWeaponClass;

    /**all client**/
    UFUNCTION(Reliable, NetMulticast)
    void AllClientStartFire();
    
    UFUNCTION(Reliable, NetMulticast)
    void AllClientSetViewMode(bool Value);
    
    /**server**/
    UFUNCTION(Reliable, Server)
    void ServerStartFire();
    
    UFUNCTION(Reliable, Server)
    void ServerEquipDefaultWeapon();
    
    UFUNCTION(Reliable, Server)
    void ServerSetRunning(bool Value);
    
    UFUNCTION(Reliable, Server)
    void ServerSetJumping(bool Value);
    
    UFUNCTION(Reliable, Server)
    void ServerSetCrouching(bool Value);
    
    UFUNCTION(Reliable, Server)
    void ServerSetViewMode(bool Value);
    
    UFUNCTION(Reliable, Server)
    void ServerSetTurning(bool Value);
    
    UFUNCTION()
    void OnRep_CurrentWeapon();
    
protected:
    
    UPROPERTY(EditDefaultsOnly, Category = "Character Movement")
    float RunSpeedMultiplier;
    
    UPROPERTY(EditDefaultsOnly, Category = "Character Movement")
    float WalkSpeedMultiplier;
    
    UFUNCTION(BlueprintImplementableEvent)
    void UpdateWeaponMesh(AWeapon* MyWeapon);

    UFUNCTION(BlueprintImplementableEvent)
    void PlayWeaponChangedAnimation(AWeapon* MyWeapon);

    UFUNCTION(BlueprintImplementableEvent)
    void PlayWeaponFireAnimation(AWeapon* MyWeapon);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
private:
    UPROPERTY(EditDefaultsOnly, Category = "Charater Rotation")
    float MinTurnRate;
};


