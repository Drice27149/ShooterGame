// Copyright Drice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/Weapon.h"
#include "ShooterCharacter.generated.h"

USTRUCT(Blueprintable)
struct FStateMachine
{
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY(BlueprintReadOnly, Category = "StateItem")
    bool IsCrouching;
    
    UPROPERTY(BlueprintReadOnly, Category = "StateItem")
    bool IsJumping;
    
    UPROPERTY(BlueprintReadOnly, Category = "StateItem")
    bool IsRunning;
    
    UPROPERTY(BlueprintReadOnly, Category = "StateItem")
    bool IsViewMode;
    
    UPROPERTY(BlueprintReadOnly, Category = "StateItem")
    bool IsTurning;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StateItem")
    float AimYaw;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StateItem")
    float AimPitch;
    
    FStateMachine()
    {
        IsCrouching = false;
        IsJumping = false;
        IsRunning = false;
        IsViewMode = false;
        IsTurning = false;
        AimYaw = 0.0f;
        AimPitch = 0.0f;
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

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class AWeapon> DefaultWeaponClass;

    UFUNCTION(Reliable, NetMulticast)
    void AllClientStartFire();
    
    UFUNCTION(Reliable, Server)
    void ServerStartFire();
    
    UFUNCTION(Reliable, Server)
    void ServerEquipDefaultWeapon();
    
    UFUNCTION(Reliable, Server)
    void ServerUpdateStateMachine(const FStateMachine& NewState);
    
    UFUNCTION(Reliable, Server)
    void ServerChangeWalkSpeed(const int& NewSpeed);
    
    UFUNCTION()
    void OnRep_CurrentWeapon();
    
protected:
    /** replicate variable that controll the animation state machine, in order to replicate animation **/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "StateMachine")
    FStateMachine StateMachine;
    
    UPROPERTY(EditDefaultsOnly, Category = "Pawn Movement")
    float WalkSpeed;
    
    UPROPERTY(EditDefaultsOnly, Category = "Pawn Movement")
    float RunSpeed;
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation Data")
    float MyDeltaTime;
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation Data")
    float AimSpeed;
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation Data")
    float TurnSpeed;
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation Data")
    float TurnMinimum;

    UFUNCTION(BlueprintImplementableEvent)
    void UpdateWeaponMesh(AWeapon* MyWeapon);

    UFUNCTION(BlueprintImplementableEvent)
    void PlayWeaponChangedAnimation(AWeapon* MyWeapon);

    UFUNCTION(BlueprintImplementableEvent)
    void PlayWeaponFireAnimation(AWeapon* MyWeapon);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    UFUNCTION(BlueprintCallable, Category = "Animation Data")
    float GetNextAimYaw(float LastAimYaw);
    
    UFUNCTION(BlueprintCallable, Category = "Animation Data")
    float GetNextAimPitch(float LastAimPitch);
    
    UFUNCTION(BlueprintCallable, Category = "Animation Data")
    bool ActorWillTurn(float lastYaw, float NextYaw);
};


