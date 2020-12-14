// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "ShooterGame.h"
#include "Player/ShooterCharacter.h"
#include "Player/ShooterPlayerController.h"
#include "Player/ShooterPlayerState.h"
#include "UI/ShooterHUD.h"

AShooterGameMode::AShooterGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Player/BPShooterPawn"));
    static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDBPClass(TEXT("/Game/Blueprints/UI/BPShooterHUD"));
    
    DefaultPawnClass = PlayerPawnBPClass.Class;
    HUDClass = PlayerHUDBPClass.Class;
	PlayerControllerClass = AShooterPlayerController::StaticClass();
    PlayerStateClass = AShooterPlayerState::StaticClass();
    
    RespawnTime = 5.0f;
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
        if(MatchState==EMatchState::BeforeMatch) 
        {
            // match start
            StateRemainTime = MatchDuration;
            MatchState = EMatchState::DuringMatch;
            GetWorldTimerManager().SetTimer(MatchTimer, this, &AShooterGameMode::HoldingMatch, OneSecond, false);
            
            //notify all client match start
            for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
            {
                AShooterPlayerController* MyController = Cast<AShooterPlayerController>(*It);
                if(MyController != NULL)
                {
                    MyController->ClientMatchStart();
                }
            }
        }
        else 
        {
            // match end
            StateRemainTime = 0;
            MatchState = EMatchState::AfterMatch;
            
            //notify all client match end
            for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
            {
                AShooterPlayerController* MyController = Cast<AShooterPlayerController>(*It);
                if(MyController != NULL)
                {
                    MyController->ClientMatchEnd();
                }
            }
        }
    }
    else
    {
        GetWorldTimerManager().SetTimer(MatchTimer, this, &AShooterGameMode::HoldingMatch, OneSecond, false);
    }
    
    // broadcast match timer change
    for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        AShooterPlayerController* MyController = Cast<AShooterPlayerController>(*It);
        if(MyController != NULL)
        {
            MyController->ClientTimeChange(StateRemainTime);
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
                MyController->ClientScoreChange();
            }
        }
    }
}

void AShooterGameMode::Killed(AShooterPlayerController* KillerPC, AShooterPlayerController* KilledPC)
{
    AShooterPlayerState* KillerPlayerState = KillerPC?Cast<AShooterPlayerState>(KillerPC->PlayerState):NULL;
    AShooterPlayerState* KilledPlayerState = KilledPC?Cast<AShooterPlayerState>(KilledPC->PlayerState):NULL;
    
    if(KillerPlayerState && KilledPlayerState)
    {
        // update death and kill counter 
        KillerPlayerState->ScoreKill();
        KilledPlayerState->ScoreDeath();
        
        // broadcast death message
        for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
        {
            AShooterPlayerController* MyController = Cast<AShooterPlayerController>(*It);
            if(MyController != NULL)
            {
                MyController->NotifyKilled(KillerPlayerState, KilledPlayerState);
            }
        }
        
        // respawn the killed player 
        KilledPC->BeginDelayedRespawn(RespawnTime);
    }
}

void AShooterGameMode::KillByAI(AShooterPlayerController* KilledPC)
{
    AShooterPlayerState* KilledPlayerState = KilledPC?Cast<AShooterPlayerState>(KilledPC->PlayerState):NULL;
    if(KilledPlayerState)
    {
        KilledPlayerState->ScoreDeath();
        KilledPC->ClientNotifyKillByAI();
        KilledPC->BeginDelayedRespawn(RespawnTime);
    }
}
    
void AShooterGameMode::KillAI(AShooterPlayerController* KillerPC)
{
    AShooterPlayerState* KillerPlayerState = KillerPC?Cast<AShooterPlayerState>(KillerPC->PlayerState):NULL;
    if(KillerPlayerState)
    {
        KillerPlayerState->ScoreKill();
        KilledPC->ClientNotifyKillAI();
    }
}


