// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SWeaponGrenadeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class LEARNUE4_COOPGAME_API ASWeaponGrenadeLauncher : public ASWeapon
{
	GENERATED_BODY()

public:
	ASWeaponGrenadeLauncher();

	virtual void Fire() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FName MuzzleFlashSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<class ASGrenade> GrenadeBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	class USoundBase* FireSound;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon, meta=(ClampMin=0))
	float LaunchStrength;
};
