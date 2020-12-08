// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExplosionProjectile.generated.h"

class USphereComponent;
class UParticleSystem;
class URadialForceComponent;
class UPrimitiveComponent;

UCLASS()
class AExplosionProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
    AExplosionProjectile();
    
    virtual void Tick(float DeltaTime) override;
    
protected:
    UPROPERTY(EditDefaultsOnly)
    USphereComponent* CollisionComp;
    
    UPROPERTY(EditDefaultsOnly)
    USphereComponent* DetectionComp;
    
    UPROPERTY(EditDefaultsOnly)
    URadialForceComponent* RadialForceComp;
    
    UPROPERTY(EditDefaultsOnly)
    UParticleSystem* ExplosionEffect;
    
    UPROPERTY(EditDefaultsOnly)
    float ExplodeRadius;
    
    UPROPERTY(EditDefaultsOnly)
    float ExplodeImpulseScale;
    
    UPROPERTY(EditDefaultsOnly)
    float ExplosionSpeed;
    
    UPROPERTY(EditDefaultsOnly)
    float ExplosionDamage;
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnExplode();
    
    UFUNCTION(BlueprintImplementableEvent)
    void FireTest(int signal);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnHitActors(const TArray<AActor*>& HitActors);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnHitOtherActor(AActor* OtherActor);
    
    virtual void BeginPlay() override;
    
    virtual void Destroyed() override;
    
private:
    bool bLaunched;

    void Explode();
    
    UFUNCTION()
    void OnExplosionProjectileHit(UPrimitiveComponent* HitComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
