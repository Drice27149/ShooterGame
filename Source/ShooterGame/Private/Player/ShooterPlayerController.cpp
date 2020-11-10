// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ShooterPlayerController.h"
#include "UI/ShooterHUD.h"
#include "Containers/UnrealString.h"

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

