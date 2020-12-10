// Drice


#include "Weapon/Weapon.h"
#include "ShooterGame.h"
#include "Player/ShooterCharacter.h"

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
    
    // set up collision for pick up detection
    RootComp->SetCollisionProfileName(TEXT("OverlapAll"));
    if(GetLocalRole() == ROLE_Authority)
    {
        RootComp->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponOverlapBegin);
        RootComp->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnWeaponOverlapEnd);
    }
    
    AmmoCount = MaxAmmo;
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
    
    DOREPLIFETIME_CONDITION(AWeapon, AmmoCount, COND_OwnerOnly);
    // replicated breload to call cosmetic on remote client
    DOREPLIFETIME_CONDITION(AWeapon, bReload, COND_SkipOwner);
}

int32 AWeapon::GetAmmoCount()
{
    return AmmoCount;
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
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, UnUsedSocket);
    /*
    if(OwnerCharacter)
    {
        OwnerCharacter->PlayCharacterMontage(UnEquipMontage_Character);
    }*/
}
    
void AWeapon::SimulateDrop()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

USkeletalMeshComponent* AWeapon::GetMesh()
{
    return Mesh;
}

void AWeapon::Reload()
{
    SimulateReload();
    ServerReload();
}

void AWeapon::SimulateReload()
{
    if(OwnerCharacter)
    {
        OwnerCharacter->PlayCharacterMontage(ReloadMontage_Character);
        PlayWeaponMontage(ReloadMontage_Weapon);
    }
}

void AWeapon::ServerReload_Implementation()
{
    AmmoCount = MaxAmmo;
    // call simulation on remote client
    bReload = !bReload;
}

void AWeapon::OnRep_bReload()
{
    SimulateReload();
}

void AWeapon::OnRep_AmmoCount()
{
    // notify player ammo change to update HUD display
    if(OwnerCharacter)
    {
        OwnerCharacter->NotifyAmmoCountChange(AmmoCount);
    }
}

