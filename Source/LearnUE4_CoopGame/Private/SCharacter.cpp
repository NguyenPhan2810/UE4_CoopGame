// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "SWeapon.h"
#include "SWeaponRifle.h"
#include "SWeaponGrenadeLauncher.h"
#include "Components/SHealthComponent.h"	
#include "../LearnUE4_CoopGame.h"

#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/PawnMovementComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/CapsuleComponent.h>
#include <Net/UnrealNetwork.h>


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Enable crouch
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Prevent capsule from blocking collision event on Weapon channel
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	// Setup components
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<USHealthComponent>("HealthComponent");


	// Setup variables
	bAimDownSight = false;
	AimedFov = 45;
	DefaultFov = 70;
	AdsInterpSpeed = 25;
	bDied = false;

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentFov = DefaultFov;
	CameraComponent->SetFieldOfView(CurrentFov);

	// Register event of health change
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);

	HandleSpawnWeapons(1);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Spawn Weapon
	if (IsPlayerControlled())
	{
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One) && RifleBP)
			HandleSpawnWeapons(1);
		else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two) && GrenadeLauncherBP)
			HandleSpawnWeapons(2);
	}

	// Aim down sight

	float targetFov = bAimDownSight ? AimedFov : DefaultFov;
	
	CurrentFov = FMath::FInterpTo(CurrentFov, targetFov, DeltaTime, AdsInterpSpeed);

	CameraComponent->SetFieldOfView(CurrentFov);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Directional movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	// Crouch
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);
	
	// Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	// Mouse look
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	// Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::BeginFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::EndFire);

	// Aim down sight
	PlayerInputComponent->BindAction("AimDownSight", IE_Pressed, this, &ASCharacter::BeginAimDownSight);
	PlayerInputComponent->BindAction("AimDownSight", IE_Released, this, &ASCharacter::EndAimDownSight);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (CameraComponent)
	{
		return CameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void ASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacter, weapon);
	DOREPLIFETIME(ASCharacter, bDied);
}

void ASCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}

void ASCharacter::BeginFire()
{
	if (weapon)
	{
		weapon->BeginFire();
	}
}

void ASCharacter::EndFire()
{
	if (weapon)
	{
		weapon->EndFire();
	}
}

void ASCharacter::BeginAimDownSight()
{
	bAimDownSight = true;
}

void ASCharacter::EndAimDownSight()
{
	bAimDownSight = false;
}

void ASCharacter::HandleSpawnWeapons_Implementation(int weaponType)
{
	if (weaponType == 1)
	{
		if (weapon)
		{
			weapon->Destroy();
		}

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		weapon = GetWorld()->SpawnActor<ASWeaponRifle>(RifleBP, spawnParams);
		weapon->SetOwner(this);
		weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
	}
	else if (weaponType == 2)
	{
		if (weapon)
		{
			weapon->Destroy();
		}

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		weapon = GetWorld()->SpawnActor<ASWeaponGrenadeLauncher>(GrenadeLauncherBP, spawnParams);
		weapon->SetOwner(this);
		weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
	}
}

bool ASCharacter::HandleSpawnWeapons_Validate(int weaponType)
{
	return true;
}

void ASCharacter::OnHealthChanged(USHealthComponent* HealthComponentDamaged, float CurrentHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (CurrentHealth <= 0 && !bDied)
	{
		// Die
		bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		EndFire();
		DetachFromControllerPendingDestroy();
		SetLifeSpan(2);
	}
}