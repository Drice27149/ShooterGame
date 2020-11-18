// Drice


#include "Weapon/Weapon.h"
#include "ShooterGame.h"
#include "Player/ShooterCharacter.h"


// Sets default values
AWeapon::AWeapon()
{
    RootComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComp"));
    RootComponent = RootComp;
    
    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponGunMesh"));
	Mesh->SetupAttachment(RootComponent);
    
    bReplicates = true;
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

float AWeapon::PlayWeaponMontage(UAnimMontage* AnimMontage, float InPlayRate)
{
	if (AnimMontage && Mesh && Mesh->AnimScriptInstance)
	{
		return Mesh->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}

	return 0.0f;
}

void AWeapon::StartFire()
{
    ServerHandleFiring();
}
    
void AWeapon::StartEquip()
{
    ServerHandleEquip();
}
    
void AWeapon::StartUnEquip()
{
    ServerHandleUnEquip();
}

void AWeapon::ServerHandleFiring_Implementation()
{
    HandleFiring(false);
    MultiCastHandleFiring();
}
    
void AWeapon::MultiCastHandleFiring_Implementation()
{
    HandleFiring(true);
}
    
void AWeapon::ServerHandleEquip_Implementation()
{
    HandleEquip(false);
    MultiCastHandleEquip();
}

void AWeapon::MultiCastHandleEquip_Implementation()
{
    HandleEquip(true);
}
    
void AWeapon::ServerHandleUnEquip_Implementation()
{
    HandleUnEquip(false);
    MultiCastHandleUnEquip();
}
    
void AWeapon::MultiCastHandleUnEquip_Implementation()
{
    HandleUnEquip(true);
}

void AWeapon::HandleEquip(bool bfromReplication)
{
    //server
    if(GetLocalRole() == ROLE_Authority && !bfromReplication)
    {
        if(OwnerCharacter && OwnerCharacter->GetMesh()){
            //detach from inventory and attach to hand
            DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
            AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, UsedSocket);
        }
    }
    
    //client
    if(GetLocalRole() != ROLE_Authority)
    {
        if(OwnerCharacter && OwnerCharacter->GetMesh()){
            //detach from inventory and attach to hand
            DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
            AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, UsedSocket);
            //play equip montage on character
            OwnerCharacter->PlayCharacterMontage(EquipMontage_Character);
        }
    }
}

void AWeapon::HandleUnEquip(bool bfromReplication)
{
    //server
    if(GetLocalRole() == ROLE_Authority && !bfromReplication)
    {
        //detach from hand and attach to inventory
        DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
        AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, UnUsedSocket);
    }
    
    //client
    if(GetLocalRole() != ROLE_Authority)
    {
        //detach from hand and attach to inventory
        DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
        AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, UnUsedSocket);
        //play unequip montange
        OwnerCharacter->PlayCharacterMontage(UnEquipMontage_Character);
    }
}

bool AWeapon::CanFire()
{
    return true;
}

int AWeapon::GetWeaponTypeId()
{
    return -1;
}

void AWeapon::HandleFiring(bool bfromReplication)
{
    
}
