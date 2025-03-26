#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAbility.generated.h"

class UAbilityComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class RORA_API ABaseAbility : public AActor
{
	GENERATED_BODY()

public:
	ABaseAbility();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION()
	virtual void Activate();

	UFUNCTION()
	virtual void ApplyDamage();

	UFUNCTION()
	virtual void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						 bool bFromSweep, const FHitResult& SweepResult);
	
	//Getter
	UFUNCTION()
	float GetCooltime() const;

	//Setter
	UFUNCTION()
	void SetCooltime(float NewCooltime); 

	UFUNCTION()
	void SetOwnerPosition(FVector NewPosition);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	USphereComponent* SphereComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* MeshComp;

	//직접 동작하지 않고, 파티클을 생성하는 데만 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* AbilityParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* AbilitySound;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float Cooltime;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Ability")
	FVector OwnerPosition;



	
	//ToDo Code  : 수정할수도 있습니다.
public:
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Ability")
	ACharacter* OwnerCharacter;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Ability")
	UAbilityComponent* OwnerAbilityComponent;
	

	
};
