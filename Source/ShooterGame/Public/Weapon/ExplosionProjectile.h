// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExplosionProjectile.generated.h"

class USphereComponent;
class UParticleSystem;

UCLASS()
class AExplosionProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
    AExplosionProjectile();
    
    virtual void Tick(float DeltaTime) override;
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USphereComponent* CollisionComp;
    
    UPROPERTY(EditDefaultsOnly)
    USphereComponent* DetectionComp;
    
    UPROPERTY(EditDefaultsOnly)
    UParticleSystem* ExplosionEffect;
    
    UPROPERTY(EditDefaultsOnly)
    float ExplodeRadius;
    
    UPROPERTY(EditDefaultsOnly)
    float ExplodeImpulseScale;
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnExplode();
    
    UFUNCTION(BlueprintImplementableEvent)
    void FireTest(int signal);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnHitActors(const TArray<AActor*>& HitActors);
    
    virtual void BeginPlay() override;
    
    virtual void Destroyed() override;
    
private:
    void Explode();
    
    UFUNCTION()
    void OnExplosionProjectileHit(UPrimitiveComponent* HitComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
