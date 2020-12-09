// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ShooterPlayerController.h"
#include "ShooterGame.h"
#include "Player/ShooterPlayerState.h"
#include "UI/ShooterHUD.h"

void AShooterPlayerController::ClientScoreChange_Implementation()
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        MyShooterHUD->NotifyScoreChange();
    }
}

void AShooterPlayerController::ClientTimeChange_Implementation(const int& NewTime)
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        FString TimeText = FString::Printf(TEXT("%d"),NewTime);
        MyShooterHUD->NotifyTimeChange(TimeText);
    }
}

void AShooterPlayerController::ClientRecieveMessage_Implementation(const FString& Message)
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        MyShooterHUD->NotifyMatchMessage(Message);
    }
}

void AShooterPlayerController::ClientMatchStart_Implementation()
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        FString StartMessage = FString::Printf(TEXT("Game Start"));
        MyShooterHUD->NotifyMatchMessage(StartMessage);
    }
}

void AShooterPlayerController::ClientMatchEnd_Implementation()
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        FString EndMessage = FString::Printf(TEXT("Game End"));
        MyShooterHUD->NotifyMatchMessage(EndMessage);
    }
}

void AShooterPlayerController::BeginDelayedRespawn(float DelayedTime)
{
    GetWorldTimerManager().SetTimer(RespawnTimer, this, &AShooterPlayerController::RespawnPlayerPawn, DelayedTime, false);
    // notify hud begin respawn
    ClientBeginDelayRespawn(DelayedTime);
}

void AShooterPlayerController::RespawnPlayerPawn()
{
    ServerRestartPlayer();
    // notify hud respawn complete
    ClientRespawnComplete();
}

void AShooterPlayerController::NotifyKilled(AShooterPlayerState* KillerPlayerState, AShooterPlayerState* KilledPlayerState)
{
    if(KilledPlayerState == PlayerState)
    {
        ClientNotifySelfDeath(KillerPlayerState);
    }
    else if(KillerPlayerState == PlayerState)
    {
        ClientNotifyKill(KilledPlayerState);
    }
    else
    {
        ClientNotifyOtherDeath(KillerPlayerState, KilledPlayerState);
    }
}

void AShooterPlayerController::ClientNotifyKill_Implementation(class AShooterPlayerState* KilledPlayerState)
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        MyShooterHUD->OnKill(KilledPlayerState);
    }
}
    
void AShooterPlayerController::ClientNotifySelfDeath_Implementation(class AShooterPlayerState* KillerPlayerState)
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        MyShooterHUD->OnSelfDeath(KillerPlayerState);
    }
}
    
void AShooterPlayerController::ClientNotifyOtherDeath_Implementation(class AShooterPlayerState* KillerPlayerState, class AShooterPlayerState* KilledPlayerState)
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        MyShooterHUD->OnOtherDeath(KillerPlayerState, KilledPlayerState);
    }
}

void AShooterPlayerController::ClientBeginDelayRespawn_Implementation(float DelayTime)
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        MyShooterHUD->OnBeginDelayRespawn(DelayTime);
    }
}
    
void AShooterPlayerController::ClientRespawnComplete_Implementation()
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        MyShooterHUD->OnRespawnComplete();
    }
}

