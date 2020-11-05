// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShooterHUD.h"
#include "UI/MatchInfoWidget.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "TimerManager.h"

void AShooterHUD::NotifyPlayerNameChange(FString NewName)
{
    if(MatchInfoWidget != NULL)
    {
        MatchInfoWidget->SetNameText(NewName);
    }
}
    
void AShooterHUD::NotifyScoreChange(FString NewScore)
{
    if(MatchInfoWidget != NULL)
    {
        MatchInfoWidget->SetScoreText(NewScore);
    }
}
    
void AShooterHUD::NotifyMatchMessage(FString Message,float MessageDuration)
{
    if(MatchInfoWidget != NULL)
    {
        MatchInfoWidget->SetMessageText(Message);
        GetWorldTimerManager().ClearTimer(MessageTimer);
        GetWorldTimerManager().SetTimer(MessageTimer, this, &AShooterHUD::ClearMatchMessage, MessageDuration, false);
    }
}

void AShooterHUD::ClearMatchMessage()
{
    if(MatchInfoWidget != NULL)
    {
        MatchInfoWidget->SetMessageText(FString(""));
    }
}

void AShooterHUD::NotifyTimeChange(FString NewTime)
{
    if(MatchInfoWidget != NULL)
    {
        MatchInfoWidget->SetRemainTimeText(NewTime);
    }
}
