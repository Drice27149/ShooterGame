// Copyright Drice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AWeapon;

UCLASS(config=Game)
class AShooterCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()
public:
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
    UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
    AWeapon* CurrentWeapon;

    UPROPERTY(Replicated)
    bool bRunning = false;
    
    /** -1: Turning left, 0: Not turning, 1: Turning Right **/
    UPROPERTY(Replicated)
    float TurnDirection = 0.0f;

    /**Multicast**/
    UFUNCTION(Reliable, NetMulticast)
    void MulticastSetViewMode(bool Value);
    
    /**Server**/
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
    
protected:
    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_PickUpWeapon)
    AWeapon* PickUpWeapon;

    UPROPERTY(EditDefaultsOnly, Category = "Character Movement")
    float RunSpeedMultiplier;
    
    UPROPERTY(EditDefaultsOnly, Category = "Character Movement")
    float WalkSpeedMultiplier;

    UFUNCTION(BlueprintImplementableEvent)
    void FireTest(int signal);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};


