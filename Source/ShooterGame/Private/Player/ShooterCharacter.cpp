// Drice

#include "Player/ShooterCharacter.h"
#include "ShooterGame.h"
#include "Weapon/Weapon.h"
#include "Weapon/WeaponGun.h"
#include "UI/ShooterHUD.h"
#include "Player/MyCharacterMovementComponent.h"
#include "Player/ShooterPlayerController.h"

AShooterCharacter::AShooterCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

void AShooterCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    CurrentWeapon = NULL;
    Health = MaxHealth;
}

bool AShooterCharacter::IsRunning()
{
    return bRunning;
}

FRotator AShooterCharacter::GetAimOffset()
{
    const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

float AShooterCharacter::GetWalkSpeedMultiplier()
{
    return WalkSpeedMultiplier;
}
    
float AShooterCharacter::GetRunSpeedMultiplier()
{
    return RunSpeedMultiplier;
}
    
void AShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterCharacter::OnJumpStart);
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::OnMoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &AShooterCharacter::OnTurn);
    PlayerInputComponent->BindAxis("LookUp", this, &AShooterCharacter::OnLookUp);
    
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AShooterCharacter::OnStartFire);
}

void AShooterCharacter::OnMoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::OnMoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::OnStartFire()
{
    if(CurrentWeapon != NULL && CurrentWeapon->CanFire())
    {
        CurrentWeapon->StartFire();
    }
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AShooterCharacter, bRunning);
    DOREPLIFETIME(AShooterCharacter, LastHitInfo);
    DOREPLIFETIME(AShooterCharacter, Health);
    
    DOREPLIFETIME_CONDITION(AShooterCharacter, PickUpWeapon, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(AShooterCharacter, bBusy, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(AShooterCharacter, Inventory, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(AShooterCharacter, CurrentWeaponIndex, COND_OwnerOnly);
    // owner alreay known change of weapon and simulation was done beforehead
    DOREPLIFETIME_CONDITION(AShooterCharacter, CurrentWeapon, COND_SkipOwner);
}

void AShooterCharacter::OnCrouchStart()
{
    Crouch();
}
    
void AShooterCharacter::OnCrouchEnd()
{
    UnCrouch();
}
    
void AShooterCharacter::OnJumpStart()
{
    Jump();
}

void AShooterCharacter::OnRunStart()
{
    bRunning = true;
    ServerSetRunning(true);
}
    
void AShooterCharacter::OnRunEnd()
{
    bRunning = false;
    ServerSetRunning(false);
}
    
void AShooterCharacter::OnTurn(float Value)
{
    if ( (Controller != NULL) && (Value != 0.0f) )
    {
        AddControllerYawInput(Value);
    }
}
    
void AShooterCharacter::OnLookUp(float Value)
{
    if ( (Controller != NULL) && (Value != 0.0f) )
    {
        AddControllerPitchInput(Value);
    }
}

void AShooterCharacter::OnPickUp()
{
    if(PickUpWeapon && !PickUpWeapon->HasOwner())
    {
        // add weapon to inventory
        ServerPickUpWeapon();
        
        SwitchToNewWeapon(PickUpWeapon);
    }
}

void AShooterCharacter::OnSwitchWeapon(int direction)
{
    if(Inventory.Num())
    {
        int NextWeaponIndex = (CurrentWeaponIndex + direction)%Inventory.Num();
        if(NextWeaponIndex < 0)
        {
            NextWeaponIndex += Inventory.Num();
        }
        
        AWeapon* NextWeapon = Inventory[NextWeaponIndex];
        SwitchToNewWeapon(NextWeapon);
    }
}

void AShooterCharacter::OnUnEquip()
{
    if(CurrentWeapon)
    {
        LastEquipWeapon = CurrentWeapon;
        LastEquipWeapon->SimulateUnequip();
        CurrentWeapon = NULL;
        
        ServerSetCurrentWeapon(NULL);
    }
}

void AShooterCharacter::OnDrop()
{
    if(CurrentWeapon)
    {
        LastEquipWeapon = CurrentWeapon;
        LastEquipWeapon->SimulateDrop();
        CurrentWeapon = NULL;
        
        ServerSetCurrentWeapon(NULL, true);
    }
}

void AShooterCharacter::SwitchToNewWeapon(AWeapon* NewWeapon)
{
    // set up local because CurrentWeapon won't be replicated to local
    if(CurrentWeapon)
    {
        LastEquipWeapon = CurrentWeapon;
        LastEquipWeapon->SimulateUnequip();
    }
    CurrentWeapon = NewWeapon;
    CurrentWeapon->SimulateEquip();
    
    // call rpc to replicate weapon on server and remote client
    ServerSetCurrentWeapon(NewWeapon);
}

void AShooterCharacter::ServerSetCurrentWeapon_Implementation(AWeapon* NewWeapon, bool bDropLastWeapon)
{
    AWeapon* LastWeapon = CurrentWeapon;
    
    if(bDropLastWeapon)
    {
        if(LastWeapon)
        {
            LastWeapon->SetOwnerCharacter(NULL);
            Inventory.RemoveAt(CurrentWeaponIndex);
        }
    }
    
    //if new weapon is picked up, need to set it's owner
    if(!bDropLastWeapon && NewWeapon){
        NewWeapon->SetOwnerCharacter(this);
        NewWeapon->SetOwner(this);
    }
    
    CurrentWeapon = NewWeapon;
    
    OnRep_CurrentWeapon(LastWeapon);
}

void AShooterCharacter::ServerPickUpWeapon_Implementation()
{
    Inventory.Add(PickUpWeapon);
    CurrentWeaponIndex = Inventory.Num()-1;
}
    
void AShooterCharacter::ServerSetRunning_Implementation(bool Value)
{
    bRunning = Value;
}

float AShooterCharacter::PlayCharacterMontage(UAnimMontage* AnimMontage, float InPlayRate)
{
	if (AnimMontage && GetMesh() && GetMesh()->AnimScriptInstance)
	{
		return GetMesh()->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}

	return 0.0f;
}

void AShooterCharacter::OnStartReload()
{
    AWeaponGun* CurrentWeaponGun = Cast<AWeaponGun>(CurrentWeapon);
    if(CurrentWeaponGun)
    {
        CurrentWeaponGun->StartReload();
    }
}

int AShooterCharacter::GetCurrentWeaponType()
{
    if(CurrentWeapon)
    {
        return CurrentWeapon->GetWeaponTypeId();
    }
    else{
        return 0;
    }
}

void AShooterCharacter::OnRep_PickUpWeapon()
{
    //only display pick up option on character's owner client
    if(IsLocallyControlled())
    {
        AShooterPlayerController* MyShooterPlayerController = Cast<AShooterPlayerController>(GetController());
        AShooterHUD* MyShooterHUD = MyShooterPlayerController?(Cast<AShooterHUD>(MyShooterPlayerController->GetHUD())):NULL;
        if(MyShooterHUD){
            if(PickUpWeapon)
            {
                MyShooterHUD->OnPickUpWeaponChange(PickUpWeapon->GetWeaponName());
            }
            else
            {
                FString NoPickUp = FString::Printf(TEXT(""));
                MyShooterHUD->OnPickUpWeaponChange(NoPickUp);
            }
        }
    }
}

void AShooterCharacter::SetPickUpWeapon(AWeapon* NewPickUpWeapon)
{
    PickUpWeapon = NewPickUpWeapon;
}

void AShooterCharacter::OnRep_CurrentWeapon(AWeapon* LastWeapon)
{
    // save for detach in animation notify
    LastEquipWeapon = LastWeapon;
    
    if(LastWeapon){
        //Unequip last weapon
        if(LastWeapon->HasOwner())
        {
            LastWeapon->SimulateUnequip();
        }
        //drop last weapon
        else
        {
            LastWeapon->SimulateDrop();
        }
    }
    
    if(CurrentWeapon)
    {
        CurrentWeapon->SimulateEquip();
    }
}

void AShooterCharacter::PlayHit(AActor* OtherActor, EHitType HitType, float HitDamage, FVector HitVector, FVector HitImpulse, FName HitBoneName)
{
    FTakeHitInfo NewHitInfo = LastHitInfo;
    NewHitInfo.HitType = HitType;
    NewHitInfo.HitCounter++;
    NewHitInfo.HitVector = HitVector;
    NewHitInfo.HitImpulse = HitImpulse;
    NewHitInfo.HitBoneName = HitBoneName;
 
    Health -= HitDamage;
    
    if(Health > 0){
        if(HitType==EHitType::NormalHit) // will be changed to physic-based animation soon
        {
        }
        else
        {
            UCharacterMovementComponent* MyCharacterMovement = Cast<UCharacterMovementComponent>(GetCharacterMovement());
            if(MyCharacterMovement)
            {
                MyCharacterMovement->AddImpulse(HitImpulse, false);
            }
        }
    }
    else{
        NewHitInfo.bDeath = true;
        OnDeath();
    }
    
    LastHitInfo = NewHitInfo;
}

void AShooterCharacter::OnRep_LastHitInfo()
{
    if(!LastHitInfo.bDeath){
        if(LastHitInfo.HitType == EHitType::NormalHit){
            SimulateHit();
        }
    }
    else{
        SimulateDeath();
    }
}

void AShooterCharacter::SimulateHit()
{
    FVector2D HitVector2D = FVector2D(LastHitInfo.HitVector.X, LastHitInfo.HitVector.Y);
    FVector2D ForwardVector2D = FVector2D(GetActorForwardVector().X, GetActorForwardVector().Y); 
    float DotProduct = FVector2D::DotProduct(HitVector2D, ForwardVector2D);
    //debug
    FireFloat(DotProduct);
    if(DotProduct > 0)
    {
        PlayCharacterMontage(BackHitMontage);
    }
    else{
        PlayCharacterMontage(FrontHitMontage);
    }
}

void AShooterCharacter::SimulateDeath()
{
    //enable physic to simulate ragdoll
    GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);
    GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
    GetMesh()->SetSimulatePhysics(true);
}

void AShooterCharacter::OnDeath()
{
    float TimeGap = 3.0f;
    AShooterPlayerController* MyPC = Cast<AShooterPlayerController>(GetController());
    if(MyPC){
        MyPC->BeginDelayedRespawn(TimeGap + RespawnTime);
    }
    DetachFromControllerPendingDestroy();
    SetLifeSpan(TimeGap);
    
    //call simulation on server
    SimulateDeath();
}

void AShooterCharacter::MulticastPlayMontage_Implementation(UAnimMontage* AnimMontage, bool bSkipOwner)
{
    if(GetLocalRole() < ROLE_Authority && (!IsLocallyControlled() || !bSkipOwner))
    {
        PlayCharacterMontage(AnimMontage);
    }
}

void AShooterCharacter::EquipWeaponMesh()
{
    if(CurrentWeapon)
    {
        CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentWeapon->UsedSocket);
    }
}
    

void AShooterCharacter::UnequipWeaponMesh()
{
    if(LastEquipWeapon)
    {
        LastEquipWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        LastEquipWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentWeapon->UnUsedSocket);
    }
    
}

void AShooterCharacter::DropWeaponMesh()
{
    if(LastEquipWeapon)
    {
        LastEquipWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }
    
}
