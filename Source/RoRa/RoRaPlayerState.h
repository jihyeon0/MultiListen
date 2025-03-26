// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GuessResultStruct.h"
#include "RoRaPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RORA_API ARoRaPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	int32 GetResultStrike();
	
	UFUNCTION()
	int32 GetResultOut();
	
	UFUNCTION()
	int32 GetCurrentTry();

	UFUNCTION()
	void SetCurrentTry(int32 count = 1);

	UFUNCTION()
	void ResetGame();
	
	UFUNCTION()
	void UpdateGuessResultStruct(const FGuessResultStruct& GuessResult);
	
	//올바르지 않은 게임추측답변에 대해 OUT 카운트 처리 
	UFUNCTION() 
	FGuessResultStruct& NotifyIncorrectAnswerAddPlayerOut();
	
protected:
	UPROPERTY(Replicated,EditAnywhere)
	int32 CurrentTry = 0;	
	
	//실제 정의된 구조를 알아야하니까 전방선언만으로는 무리겠지요? 포인터로 저장하는 변수는 포인터 크키만 알면 되니까 괜찮겠지요? 이걸 아직도 헷갈리면 안 괜찮겠지요?
	UPROPERTY(Replicated)
	TArray<FGuessResultStruct> GuessResultStruct;
};
