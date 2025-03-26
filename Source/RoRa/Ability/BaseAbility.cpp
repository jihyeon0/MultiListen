#include "BaseAbility.h"

#include "AbilityComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"


ABaseAbility::ABaseAbility()
{
	bReplicates = true;
}

void ABaseAbility::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseAbility::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseAbility, OwnerPosition);
	DOREPLIFETIME(ABaseAbility, Cooltime);
	DOREPLIFETIME(ABaseAbility, OwnerCharacter);
	DOREPLIFETIME(ABaseAbility, OwnerAbilityComponent);
}

void ABaseAbility::Activate()
{
	
}

void ABaseAbility::ApplyDamage()
{
}

void ABaseAbility::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

float ABaseAbility::GetCooltime() const
{
	return Cooltime;
}

void ABaseAbility::SetCooltime(float NewCooltime)
{
	Cooltime = NewCooltime;
}

void ABaseAbility::SetOwnerPosition(FVector NewPosition)
{
	//UKismetSystemLibrary::PrintString(this,FString::Printf(TEXT("ABaseAbility::SetOwnerPosition OwnerPosition: X=%.2f, Y=%.2f, Z=%.2f"), NewPosition.X, NewPosition.Y, NewPosition.Z));

	OwnerPosition = NewPosition;
	SetActorLocation(OwnerPosition);
}
