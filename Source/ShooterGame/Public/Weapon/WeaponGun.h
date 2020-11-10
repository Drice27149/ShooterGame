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
    
    virtual bool CanFire() override;
    
    /** server only, handle fire and create projectile **/
    virtual void HandleFire() override;
    
protected:
    UPROPERTY(Replicated, EditDefaultsOnly, Category = "GunData")
    int BulletCount;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    int CoolDownDuration;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    TSubclassOf<class AShooterProjectile> ProjectileClass = NULL;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    class USphereComponent* CenterComp;
    
    /** use to spawn projectile, value will be set in blueprint **/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GunData")
    class USceneComponent* FirePointComp;
};
