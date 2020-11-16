// Drice

#include "Weapon/WeaponGun.h"
#include "ShooterGame.h"
#include "Weapon/ShooterProjectile.h"
#include "Player/ShooterCharacter.h"


// Sets default values
AWeaponGun::AWeaponGun()
{
    CenterComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComp"));
    RootComponent = CenterComp;
    
    FirePointComp = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointComp"));
    FirePointComp->SetupAttachment(RootComponent);
    
    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponGunMesh"));
	Mesh->SetupAttachment(RootComponent);
    
    bReplicates = true;
    
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

void AWeaponGun::HandleFire()
{
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

void AWeaponGun::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AWeaponGun, BulletCount);
}

void AWeaponGun::StartReload(bool bRemoteClient)
{
    if(bRemoteClient && OwnerCharacter && OwnerCharacter->IsLocallyControlled())
    {
        //do nothing because it is a call back on local
    }
    else{
        // local
        if(!bRemoteClient && GetLocalRole() < ROLE_Authority)
        {
            ServerStartReload();
        }
        
        // server
        if(GetLocalRole() == ROLE_Authority)
        {
            BulletCount = MaxBulletCount;
        }
        
        // all
        if(OwnerCharacter)
        {
            OwnerCharacter->PlayCharacterMontage(PawnReloadAnimation);
            PlayWeaponGunMontage(WeaponReloadAnimation);
        } 
    }
}

void AWeaponGun::ServerStartReload_Implementation()
{
    StartReload();
    // call cosmetic function on remote client
    MulticastStartReload();
}

float AWeaponGun::PlayWeaponGunMontage(UAnimMontage* AnimMontage, float InPlayRate)
{
	if (AnimMontage && Mesh && Mesh->AnimScriptInstance)
	{
		return Mesh->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}

	return 0.0f;
}

void AWeaponGun::MulticastStartReload_Implementation()
{
    StartReload(true);
}
