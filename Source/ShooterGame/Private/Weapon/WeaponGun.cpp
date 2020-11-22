// Drice

#include "Weapon/WeaponGun.h"
#include "ShooterGame.h"
#include "Weapon/ShooterProjectile.h"
#include "Player/ShooterCharacter.h"


// Sets default values
AWeaponGun::AWeaponGun()
{
    FirePointComp = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointComp"));
    FirePointComp->SetupAttachment(RootComponent);
    
    bReplicates = true;
    SetReplicateMovement(true);
    
    BulletCount = MaxBulletCount;
} 

bool AWeaponGun::CanFire()
{
    if(BulletCount>0)
    {
        return true;
    }
    else{
        return false;
    }
}

int AWeaponGun::GetWeaponTypeId()
{
    return 1;
}

void AWeaponGun::HandleFiring(bool bfromReplication)
{
    //server
    if(GetLocalRole() == ROLE_Authority && !bfromReplication)
    {
        //spawn and fire projectile
        BulletCount--;
        if(ProjectileClass != NULL)
        {
            FRotator FireRotation = FirePointComp->GetComponentRotation();
            FVector FireLocation = FirePointComp->GetComponentLocation();
            FTransform SpawnTransform(FireRotation, FireLocation);
            AShooterProjectile* SpawnedProjectile = Cast<AShooterProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTransform));
            if(SpawnedProjectile != NULL)
            {
                SpawnedProjectile->OwnerCharacter = OwnerCharacter;
                UGameplayStatics::FinishSpawningActor(SpawnedProjectile, SpawnTransform);
            }
        }
    }
    //client
    if(GetLocalRole() != ROLE_Authority)
    {
        if(OwnerCharacter){
            OwnerCharacter->PlayCharacterMontage(FireMontage_Character);
            PlayWeaponMontage(FireMontage_Weapon);
        }
    }
}


void AWeaponGun::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AWeaponGun, BulletCount);
}

void AWeaponGun::StartReload()
{
    ServerStartReload();
}

void AWeaponGun::HandleReload(bool bfromReplication)
{
    //server
    if(GetLocalRole() == ROLE_Authority && !bfromReplication)
    {
        BulletCount = MaxBulletCount;
    }
    //client
    if(GetLocalRole() != ROLE_Authority)
    {
        if(OwnerCharacter)
        {
            OwnerCharacter->PlayCharacterMontage(PawnReloadAnimation);
            PlayWeaponMontage(WeaponReloadAnimation);
        } 
    }
}

void AWeaponGun::ServerStartReload_Implementation()
{
    HandleReload(false);
    MulticastStartReload();
}

void AWeaponGun::MulticastStartReload_Implementation()
{
    HandleReload(true);
}
