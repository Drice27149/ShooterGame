// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    void ScoreKill();
    
    void ScoreDeath();
    
    UFUNCTION(BlueprintCallable)
    int32 GetDeathCount();
    
    UFUNCTION(BlueprintCallable)
    int32 GetKillCount();

    UFUNCTION(BlueprintCallable)
    FString GetShooterPlayerName();
    
    UFUNCTION(BlueprintCallable)
    void SetShooterPlayerName(FString NewName);
    
    UFUNCTION(Reliable, Server)
    void ServerSetShooterPlayerName(const FString& NewName);

private:
    UPROPERTY(Replicated)
    FString ShooterPlayerName;
    
    UPROPERTY(Replicated)
    int32 KillCount;
    
    UPROPERTY(Replicated)
    int32 DeathCount;
};
