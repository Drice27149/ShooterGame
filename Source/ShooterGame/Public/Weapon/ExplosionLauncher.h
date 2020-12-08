
// Drice

#pragma once

#include "ShooterGame.h"
#include "Weapon/Weapon.h"
#include "ExplosionLauncher.generated.h"

UCLASS()
class SHOOTERGAME_API AExplosionLauncher : public AWeapon
{
	GENERATED_BODY()
	
public:	
	AExplosionLauncher();
    
    //override from Aweapon
    virtual void StartFire() override;
    
    virtual bool CanFire() override;

    virtual int GetWeaponTypeId() override;
    
    virtual void HandleFiring(bool bfromReplication) override;    

protected:
    UPROPERTY(EditDefaultsOnly, Category = "ProjectileClass")
    TSubclassOf<class AExplosionProjectile> ProjectileClass;
    
    /** use to spawn projectile **/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FirePoint")
    class USceneComponent* FirePointComp;
    
    UPROPERTY(EditDefaultsOnly, Category = "FireImpulseScale")
    float FireImpulseScale;

private:
    UPROPERTY(ReplicatedUsing = OnRep_BurstCounter)
    int BurstCounter;
    
    UFUNCTION(Reliable, Server)
    void ServerFireLauncher(FTransform FireTransform, FVector FireVector);
    
    UFUNCTION()
    void OnRep_BurstCounter();
    
    void SimulateLauncherFire();
};
