// Drice

#pragma once

#include "CoreMinimal.h"
#include "Weapon.generated.h"

class AShooterCharacter;
class UAnimMontage;
class FName;
class UPrimitiveComponent;
class UCameraShake;

UCLASS()
class SHOOTERGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
    
    virtual void BeginPlay() override;
    
    virtual void StartFire();
    
    void OnEquip();
    
    void OnUnEquip();
    
    void OnDrop();
    
    void Reload();
    
    void SimulateEquip();
    
    void SimulateUnequip();
    
    void SimulateDrop();
    
    void SetOwnerCharacter(AShooterCharacter* NewOwnerCharacter);
    
    bool HasOwner();
    
    UFUNCTION(BlueprintCallable)
    FString GetWeaponName();
    
    UFUNCTION(BlueprintCallable)
    int32 GetAmmoCount();
    
    class USkeletalMeshComponent* GetMesh();
    
    /** will be overridden by subclass **/
    virtual bool CanFire();

    virtual int GetWeaponTypeId();
    
    virtual void HandleFiring(bool bfromReplication);
    
    UPROPERTY(EditDefaultsOnly, Category = "AttachSocket")
    FName UsedSocket;

    UPROPERTY(EditDefaultsOnly, Category = "AttachSocket")
    FName UnUsedSocket;
    
    UPROPERTY(EditDefaultsOnly, Category = "Camera")
    FName CameraSocket;
    
protected:
    /** replicate for calling cosmetic function on owner character **/
    UPROPERTY(Replicated)
    AShooterCharacter* OwnerCharacter;
    
    UPROPERTY(ReplicatedUsing = OnRep_bReload)
    bool bReload;
    
    UPROPERTY(ReplicatedUsing = OnRep_AmmoCount)
    int32 AmmoCount;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 MaxAmmo;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf <UCameraShake> FireCameraShake;

    UPROPERTY(EditDefaultsOnly, Category = "CollisionComp")
    class USphereComponent* RootComp;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh")
    class USkeletalMeshComponent* Mesh;
    
    UPROPERTY(EditDefaultsOnly, Category = "WeaponName")
    FString WeaponName;

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* EquipMontage_Character;
    
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* UnEquipMontage_Character;
    
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* FireMontage_Character;
    
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* FireMontage_Weapon;
    
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* ReloadMontage_Character;
    
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* ReloadMontage_Weapon;

    float PlayWeaponMontage(UAnimMontage* AnimMontage, float InPlayRate = 1.0f);
    
    UFUNCTION()
    void OnRep_AmmoCount();
    
    UFUNCTION(BlueprintImplementableEvent)
    void FireTest(int signal);
    
private:    
    UFUNCTION(Reliable, Server)
    void ServerReload();

    UFUNCTION(Reliable, Server)
    void ServerHandleFiring();
    
    UFUNCTION(Reliable, NetMulticast)
    void MultiCastHandleFiring();
    
    UFUNCTION()
    void OnRep_bReload();
    
    UFUNCTION()
    void OnWeaponOverlapBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const struct FHitResult& SweepResult);

    UFUNCTION()
    void OnWeaponOverlapEnd(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void SimulateReload();
};
