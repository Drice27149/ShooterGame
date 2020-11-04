// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "Player/ShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Actor.h"

AShooterGameMode::AShooterGameMode()
{
	// set default pawn class to our Blueprinted character
    // would be useful
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    
    //DefaultPawnClass = AShooterCharacter::StaticClass();
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
            PlayerState->SetScore(PlayerState->GetScore()+Score);
        }
    }
}


