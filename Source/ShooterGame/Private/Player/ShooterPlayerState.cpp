// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ShooterPlayerState.h"
#include "ShooterGame.h"
#include "Net/UnrealNetwork.h"

void AShooterPlayerState::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AShooterPlayerState, ShooterPlayerName );
    
    DOREPLIFETIME_CONDITION(AShooterPlayerState, KillCount, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(AShooterPlayerState, DeathCount, COND_OwnerOnly);
}

FString AShooterPlayerState::GetShooterPlayerName()
{
    return ShooterPlayerName;
}
    
void AShooterPlayerState::SetShooterPlayerName(FString NewName)
{
    ServerSetShooterPlayerName(NewName);
}
    
void AShooterPlayerState::ServerSetShooterPlayerName_Implementation(const FString& NewName)
{
    ShooterPlayerName = NewName;
}

void AShooterPlayerState::ScoreKill()
{
    KillCount++;
}

void AShooterPlayerState::ScoreDeath()
{
    DeathCount++;
}

int32 AShooterPlayerState::GetKillCount()
{
    return KillCount;
}

int32 AShooterPlayerState::GetDeathCount()
{
    return DeathCount;
}
