// Drice

#pragma once

#include "ShooterGame.h"
#include "Weapon/Weapon.h"
#include "WeaponSword.generated.h"

class UAnimMontage;
class AShooterCharacter;

UCLASS()
class SHOOTERGAME_API AWeaponSword : public AWeapon
{
	GENERATED_BODY()
	
public:	
	AWeaponSword();
    //override from Aweapon
    virtual bool CanFire() override;

    virtual int GetWeaponTypeId() override;
    
    virtual void HandleFiring(bool bfromReplication) override;   
     
protected:
    UPROPERTY(EditDefaultsOnly, Category = "Sword data")
    int MaxCombo;

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* ComboMontage_Character[5];

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* ComboMontage_Weapon[5];

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    float ComboMontageLength[5];

private:
    /** replicated to know whether fire or not **/
    UPROPERTY(Replicated)
    bool bCanCombo;

    /** replicated to simulate the exact combo on client **/
    UPROPERTY(Replicated)
    int NextCombo;

    struct FTimerHandle ComboTimer;

    void ResetCombo();
    
    void EnableCombo();
};
