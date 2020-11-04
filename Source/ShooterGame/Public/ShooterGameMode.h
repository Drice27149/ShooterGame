// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "ShooterGameMode.generated.h"

class AShooterCharacter;

enum EMatchState{
    BeforeMatch,
    DuringMatch,
    AfterMatch
};

UCLASS(minimalapi)
class AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()
    
    

public:
	AShooterGameMode();
    
    /** Calculate and add PlayerScore when hitting target **/
    void HandleHitScore(AShooterCharacter* ShooterPawn,float ScoreRadius,FVector CenterLocation,FVector HitLocation);

    /** set up matchtimer **/
    virtual void StartPlay() override;

    /** display remain time on UI/hud **/
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    int GetStateRemainTime();

protected:
    void BeginMatch();
    
    /** called every second during match, used to count down **/
    void HoldingMatch();
    
private:
    const int WarmUpDuration = 10;
    
    const int MatchDuration = 20;
    
    const double OneSecond = 1.0f;
    
    EMatchState MatchState;
    
    int StateRemainTime;
    
    FTimerHandle MatchTimer;
};



