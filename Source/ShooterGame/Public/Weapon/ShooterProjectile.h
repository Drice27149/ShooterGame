// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterProjectile.generated.h"

class UPrimitiveComponent;

UCLASS()
class AShooterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
    AShooterProjectile();
    
	// Sphere component used to test collision.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereComponent;

	// Movement component for handling projectile movement.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	// Particle used when the projectile impacts against another object and explodes.
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystem* ExplosionEffect;

	//The damage type and damage that will be done by this projectile
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<class UDamageType> DamageType;

	//The damage dealt by this projectile.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float ProjectileDamage;
    
    UPROPERTY(EditDefaultsOnly, Category = "ImpulseScale")
    float ImpulseScale;

	class AShooterCharacter* OwnerCharacter;

    UFUNCTION(BlueprintImplementableEvent, Category = "test")
    void FireTest(int signal);
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UFUNCTION(Category = "Projectile")
	void OnProjectileImpact(UPrimitiveComponent* HitComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
