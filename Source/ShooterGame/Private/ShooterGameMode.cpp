// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "Player/ShooterCharacter.h"
#include "UI/ShooterHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Containers/UnrealString.h"
#include "Engine/EngineTypes.h"
#include "Player/ShooterPlayerController.h"

AShooterGameMode::AShooterGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
    DefaultPawnClass = PlayerPawnBPClass.Class;
    
    HUDClass = AShooterHUD::StaticClass();
	PlayerControllerClass = AShooterPlayerController::StaticClass();
}

void AShooterGameMode::StartPlay()
{
	Super::StartPlay();
	BeginMatch();
}

void AShooterGameMode::BeginMatch()
{
    MatchState = EMatchState::BeforeMatch;
    StateRemainTime = WarmUpDuration;
    GetWorldTimerManager().SetTimer(MatchTimer, this, &AShooterGameMode::HoldingMatch, OneSecond, false);
}

void AShooterGameMode::HoldingMatch()
{
    StateRemainTime--;
    if(StateRemainTime==0)
    { 
        if(MatchState==EMatchState::BeforeMatch) // change to next state
        {
            StateRemainTime = MatchDuration;
            MatchState = EMatchState::DuringMatch;
            GetWorldTimerManager().SetTimer(MatchTimer, this, &AShooterGameMode::HoldingMatch, OneSecond, false);
        }
        else // match was ended
        {
            StateRemainTime = 0;
            MatchState = EMatchState::AfterMatch;
        }
    }
    else
    {
        GetWorldTimerManager().SetTimer(MatchTimer, this, &AShooterGameMode::HoldingMatch, OneSecond, false);
    }
    
    for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        AShooterPlayerController* MyController = Cast<AShooterPlayerController>(*It);
        if(MyController != NULL)
        {
            FString NewTime = FString::Printf(TEXT("%d"),StateRemainTime);
            MyController->ClientChangeTime(NewTime);
        }
    }
}

int AShooterGameMode::GetStateRemainTime()
{
    return StateRemainTime;
}

void AShooterGameMode::HandleHitScore(AShooterCharacter* ShooterPawn,float ScoreRadius,FVector CenterLocation,FVector HitLocation){
    if(MatchState==EMatchState::DuringMatch)
    {
        //Caculate the hit score
        float Distance = (CenterLocation-HitLocation).Size();
        float FullScore = 10.0f;
        float Score = (ScoreRadius-Distance)/ScoreRadius*FullScore;
        if(Score<0)
        {
            Score = 0;
        }
        //add the hit score
        APlayerState* PlayerState = Cast<APlayerState>(ShooterPawn->GetPlayerState());
        if (PlayerState != NULL)
        {
            float NewScore = PlayerState->GetScore()+Score;
            PlayerState->SetScore(NewScore);
            //notify client the hit score
            AShooterPlayerController* MyController = Cast<AShooterPlayerController>(ShooterPawn->GetController());
            if(MyController != NULL)
            {
                MyController->ClientChangeScore(FString::Printf(TEXT("%.2f"),NewScore));
                float ScoreMessageDuration = 2.0f;
                MyController->ClientRecieveMessage(FString::Printf(TEXT("+ %.2f"),Score),ScoreMessageDuration);
            }
        }
    }
}


