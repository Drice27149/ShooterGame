// Copyright Drice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/Weapon.h"
#include "ShooterCharacter.generated.h"

USTRUCT(BlueprintType)
struct FStateMachine
{
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY()
    bool IsCrouching;
    
    UPROPERTY()
    bool IsJumping;
    
    UPROPERTY()
    bool IsRunning;
    
    UPROPERTY()
    bool IsViewMode;
    
    FStateMachine()
    {
        IsCrouching = false;
        IsJumping = false;
        IsRunning = false;
        IsViewMode = false;
    }
};

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
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnCrouchStart();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnCrouchEnd();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnJumpStart();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnJumpEnd();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnRunStart();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnRunEnd();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnViewModeStart();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnViewModeEnd();
    
private:
    UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
    AWeapon* CurrentWeapon;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class AWeapon> DefaultWeaponClass;

    UFUNCTION()
    void OnRep_CurrentWeapon();
    
    UFUNCTION(Reliable, Server)
    void ServerStartFire();
    
    UFUNCTION(Reliable, Server)
    void ServerEquipDefaultWeapon();
    
    UFUNCTION(Reliable, Server)
    void ServerUpdateStateMachine(const FStateMachine& NewState);
    
    UFUNCTION(Reliable, Server)
    void ServerChangeWalkSpeed(const int& NewSpeed);
    
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
    
    /** replicate variable that controll the animation state machine, in order to replicate animation **/
    UPROPERTY(BlueprintReadOnly, Replicated, Category = "StateMachine")
    FStateMachine StateMachine;
    
    UPROPERTY(EditDefaultsOnly, Category = "Pawn Movement")
    float WalkSpeed;
    
    UPROPERTY(EditDefaultsOnly, Category = "Pawn Movement")
    float RunSpeed;
};


