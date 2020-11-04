// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "ScoreTarget.generated.h"

UCLASS()
class AScoreTarget : public AActor
{
	GENERATED_BODY()
	
public:	

	AScoreTarget();

	class USphereComponent* SphereComponent;

	/** will be set in blueprint class **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScoreMesh)
	class UStaticMeshComponent* StaticMesh;

	/** used to calculate hit score, will be set in blueprint class **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Score)
	FVector CenterLocation;

	/** used to calculate hit score, will be set in blueprint class **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Score)
	float ScoreRadius;

	/** used to triger hitscore event **/
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
//score center location & score radio set and change by blueprint 

