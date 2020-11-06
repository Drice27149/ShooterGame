// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/ShooterHUD.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    UFUNCTION(Reliable, Client)
    void ClientMatchStart();

    UFUNCTION(Reliable, Client)
    void ClientMatchEnd();

    UFUNCTION(Reliable, Client)
    void ClientChangeScore(const FString& NewScore);

    UFUNCTION(Reliable, Client)
    void ClientChangeName(const FString& NewName);

    UFUNCTION(Reliable, Client)
    void ClientChangeTime(const FString& NewTime);

    UFUNCTION(Reliable, Client)
    void ClientRecieveMessage(const FString& Message, float MessageDuration);
};
