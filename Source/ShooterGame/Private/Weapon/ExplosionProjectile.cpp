// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ExplosionProjectile.h"
#include "ShooterGame.h"
#include "Player/ShooterCharacter.h"

AExplosionProjectile::AExplosionProjectile()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComp;
    
    DetectionComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionComponent"));
    DetectionComp->SetupAttachment(RootComponent);
}

void AExplosionProjectile::BeginPlay()
{
    Super::BeginPlay();
    
    if(GetLocalRole() == ROLE_Authority)
    {
        CollisionComp->OnComponentHit.AddDynamic(this, &AExplosionProjectile::OnExplosionProjectileHit);
    }
}

void AExplosionProjectile::OnExplosionProjectileHit(UPrimitiveComponent* HitComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    FireTest(0);
    if(GetLocalRole() == ROLE_Authority)
    {
        FireTest(1);
        AShooterCharacter* OtherCharacter = Cast<AShooterCharacter>(OtherActor);
        if(OtherCharacter)
        {
            FireTest(2);
            Explode();
        }
    }
}

void AExplosionProjectile::Explode()
{
    FireTest(3);
    //create radial force in here, for non character actor
    OnExplode();
    
    TArray<AActor*> HitActors;
    DetectionComp->GetOverlappingActors(HitActors,AShooterCharacter::StaticClass());

    FireTest(4);
    //debug
    OnHitActors(HitActors);
    
    FireTest(5);
    for(AActor* HitActor: HitActors)
    {
        AShooterCharacter* HitCharacter = Cast<AShooterCharacter>(HitActor);
        if(HitCharacter)
        {
            FVector HitVector = HitCharacter->GetActorLocation() - GetActorLocation();
            FVector HitImpulse = HitVector * ExplodeImpulseScale;
            HitCharacter->PlayHit(this, EHitType::HeavyHit, 0.0f, HitVector, HitImpulse, FString(TEXT("")));
        }
    }

    FireTest(6);
    Destroy();
}


void AExplosionProjectile::Destroyed()
{
    FireTest(7);
    if(ExplosionEffect != NULL)
    {
        FireTest(8);
        FVector spawnLocation = GetActorLocation();
        UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
    }
}

void AExplosionProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

