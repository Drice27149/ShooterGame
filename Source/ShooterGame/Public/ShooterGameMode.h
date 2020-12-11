// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "ShooterGameMode.generated.h"

class AShooterCharacter;
class AShooterPlayerController;

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
    
    void Killed(AShooterPlayerController* KillerPC, AShooterPlayerController* KilledPC);

    /** display remain time on UI/hud **/
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    int GetStateRemainTime();

protected:
    
    void BeginMatch();
    
    /** called every second during match, used to count down **/
    void HoldingMatch();
    
private:
    const int WarmUpDuration = 30;
    
    const int MatchDuration = 120;
    
    const float OneSecond = 1.0f;
    
    float RespawnTime;
    
    EMatchState MatchState;
    
    int StateRemainTime;
    
    FTimerHandle MatchTimer;
};



