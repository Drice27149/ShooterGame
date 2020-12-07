// Drice


#include "Weapon/Weapon.h"
#include "ShooterGame.h"
#include "Player/ShooterCharacter.h"


// Sets default values
AWeapon::AWeapon()
{
    RootComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComp"));
    RootComp->InitSphereRadius(30.0f);
    RootComponent = RootComp;
    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponGunMesh"));
	Mesh->SetupAttachment(RootComponent);
    bReplicates = true;
    OwnerCharacter = NULL;
}

void AWeapon::BeginPlay(){
    Super::BeginPlay();
    
    RootComp->SetCollisionProfileName(TEXT("OverlapAll"));
    if(GetLocalRole() == ROLE_Authority)
    {
        RootComp->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponOverlapBegin);
        RootComp->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnWeaponOverlapEnd);
    }
}

void AWeapon::SetOwnerCharacter(AShooterCharacter* NewOwnerCharacter)
{
    OwnerCharacter = NewOwnerCharacter;
    SetOwner(NewOwnerCharacter);
    SetInstigator(NewOwnerCharacter);
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

void AWeapon::ServerHandleFiring_Implementation()
{
    HandleFiring(false);
    MultiCastHandleFiring();
}
    
void AWeapon::MultiCastHandleFiring_Implementation()
{
    HandleFiring(true);
}

void AWeapon::OnEquip()
{
    if(OwnerCharacter && OwnerCharacter->GetMesh()){
        //detach from inventory and attach to hand
        DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, UsedSocket);
        //plau equip montage
        if(GetLocalRole() != ROLE_Authority)
        {
            OwnerCharacter->PlayCharacterMontage(EquipMontage_Character);
        }
    }
}

void AWeapon::OnUnEquip()
{
    if(OwnerCharacter && OwnerCharacter->GetMesh()){
        //detach from hand and attach to inventory
        DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, UnUsedSocket);
        //play unequip montange
        if(GetLocalRole() != ROLE_Authority)
        {
            OwnerCharacter->PlayCharacterMontage(UnEquipMontage_Character);
        }
    }
}

void AWeapon::OnDrop()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

FString AWeapon::GetWeaponName()
{
    return WeaponName;
}

bool AWeapon::HasOwner()
{
    return (OwnerCharacter!=NULL);
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

void AWeapon::OnWeaponOverlapBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if(!HasOwner())
    {
        AShooterCharacter* OtherCharacter = Cast<AShooterCharacter>(OtherActor);
        if(OtherCharacter)
        {
            OtherCharacter->SetPickUpWeapon(this);
        }
    }
}

void AWeapon::OnWeaponOverlapEnd(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if(!HasOwner())
    {
        AShooterCharacter* OtherCharacter = Cast<AShooterCharacter>(OtherActor);
        if(OtherCharacter)
        {
            OtherCharacter->SetPickUpWeapon(NULL);
        }
    }
}

void AWeapon::SimulateEquip()
{
    if(OwnerCharacter)
    {
        OwnerCharacter->PlayCharacterMontage(EquipMontage_Character);
    }
}
    
void AWeapon::SimulateUnequip()
{
    if(OwnerCharacter)
    {
        OwnerCharacter->PlayCharacterMontage(UnEquipMontage_Character);
    }
}
    
void AWeapon::SimulateDrop()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}


