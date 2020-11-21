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

//will be overriden by subclass
bool AWeapon::CanFire()
{
    return true;
}

//will be overriden by subclass
int AWeapon::GetWeaponTypeId()
{
    return -1;
}

//will be overriden by subclass
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

FString AWeapon::GetWeaponName()
{
    return WeaponName;
}

bool AWeapon::HasOwner()
{
    return (OwnerCharacter!=NULL);
}
