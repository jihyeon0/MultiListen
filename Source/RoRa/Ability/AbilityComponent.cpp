#include "AbilityComponent.h"
#include "BaseAbility.h"
#include "Hammer.h"
#include "Chaos/CollisionResolutionUtil.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"


UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner()->HasAuthority()) //서버에서 불리고 클라에서 불려서 중복 생성됨 -> 서버에서만 생성 하도록 변경
	{
		//BP로 생성하고 싶으면 TSubclassOf<>
		for (TSubclassOf<ABaseAbility> StartAbility : StartAbilities)
		{
			FActorSpawnParameters SpawnParm;
			//SpawnAbility = GetWorld()->SpawnActor<ABaseAbility>(StartAbility);
			//UKismetSystemLibrary::PrintString(this, "Spawned Ability");
			//서버가 SpawnAbility가 누군지 알려줘야함 -> 안 알려주면  서버에서 해머를 생성하지만 클라는 몰라서 생성되지 않음
			//서버에서 SpawnAbility 값이 변경되면 클라에서 OnRep_SpawnAbility() 호출
			if (SpawnAbility)
			{
				//SpawnAbility->SetReplicates(true);
			}
			
		}
	}
	
	//UKismetSystemLibrary::PrintString(this, SpawnAbility != nullptr ? TEXT("NOT NULL Ability"):TEXT("NULL Ability"));

	//리슨서버의 클라는 OnRep_SpawnAbility() 호출 이벤트 발생하지 않음
	//리슨서버의 클라에서도 추가할 수 있도록 함
	if (GetNetMode() != NM_Client && SpawnAbility != nullptr)
	 {
		//AddAbility(SpawnAbility);
	 }
}

void UAbilityComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UKismetSystemLibrary::PrintString(this, "Called UAbilityComponent::TickComponent");

	for (ABaseAbility* Instance : AbilitesInstance)
	{
		if (Instance!=nullptr)
		{
			//UKismetSystemLibrary::PrintString(this, "NOT NULL ABaseAbility* Instance");
		}
		FVector NewPosition = GetOwner()->GetActorLocation();
		NewPosition.Z += 10.f;
		Instance->SetOwnerPosition(NewPosition);
	}
}

void UAbilityComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAbilityComponent, SpawnAbility);
}

void UAbilityComponent::AddAbility(ABaseAbility* NewAbility)
{
	if (NewAbility==nullptr)
		return;

	AbilitesInstance.AddUnique(NewAbility);
}

void UAbilityComponent::OnRep_SpawnAbility()
{
	if (SpawnAbility != nullptr)
	{
		//AddAbility(SpawnAbility);
	}
}

