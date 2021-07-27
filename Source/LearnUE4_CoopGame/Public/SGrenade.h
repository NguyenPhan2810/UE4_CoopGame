// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGrenade.generated.h"

UCLASS()
class LEARNUE4_COOPGAME_API ASGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Explode();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	class UStaticMeshComponent* MeshComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	class URadialForceComponent* RadialForceComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ExplosiveBarrel)
	class USoundBase* ExplodeSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float ExplosionTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float damageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	class UParticleSystem* ExplodeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool ExplodeOnImpact;

	FTimerHandle UnusedTimerHandle;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
};
