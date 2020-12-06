// Drice

#pragma once

#include "ShooterGame.h"
#include "Weapon/Weapon.h"
#include "WeaponGun.generated.h"

USTRUCT()
struct FHitNotify
{
	GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FVector HitLocation;
    
    UPROPERTY();
    int HitCounter;

	FTakeHitInfo()
    {
        HitCounter = 0;
    }
};

UCLASS()
class SHOOTERGAME_API AWeaponGun : public AWeapon
{
	GENERATED_BODY()
	
public:	
	AWeaponGun();
    
    //override from Aweapon
    virtual bool CanFire() override;
    
    virtual void StartFire() override;

    virtual int GetWeaponTypeId() override;
    
    virtual void HandleFiring(bool bfromReplication) override;    
    
    void StartReload();

protected:
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "BulletCount")
    int BulletCount;
    
    UPROPERTY(EditDefaultsOnly)
    class UParticleSystem* HitEffect;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    int MaxBulletCount;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    int CoolDownDuration;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    float FireRange;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    float WeaponGunDamage;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    TSubclassOf<class AShooterProjectile> ProjectileClass;
    
    /** use to spawn projectile **/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GunData")
    class USceneComponent* FirePointComp;
    
    //Animation
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    class UAnimMontage* WeaponReloadAnimation;
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    class UAnimMontage* PawnReloadAnimation;
    
    void SimulateWeaponHit(FVector HitLocation);
    
private:
    UPROPERTY(ReplicatedUsing = OnRep_BurstCounter);
    int BurstCounter;

    UPROPERTY(ReplicatedUsing = OnRep_HitNotify);
    FHitNotify HitNotify;

    UFUNCTION()
    void OnRep_BurstCounter();
    
    UFUNCTION()
    void OnRep_HitNotify();

    UFUNCTION(Reliable, Server)
    void ServerNotifyHit(struct FHitResult HitResult);

    UFUNCTION(Reliable, Server)
    void ServerStartReload();
    
    UFUNCTION(Reliable, NetMulticast)
    void MulticastStartReload();
    
    UFUNCTION(Reliable, Server)
    void ServerFireWeapon();
    
    void HandleReload(bool bfromReplication);

    void SimulateFireWeapon();
};
