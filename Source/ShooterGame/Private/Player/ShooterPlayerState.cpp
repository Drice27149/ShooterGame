// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ShooterPlayerState.h"
#include "Net/UnrealNetwork.h"

void AShooterPlayerState::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AShooterPlayerState, ShooterName );
}
