// Drice

#include "Weapon/WeaponSword.h"
#include "ShooterGame.h"
#include "Player/ShooterCharacter.h"

AWeaponSword::AWeaponSword(){
    bReplicates = true;
}

void AWeaponSword::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AWeaponSword, bCanCombo);
    DOREPLIFETIME(AWeaponSword, NextCombo);
}

bool AWeaponSword::CanFire()
{
    return bCanCombo;
}

int AWeaponSword::GetWeaponTypeId()
{
    return 2;
}
    
void AWeaponSword::HandleFiring(bool bfromReplication)
{
    //server
    if(GetLocalRole() == ROLE_Authority && !bfromReplication)
    {
        if(OwnerCharacter)
        {
            bCanCombo = false;
            GetWorldTimerManager().ClearTimer(ComboTimer);
            //combo can be canceled into next move after half
            GetWorldTimerManager().SetTimer(ComboTimer, this, &AWeaponSword::EnableCombo, ComboMontageLength[NextCombo]*0.5, false);
            //play montage on server because it is related to gameplay
            OwnerCharacter->PlayCharacterMontage(ComboMontage_Character[NextCombo]);
        }
    }
    //client
    if(GetLocalRole() != ROLE_Authority)
    {
        if(OwnerCharacter)
        {
            OwnerCharacter->PlayCharacterMontage(ComboMontage_Character[NextCombo]);
            PlayWeaponMontage(ComboMontage_Weapon[NextCombo]);
        }
    }
}

void AWeaponSword::EnableCombo()
{
    GetWorldTimerManager().SetTimer(ComboTimer, this, &AWeaponSword::ResetCombo, ComboMontageLength[NextCombo]*0.5, false);
    //transition into next combo is enabled
    NextCombo = (NextCombo+1)%MaxCombo;
    bCanCombo = true;
}

void AWeaponSword::ResetCombo()
{
    NextCombo = 0;
    bCanCombo = true;
}
