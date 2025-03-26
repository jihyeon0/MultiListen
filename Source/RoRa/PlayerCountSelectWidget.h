// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCountSelectWidget.generated.h"

/**
 * 
 */
class UButton;
class UTextBlock;


UCLASS()
class RORA_API UPlayerCountSelectWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

protected:
	void UpdateCountText();
	
	UPROPERTY(meta = (BindWidget))
	UButton* Button_Confirm;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_IncreaseCount;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_DecreaseCount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_PlayerCount;

	UFUNCTION()
	void OnConfirmClicked();

	UFUNCTION()
	void OnIncreaseClicked();

	UFUNCTION()
	void OnDecreaseClicked();

	int32 PlayerCount = 2; // 기본값
};
