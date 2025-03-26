// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RandomLibrary.generated.h"

/**
 * 랜덤숫자생성 전용 라이브러리
 * 변수 저장 하지 말고 , 입력 → 출력 구조로 만들기
 */
UCLASS()
class RORA_API URandomLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RandomLibrary")
	static int32 GenerateRandomNumber();
	static TArray<int32> GenerateRandomOrder(int32 PlayerArrayNum);
};
