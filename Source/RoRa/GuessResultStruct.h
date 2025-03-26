#pragma once
//사용자 정의 구조체 만들고 헤더를 빼먹는 얼간이가 있다? TIL에 언리얼 시스템 등록하려면 헤더 추가해야한다고 정성껏 적었던 사람 어디갓지?
#include "CoreMinimal.h"
#include "GuessResultStruct.generated.h"

USTRUCT(BlueprintType)
struct FGuessResultStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int StrikeCount = 0;

	UPROPERTY(BlueprintReadWrite)
	int BallCount = 0;

	UPROPERTY(BlueprintReadWrite)
	int OutCount = 0;

	UPROPERTY(BlueprintReadWrite)
	FString ResultString;
};
