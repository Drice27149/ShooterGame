// Fill out your copyright notice in the Description page of Project Settings.


#include "Npc/ScoreTarget.h"
#include "ShooterGame.h"
#include "Weapon/ShooterProjectile.h"
#include "Player/ShooterCharacter.h"
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
        AShooterProjectile* DamageProjectile = Cast<AShooterProjectile>(DamageCauser);
        if(DamageProjectile != NULL)
        {
            AShooterCharacter* DamageCharacter = DamageProjectile->OwnerCharacter;
            if(DamageCharacter != NULL)
            {
                AShooterGameMode* MyGameMode = Cast<AShooterGameMode>(UGameplayStatics::GetGameMode(this));
                FVector HitLocation = DamageCauser->GetActorLocation();
                
                MyGameMode->HandleHitScore(DamageCharacter,ScoreRadius,CenterLocation,HitLocation);
            }
        }
	}
	return 0;
}



