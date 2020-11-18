// Drice

#pragma once

#include "ShooterGame.h"
#include "Weapon/Weapon.h"
#include "WeaponGun.generated.h"

UCLASS()
class SHOOTERGAME_API AWeaponGun : public AWeapon
{
	GENERATED_BODY()
	
public:	
	AWeaponGun();
    
    void StartReload(bool bRemoteClient = false);
    
    //override from Aweapon
    virtual bool CanFire() override;

    virtual int GetWeaponTypeId() override;
    
    virtual void HandleFiring(bool bfromReplication) override;    

protected:
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "BulletCount")
    int BulletCount;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    int MaxBulletCount;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    int CoolDownDuration;
    
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
    
private:
    UFUNCTION(Reliable, Server)
    void ServerStartReload();

    // cosmetice function on remote client
    UFUNCTION(Reliable, NetMulticast)
    void MulticastStartReload();
};
