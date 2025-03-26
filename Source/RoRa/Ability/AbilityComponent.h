// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

class ABaseAbility;

//스킬 목록 관리
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RORA_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UFUNCTION()
	void AddAbility(ABaseAbility* NewAbility);

	UFUNCTION()
	void OnRep_SpawnAbility();

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseAbility>> StartAbilities; //시작 시 보유한 스킬
	
protected:
	/*
	 *ReplicatedUsing = OnRep_SpawnAbility, 클라를 위한, 값이 바뀌면 호출, 리슨서버의 클라는 호출되지 않음
	
	서버에서 SpawnAbility가 생성되면서 이미 nullptr이 아닌 값으로 변했기 때문에,
	리슨 서버의 클라이언트 부분에서는 "값이 변하는 순간"이 없어서 OnRep_SpawnAbility()가 호출되지 않는 것

	중요! 이게 블루프린트에서는 서버에서도 불리는데 c++에서 서버에서는 불리지 않음
	 */
	
	UPROPERTY(ReplicatedUsing = OnRep_SpawnAbility, EditAnywhere)
	ABaseAbility* SpawnAbility = nullptr;
	
	UPROPERTY(EditAnywhere)
	TArray<ABaseAbility*> AbilitesInstance; 
};
