// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class LEARNUE4_COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Directional movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	// Crouch
	void BeginCrouch();
	void EndCrouch();

	// Fire
	void BeginFire();
	void EndFire();

	// Aim down sight
	void BeginAimDownSight();
	void EndAimDownSight();

	// Spawn weapons as requested
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void HandleSpawnWeapons(int weaponType);

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* OwnerHealthComponent, float CurrentHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class USHealthComponent* HealthComponent;

	// Weapon Component
	UPROPERTY(EditAnywhere, Category = Weapon)
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class ASWeaponRifle> RifleBP;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class ASWeaponGrenadeLauncher> GrenadeLauncherBP;
	
protected:
	UPROPERTY(Replicated)
	class ASWeapon* weapon;

	UPROPERTY(EditAnywhere, Category = Player, meta = (ClampMin = 0.1, ClampMax = 100))
	float AdsInterpSpeed;

	UPROPERTY(EditAnywhere, Category = Player, meta = (ClampMin = 0.1, ClampMax = 360))
	float AimedFov;

	UPROPERTY(EditAnywhere, Category = Player, meta = (ClampMin = 0.1, ClampMax = 360))
	float DefaultFov;

	bool bAimDownSight;
	float CurrentFov;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Player)
	bool bDied;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
};
