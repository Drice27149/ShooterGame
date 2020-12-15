// Copyright Drice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterGameType.h"
#include "ShooterCharacter.generated.h"

class AWeapon;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FTakeHitInfo
{
	GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FVector HitImpulse;
    
    UPROPERTY()
    FVector HitVector;

    UPROPERTY()
    EHitType HitType;

    UPROPERTY()
    int8 HitCounter;
    
    UPROPERTY()
    FName HitBoneName;
    
    UPROPERTY()
    bool bDeath;

	FTakeHitInfo()
    {
        HitType = EHitType::NormalHit;
        HitCounter = 0;
        bDeath = false;
    }
};

UCLASS(config=Game)
class AShooterCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()
public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "StateMachine")
    bool IsRunning();
    
    UFUNCTION(BlueprintCallable, Category = "StateMachine")
    bool IsAiming();
    
    UFUNCTION(BlueprintCallable, Category = "StateMachine")
    FRotator GetAimOffset();
    
    UFUNCTION(BlueprintCallable, Category = "StateMachine")
    int GetCurrentWeaponType();
    
    float GetWalkSpeedMultiplier();
    
    float GetRunSpeedMultiplier();
    
    float PlayCharacterMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.0f);
    
    void PlayHit(AShooterCharacter* OtherCharacter, EHitType HitType, float HitDamage, FVector HitVector, FVector HitImpulse, FName HitBoneName);

    UFUNCTION(BlueprintCallable)
    void SetPickUpWeapon(AWeapon* PickUpWeapon);
    
    void NotifyAmmoCountChange(int32 AmmoCount);

protected:
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentWeapon)
    AWeapon* CurrentWeapon;

    /** handle player input **/

    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnCrouchStart();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnCrouchEnd();

    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnJumpStart();

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
    void OnStartReload();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnUnEquip();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnPickUp();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnDrop();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnSwitchWeapon(int direction);
    
    UFUNCTION(BlueprintCallable, Category = "Animation Notify")
    void EquipWeaponMesh();
    
    UFUNCTION(BlueprintCallable, Category = "Animation Notify")
    void UnequipWeaponMesh();
    
    UFUNCTION(BlueprintCallable, Category = "Animation Notify")
    void DropWeaponMesh();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnAim();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnUnAim();
    
private:
    struct FTimerHandle CheatTimer;
    
    UPROPERTY(ReplicatedUsing = OnRep_bCheat)
    bool bCheat = false;
    
    UPROPERTY(Replicated)
    bool bBusy = false;

    UPROPERTY(Replicated)
    bool bAiming = false;
    
    UPROPERTY(ReplicatedUsing = OnRep_PickUpWeapon)
    AWeapon* PickUpWeapon;
    
    UPROPERTY(EditDefaultsOnly)
    float MaxHealth;

    UPROPERTY(Replicated)
    bool bRunning = false;

    UPROPERTY(ReplicatedUsing = OnRep_LastHitInfo)
    FTakeHitInfo LastHitInfo;
    
    UFUNCTION(Reliable, NetMulticast)
    void MulticastPlayMontage(UAnimMontage* AnimMontage, bool bSkipOwner = false);
    
    UFUNCTION(Reliable, Server)
    void ServerSetCurrentWeapon(AWeapon* NewWeapon, bool bDropLastWeapon = false);
    
    UFUNCTION(Reliable, Server)
    void ServerSetRunning(bool Value);
    
    UFUNCTION(Reliable, Server)
    void ServerSetAiming(bool Value);
    
    UFUNCTION()
    void OnRep_PickUpWeapon();
    
    UFUNCTION()
    void OnRep_CurrentWeapon(AWeapon* LastWeapon);
    
    UFUNCTION()
    void OnRep_LastHitInfo();
    
    UFUNCTION()
    void OnRep_Health();
    
    UFUNCTION()
    void OnRep_bCheat();
    
    void OnDeath();
    
    void SimulateHit();
    
    void SimulateDeath();
    
    void SwitchToNewWeapon(AWeapon* NewWeapon);
    
    void EnableCheat();
    
    void UnableCheat();
    
protected:
    UPROPERTY()
    TArray<AWeapon*> Inventory;
    
    UPROPERTY()
    int CurrentWeaponIndex;

    UPROPERTY()
    AWeapon* LastEquipWeapon;

    UPROPERTY(Replicated, ReplicatedUsing = OnRep_Health)
    float Health;
    
    UPROPERTY(EditDefaultsOnly, Category = "Cheat")
	float RespawnCheatTime = 5.0f;
    
    UPROPERTY(EditDefaultsOnly, Category = "Character Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Character Camera")
	class UCameraComponent* FollowCamera;
    
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* FrontHitMontage;
    
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* BackHitMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Character Movement")
    float RunSpeedMultiplier;
    
    UPROPERTY(EditDefaultsOnly, Category = "Character Movement")
    float WalkSpeedMultiplier;
    
    UPROPERTY(EditDefaultsOnly, Category = "RespawnTime")
    float RespawnTime;

    UFUNCTION(BlueprintImplementableEvent)
    void ReportDamageEventToAI(AActor* OtherActor);

    UFUNCTION(BlueprintImplementableEvent)
    void FireTest(int signal);

    UFUNCTION(BlueprintImplementableEvent)
    void FireSound(const FString& BoneName);
    
    UFUNCTION(BlueprintImplementableEvent)
    void FireFloat(float signal);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};


