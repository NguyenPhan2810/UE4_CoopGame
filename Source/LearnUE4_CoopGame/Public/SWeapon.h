// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

UCLASS()
class LEARNUE4_COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

	// Fire a single shot
	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerFire();

	// Automatic 
	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual void BeginFire();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual void EndFire();

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = Weapon)
	void BlueprintFireEvent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	class USkeletalMeshComponent* MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta	= (ClampMin=0))
	float FireInterval;

	FTimerHandle FireTimerHandle;
	float lastFireTime;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool bEnableAutomaticFire;

public:
	// Console variable for debugging
	// value = 0 means disable and > 0 means enable
	static int DebugWeaponDrawing;

public:	
};
