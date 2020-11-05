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
    /** display match information during game play **/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MatchInfo)
    UMatchInfoWidget* MatchInfoWidget = NULL;
    
    void NotifyPlayerNameChange(FString NewName);
    
    void NotifyScoreChange(FString NewScore);
    
    void NotifyMatchMessage(FString Message,float MessageDuration);

    void ClearMatchMessage();
    
    void NotifyTimeChange(FString NewTime);

private:
    FTimerHandle MessageTimer;
};
