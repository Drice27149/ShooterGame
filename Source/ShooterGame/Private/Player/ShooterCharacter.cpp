// Drice

#include "Player/ShooterCharacter.h"
#include "ShooterGame.h"
#include "Weapon/Weapon.h"


AShooterCharacter::AShooterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
    
    // Set up Default Weapon
    CurrentWeapon = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterCharacter::OnJumpStart);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AShooterCharacter::OnJumpEnd);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::OnMoveRight);
    
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AShooterCharacter::OnStartFire);
}

void AShooterCharacter::OnMoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
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
		// find out which way is right
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
        ServerStartFire();
    }
}

void AShooterCharacter::ServerStartFire_Implementation()
{
    CurrentWeapon->HandleFire();
    AllClientStartFire();
}

void AShooterCharacter::AllClientStartFire_Implementation()
{
    // play weapon animation
    CurrentWeapon->PlayWeaponFireAnimation();
    // play character animation
    PlayWeaponFireAnimation(CurrentWeapon);
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AShooterCharacter, CurrentWeapon);
    DOREPLIFETIME(AShooterCharacter, StateMachine);
}

void AShooterCharacter::OnRep_CurrentWeapon()
{
    UpdateWeaponMesh(CurrentWeapon);
    PlayWeaponChangedAnimation(CurrentWeapon);
}

void AShooterCharacter::OnEquipDefaultWeapon()
{
    ServerEquipDefaultWeapon();
}

void AShooterCharacter::ServerEquipDefaultWeapon_Implementation()
{
    if(DefaultWeaponClass != NULL)
    {
        FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
        AWeapon* DefaultWeapon = Cast<AWeapon>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, DefaultWeaponClass, SpawnTransform));
        if(DefaultWeapon != NULL)
        {
            DefaultWeapon->OwnerPawn = Cast<APawn>(this);
            UGameplayStatics::FinishSpawningActor(DefaultWeapon, SpawnTransform);
        }
        CurrentWeapon = DefaultWeapon;
        // server won't call OnRep_CurrentWeapon automatically, need to call it by hand
        OnRep_CurrentWeapon();
    }
}

void AShooterCharacter::ServerUpdateStateMachine_Implementation(const FStateMachine& NewState)
{
    StateMachine = NewState;
}

void AShooterCharacter::ServerChangeWalkSpeed_Implementation(const int& NewSpeed)
{
    GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AShooterCharacter::OnCrouchStart()
{
    StateMachine.IsCrouching = true;
    ServerUpdateStateMachine(StateMachine);
}
    
void AShooterCharacter::OnCrouchEnd()
{
    StateMachine.IsCrouching = false;
    ServerUpdateStateMachine(StateMachine);
}
    
void AShooterCharacter::OnJumpStart()
{
    Jump();
    StateMachine.IsJumping = true;
    ServerUpdateStateMachine(StateMachine);
}
    
void AShooterCharacter::OnJumpEnd()
{
    StopJumping();
    StateMachine.IsJumping = false;
    ServerUpdateStateMachine(StateMachine);
}
    
void AShooterCharacter::OnRunStart()
{
    StateMachine.IsRunning = true;
    ServerUpdateStateMachine(StateMachine);
    ServerChangeWalkSpeed(RunSpeed);
}
    
void AShooterCharacter::OnRunEnd()
{
    StateMachine.IsRunning = false;
    ServerUpdateStateMachine(StateMachine);
    ServerChangeWalkSpeed(WalkSpeed);
}
    
void AShooterCharacter::OnViewModeStart()
{
    StateMachine.IsViewMode = true;
    ServerUpdateStateMachine(StateMachine);
}

void AShooterCharacter::OnViewModeEnd()
{
    StateMachine.IsViewMode = false;
    ServerUpdateStateMachine(StateMachine);
}


