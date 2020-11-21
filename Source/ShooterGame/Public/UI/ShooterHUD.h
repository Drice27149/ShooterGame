// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

class FString;

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
};
