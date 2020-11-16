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
    
    void StartReload(bool bRemoteClient = false);
    
protected:
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Debug data")
    int BulletCount;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    int MaxBulletCount;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    int CoolDownDuration;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    TSubclassOf<class AShooterProjectile> ProjectileClass = NULL;
    
    UPROPERTY(EditDefaultsOnly, Category = "GunData")
    class USphereComponent* CenterComp;
    
    /** use to spawn projectile **/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GunData")
    class USceneComponent* FirePointComp;
    
    UPROPERTY(EditDefaultsOnly, Category = "Shooter Mesh")
    class USkeletalMeshComponent* Mesh;
    
    //Animation
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    class UAnimMontage* WeaponFireAnimation;
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    class UAnimMontage* WeaponReloadAnimation;
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    class UAnimMontage* PawnFireAnimation;
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    class UAnimMontage* PawnReloadAnimation;
    
    UFUNCTION(BlueprintImplementableEvent)
    void FireTest(int seed);
    
private:
    UFUNCTION(Reliable, Server)
    void ServerStartReload();

    // cosmetice function on remote client
    UFUNCTION(Reliable, NetMulticast)
    void MulticastStartReload();
    
    float PlayWeaponGunMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.0f);
};
