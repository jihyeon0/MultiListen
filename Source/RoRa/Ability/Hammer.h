// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Hammer.generated.h"

/**
 * 
 */
class UParticleSystemComponent;

UCLASS()
class RORA_API AHammer : public ABaseAbility
{
	GENERATED_BODY()

public:
	AHammer();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void Activate() override;
	virtual void ApplyDamage() override;
	virtual void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						 bool bFromSweep, const FHitResult& SweepResult) override;

};
