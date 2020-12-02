// Copyright Drice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterGameType.h"
#include "ShooterCharacter.generated.h"

class AWeapon;
class UAnimMontage;

USTRUCT()
struct FTakeHitInfo
{
	GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FVector HitImpulse;

    UPROPERTY()
    EHitType HitType;

    UPROPERTY()
    int8 HitDirection;
    
    UPROPERTY()
    int8 HitCounter;
    
    UPROPERTY()
    bool bDeath;

	FTakeHitInfo()
    {
        HitType = EHitType::NormalHit;
        HitDirection = 0;
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
    bool IsViewMode();
    
    UFUNCTION(BlueprintCallable, Category = "StateMachine")
    float GetTurnDirection();
    
    UFUNCTION(BlueprintCallable, Category = "StateMachine")
    FRotator GetAimOffset();
    
    UFUNCTION(BlueprintCallable, Category = "StateMachine")
    int GetCurrentWeaponType();
    
    float GetWalkSpeedMultiplier();
    
    float GetRunSpeedMultiplier();
    
    float PlayCharacterMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.0f);
    
    /** server **/
    void SetPickUpWeapon(AWeapon* NewPickUpWeapon);
    
    void PlayHit(AActor* OtherActor, EHitType HitType, float HitDamage, FVector HitVector, FVector HitImpulse, FString HitBoneName);

protected:

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
    void OnViewModeStart();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnViewModeEnd();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnStartReload();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnUnEquip();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnPickUp();
    
    UFUNCTION(BlueprintCallable, Category = "PlayerInput")
    void OnDrop();
    
private:
    struct FTimerHandle TurnTimer;

    int TurnCounter;

    UPROPERTY(Replicated)
    bool bBusy = false;

    UPROPERTY(ReplicatedUsing = OnRep_RotationYaw)
    float RotationYaw;
    
    float DeltaYaw;
    
    float DeltaTime;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
    AWeapon* CurrentWeapon;
    
    UPROPERTY(ReplicatedUsing = OnRep_PickUpWeapon)
    AWeapon* PickUpWeapon;
    
    UPROPERTY(EditDefaultsOnly)
    float MaxHealth;

    UPROPERTY(Replicated)
    bool bRunning = false;
    
    /** -1: Turning left, 0: Not turning, 1: Turning Right **/
    UPROPERTY(Replicated)
    float TurnDirection = 0.0f;

    UPROPERTY(ReplicatedUsing = OnRep_LastHitInfo)
    FTakeHitInfo LastHitInfo;

    /**Multicast**/
    UFUNCTION(Reliable, NetMulticast)
    void MulticastSetViewMode(bool Value);
    
    UFUNCTION(Reliable, NetMulticast)
    void MulticastPlayMontage(UAnimMontage* AnimMontage, bool bSkipOwner = false);
    
    /**Server**/
    UFUNCTION(Reliable, Server)
    void ServerTurnInPlace(int TurnType);
    
    UFUNCTION(Reliable, Server)
    void ServerSetCurrentWeapon(AWeapon* NewWeapon);
    
    UFUNCTION(Reliable, Server)
    void ServerDropCurrentWeapon();
    
    UFUNCTION(Reliable, Server)
    void ServerSetRunning(bool Value);
    
    UFUNCTION(Reliable, Server)
    void ServerSetViewMode(bool Value);
    
    UFUNCTION(Reliable, Server)
    void ServerSetTurnDirection(float Value);
    
    UFUNCTION()
    void OnRep_PickUpWeapon();
    
    UFUNCTION()
    void OnRep_CurrentWeapon(AWeapon* LastWeapon);
    
    UFUNCTION()
    void OnRep_LastHitInfo();
    
    UFUNCTION()
    void OnRep_RotationYaw();
    
    void OnDeath();
    
    void SimulateHit();
    
    void SimulateDeath();
    
    void TurnInPlace();
    
protected:
    UPROPERTY(EditDefaultsOnly)
    float MinTurnDelta;

    UPROPERTY(EditDefaultsOnly)
    float Turn90Delta;
    
    UPROPERTY(EditDefaultsOnly)
    float Turn180Delta;
    
    UPROPERTY(Replicated, BlueprintReadOnly)
    float Health;
    
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* TakeHitMontage[5];
    
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* TurnInPlaceMontage[4];

    UPROPERTY(EditDefaultsOnly, Category = "Character Movement")
    float RunSpeedMultiplier;
    
    UPROPERTY(EditDefaultsOnly, Category = "Character Movement")
    float WalkSpeedMultiplier;
    
    UPROPERTY(EditDefaultsOnly, Category = "RespawnTime")
    float RespawnTime;

    UFUNCTION(BlueprintImplementableEvent)
    void FireTest(int signal);

    UFUNCTION(BlueprintImplementableEvent)
    void FireSound(const FString& BoneName);
    
    UFUNCTION(BlueprintImplementableEvent)
    void FireFloat(float signal);
    
    UFUNCTION(BlueprintCallable)
    void TestPhysicHit(FVector HitImpulse, FName HitBoneName);
    
    UFUNCTION(BlueprintImplementableEvent)
    void SimulatePhysicHit(FVector HitImpulse, FName HitBoneName);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};


