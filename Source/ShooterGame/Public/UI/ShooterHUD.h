// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/MatchInfoWidget.h"
#include "Engine/EngineTypes.h"
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
    
    AShooterHUD();

    /** display match information during game play **/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MatchInfo)
    UMatchInfoWidget* MatchInfoWidget = NULL;
    
    UFUNCTION(BlueprintCallable, Category = "MatchInformation")
    void LoadMatchInfoWidget();
    
    void NotifyPlayerNameChange();
    
    void NotifyScoreChange();
    
    void NotifyMatchMessage(FString Message);
    
    void NotifyTimeChange(FString NewTime);
    
private:
    TSubclassOf<class UUserWidget> MatchInfoWidgetClass;
};
