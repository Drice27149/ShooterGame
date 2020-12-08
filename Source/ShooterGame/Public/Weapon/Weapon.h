// Drice

#pragma once

#include "CoreMinimal.h"
#include "Weapon.generated.h"

class AShooterCharacter;
class UAnimMontage;
class FName;
class UPrimitiveComponent;

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
    
    void SimulateEquip();
    
    void SimulateUnequip();
    
    void SimulateDrop();
    
    void SetOwnerCharacter(AShooterCharacter* NewOwnerCharacter);
    
    bool HasOwner();
    
    FString GetWeaponName();
    
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

    float PlayWeaponMontage(UAnimMontage* AnimMontage, float InPlayRate = 1.0f);
    
    UFUNCTION(BlueprintImplementableEvent)
    void FireTest(int signal);

private:    
    UFUNCTION(Reliable, Server)
    void ServerHandleFiring();
    
    UFUNCTION(Reliable, NetMulticast)
    void MultiCastHandleFiring();
    
    UFUNCTION()
    void OnWeaponOverlapBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const struct FHitResult& SweepResult);

    UFUNCTION()
    void OnWeaponOverlapEnd(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
