// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SWeaponRifle.generated.h"

/**
 * 
 */
UCLASS()
class LEARNUE4_COOPGAME_API ASWeaponRifle : public ASWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Fire() override;

	virtual void PlayFireEffect(bool hit, FHitResult hitResult);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	UParticleSystem* SmokeTrailEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName SmokeTrailStartParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName SmokeTrailEndParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName MuzzleFlashSocketName;

	UPROPERTY(EditDefaultsOnly, Category=Camera)
	TSubclassOf<UCameraShakeBase> CameraShake;
};
