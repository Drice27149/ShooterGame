// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ShooterHUD.h"
#include "UI/MatchInfoWidget.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AShooterHUD::AShooterHUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> MyMatchInfoWidget(TEXT("/Game/Blueprints/UI/BPMatchInfoWidget"));
    if(MyMatchInfoWidget.Class != NULL)
    {
        MatchInfoWidgetClass = MyMatchInfoWidget.Class;
    }
}

void AShooterHUD::LoadMatchInfoWidget()
{
    if(MatchInfoWidgetClass != NULL){
        MatchInfoWidget = CreateWidget<UMatchInfoWidget>(PlayerOwner,MatchInfoWidgetClass);
        if(MatchInfoWidget != NULL)
        {
            MatchInfoWidget->AddToViewport();
        }
    }
}

void AShooterHUD::NotifyPlayerNameChange()
{
    if(MatchInfoWidget != NULL)
    {
        MatchInfoWidget->OnNameChange();
    }
}
    
void AShooterHUD::NotifyScoreChange()
{
    if(MatchInfoWidget != NULL)
    {
        MatchInfoWidget->OnScoreChange();
    }
}
    
void AShooterHUD::NotifyTimeChange(FString NewTime)
{
    if(MatchInfoWidget != NULL)
    {
        MatchInfoWidget->OnTimerChange(NewTime);
    }
}
    
void AShooterHUD::NotifyMatchMessage(FString Message)
{
    if(MatchInfoWidget != NULL)
    {
       MatchInfoWidget->OnRecieveServerMessage(Message);
    }
}
