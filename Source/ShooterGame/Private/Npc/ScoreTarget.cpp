// Fill out your copyright notice in the Description page of Project Settings.


#include "Npc/ScoreTarget.h"
#include "Weapon/ShooterProjectile.h"
#include "Player/ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameMode.h" 

// Sets default values
AScoreTarget::AScoreTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(0.0f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = SphereComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void AScoreTarget::BeginPlay()
{
	Super::BeginPlay();
}

void AScoreTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AScoreTarget::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser){
	if (GetLocalRole() == ROLE_Authority)
	{
		FVector HitLocation = DamageCauser->GetActorLocation();
		AShooterGameMode* MyGameMode = Cast<AShooterGameMode>(UGameplayStatics::GetGameMode(this));
		AShooterCharacter* ShooterPawn = Cast<AShooterCharacter>(DamageCauser->GetOwner());
		if(MyGameMode != NULL && ShooterPawn != NULL)
		{
			MyGameMode->HandleHitScore(ShooterPawn,ScoreRadius,CenterLocation,HitLocation);
		}
	}
	return 0;
}



