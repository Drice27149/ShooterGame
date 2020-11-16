// Drice


#include "Weapon/Weapon.h"
#include "ShooterGame.h"


// Sets default values
AWeapon::AWeapon()
{
    
    bReplicates = true;
}

bool AWeapon::CanFire()
{
    return true;
}

void AWeapon::HandleFire()
{
}


void AWeapon::SetOwnerCharacter(AShooterCharacter* NewOwnerCharacter)
{
    OwnerCharacter = NewOwnerCharacter;
}

void AWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AWeapon, OwnerCharacter);
}
