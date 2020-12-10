// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Player/ShooterCharacter.h"
#include "ShooterHUD.generated.h"

class AShooterPlayerState;

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "MatchInformation")
    void LoadMatchInfoWidget();
    
    UFUNCTION(BlueprintImplementableEvent, Category = "MatchInformation")
    void NotifyPlayerNameChange();
    
    UFUNCTION(BlueprintImplementableEvent, Category = "MatchInformation")
    void NotifyScoreChange();
    
    UFUNCTION(BlueprintImplementableEvent, Category = "MatchInformation")
    void NotifyMatchMessage(const FString& Message);
    
    UFUNCTION(BlueprintImplementableEvent, Category = "MatchInformation")
    void NotifyTimeChange(const FString& NewTime);
    
    UFUNCTION(BlueprintImplementableEvent, Category = "PickUpWeapon")
    void OnPickUpWeaponChange(const FString& NewWeaponName);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnKill(AShooterPlayerState* KilledPlayerState);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnSelfDeath(AShooterPlayerState* KillerPlayerState);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnOtherDeath(AShooterPlayerState* KillerPlayerState, AShooterPlayerState* KilledPlayerState);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnBeginDelayRespawn(float DelayTime);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnRespawnComplete();
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnHealthChange(float NewHealth, float Maxhealth);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnWeaponChange(class AWeapon* Weapon);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnHit(const FTakeHitInfo& TakeHitInfo);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnAmmoCountChange(int32 AmmoCount);
};
