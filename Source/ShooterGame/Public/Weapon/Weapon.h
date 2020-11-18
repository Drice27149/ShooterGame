// Drice

#pragma once

#include "CoreMinimal.h"
#include "Weapon.generated.h"

class AShooterCharacter;
class UAnimMontage;
class FName;

UCLASS()
class SHOOTERGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
    
    void StartFire();
    
    void StartEquip();
    
    void StartUnEquip();
    
    /** server only, becuase OwnerCharacter is a replicated property **/
    void SetOwnerCharacter(AShooterCharacter* NewOwnerCharacter);

    virtual bool CanFire();

    virtual int GetWeaponTypeId();
    
protected:
    /** replicate for calling cosmetic function on owner character **/
    UPROPERTY(Replicated)
    AShooterCharacter* OwnerCharacter;

    UPROPERTY(EditDefaultsOnly, Category = "Mesh")
    class USkeletalMeshComponent* Mesh;

    UPROPERTY(EditDefaultsOnly, Category = "AttachSocket")
    FName UsedSocket;

    UPROPERTY(EditDefaultsOnly, Category = "AttachSocket")
    FName UnUsedSocket;

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* EquipMontage_Character;
    
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* UnEquipMontage_Character;
    
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* FireMontage_Character;
    
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* FireMontage_Weapon;

    float PlayWeaponMontage(UAnimMontage* AnimMontage, float InPlayRate = 1.0f);

private:    
    class USphereComponent* RootComp;
    
    UFUNCTION(Reliable, Server)
    void ServerHandleFiring();
    
    UFUNCTION(Reliable, NetMulticast)
    void MultiCastHandleFiring();
    
    UFUNCTION(Reliable, Server)
    void ServerHandleEquip();
    
    UFUNCTION(Reliable, NetMulticast)
    void MultiCastHandleEquip();
    
    UFUNCTION(Reliable, Server)
    void ServerHandleUnEquip();
    
    UFUNCTION(Reliable, NetMulticast)
    void MultiCastHandleUnEquip();
    
    virtual void HandleFiring(bool bfromReplication);
    
    void HandleEquip(bool bfromReplication);
    
    void HandleUnEquip(bool bfromReplication);
};
