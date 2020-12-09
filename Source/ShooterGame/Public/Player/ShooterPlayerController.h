// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:    
    void BeginDelayedRespawn(float DelayedTime);
    
    void RespawnPlayerPawn();
    
    void NotifyKilled(AShooterPlayerState* KillerPlayerState, AShooterPlayerState* KilledPlayerState);
    
    UFUNCTION(Reliable, Client)
    void ClientNotifyKill(class AShooterPlayerState* KilledPlayerState);
    
    UFUNCTION(Reliable, Client)
    void ClientNotifySelfDeath(class AShooterPlayerState* KillerPlayerState);
    
    UFUNCTION(Reliable, Client)
    void ClientNotifyOtherDeath(class AShooterPlayerState* KillerPlayerState, class AShooterPlayerState* KilledPlayerState);
    
    UFUNCTION(Reliable, Client)
    void ClientBeginDelayRespawn(float DelayTime);
    
    UFUNCTION(Reliable, Client)
    void ClientRespawnComplete();
    
    UFUNCTION(Reliable, Client)
    void ClientMatchStart();

    UFUNCTION(Reliable, Client)
    void ClientMatchEnd();

    UFUNCTION(Reliable, Client)
    void ClientScoreChange();

    UFUNCTION(Reliable, Client)
    void ClientTimeChange(const int& NewTime);

    UFUNCTION(Reliable, Client)
    void ClientRecieveMessage(const FString& Message);
    
private:
    struct FTimerHandle RespawnTimer;
};
