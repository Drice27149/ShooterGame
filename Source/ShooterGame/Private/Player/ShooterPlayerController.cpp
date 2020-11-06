// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ShooterPlayerController.h"
#include "UI/ShooterHUD.h"
#include "Containers/UnrealString.h"

void AShooterPlayerController::ClientChangeScore_Implementation(const FString& NewScore)
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        MyShooterHUD->NotifyScoreChange(NewScore);
    }
}

void AShooterPlayerController::ClientChangeName_Implementation(const FString& NewName)
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        MyShooterHUD->NotifyPlayerNameChange(NewName);
    }
}

void AShooterPlayerController::ClientChangeTime_Implementation(const FString& NewTime)
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        MyShooterHUD->NotifyTimeChange(NewTime);
    }
}

void AShooterPlayerController::ClientRecieveMessage_Implementation(const FString& Message, float MessageDuration)
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        MyShooterHUD->NotifyMatchMessage(Message,MessageDuration);
    }
}

void AShooterPlayerController::ClientMatchStart_Implementation()
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        FString StartMessage = FString::Printf(TEXT("Game Start"));
        float Duration = 3.0f;
        MyShooterHUD->NotifyMatchMessage(StartMessage,Duration);
    }
}

void AShooterPlayerController::ClientMatchEnd_Implementation()
{
    AShooterHUD* MyShooterHUD = Cast<AShooterHUD>(GetHUD());
    if(MyShooterHUD != NULL)
    {
        FString EndMessage = FString::Printf(TEXT("Game End"));
        float Duration = 3.0f;
        MyShooterHUD->NotifyMatchMessage(EndMessage,Duration);
    }
}

