// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ExplosionProjectile.h"
#include "ShooterGame.h"
#include "Player/ShooterCharacter.h"

AExplosionProjectile::AExplosionProjectile()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    
    bLaunched = false;
    
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComp;
    
    DetectionComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionComponent"));
    DetectionComp->SetupAttachment(RootComponent);
    
    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
    RadialForceComp->SetupAttachment(RootComponent);
}

void AExplosionProjectile::BeginPlay()
{
    Super::BeginPlay();
    
    if(GetLocalRole() == ROLE_Authority && CollisionComp)
    {
        CollisionComp->OnComponentHit.AddDynamic(this, &AExplosionProjectile::OnExplosionProjectileHit);
    }
}

void AExplosionProjectile::OnExplosionProjectileHit(UPrimitiveComponent* HitComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    OnHitOtherActor(OtherActor);
    if(GetLocalRole() == ROLE_Authority)
    {
        AShooterCharacter* OtherCharacter = Cast<AShooterCharacter>(OtherActor);
        if(OtherCharacter)
        {
            Explode();
        }
    }
}

void AExplosionProjectile::Explode()
{
    if(RadialForceComp)
    {
        RadialForceComp->FireImpulse();
    }
    
    TArray<AActor*> HitActors;
    DetectionComp->GetOverlappingActors(HitActors,AActor::StaticClass());
    
    for(AActor* HitActor: HitActors)
    {
        AShooterCharacter* HitCharacter = Cast<AShooterCharacter>(HitActor);
        if(HitCharacter)
        {
            FVector HitVector = HitCharacter->GetActorLocation() - GetActorLocation();
            FVector HitImpulse = HitVector * ExplodeImpulseScale;
            HitCharacter->PlayHit(this, EHitType::HeavyHit, 0.0f, HitVector, HitImpulse, FName(TEXT("")));
        }
    }

    Destroy();
}


void AExplosionProjectile::Destroyed()
{
    if(ExplosionEffect != NULL)
    {
        FVector spawnLocation = GetActorLocation();
        UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
    }
}

void AExplosionProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if(GetLocalRole() == ROLE_Authority)
    {
        float CurrentSpeed = GetVelocity().Size();
        if(bLaunched && CurrentSpeed < ExplosionSpeed)
        {
            Explode();
        }
        else if(CurrentSpeed > ExplosionSpeed)
        {
            bLaunched = true;
        }
    }
}

