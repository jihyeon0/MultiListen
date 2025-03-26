#pragma once
#include "CoreMinimal.h"
#include "AbilityEnum.generated.h"

UENUM(BlueprintType)
enum class EAbilityEnum : uint8
{
	Hammer UMETA(DisplayName = "Hammer"),
	FrostBolt UMETA(DisplayName = "FrostBolt"),
	Lightning UMETA(DisplayName = "Lightning"),
	FireBall UMETA(DisplayName = "FireBall")
};
