// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoRaPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "RoRaHUD.generated.h"


//델리게이트 시그니처 정의(델리게이트 이름, 타입, 변수명)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGuessMsg, const FString&, GuessMsg, ARoRaPlayerController*, Ownerptr);

class UVerticalBox;
class UEditableTextBox;

UCLASS()
class RORA_API URoRaHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void HandleTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void SetChatVerticalBox(const FString& NewString);
	
	//이벤트 디스패처 OnGuessMsg 에 컨트롤러의 ServerSendMsg 바인딩
	//BlueprintAssignable: Event Dispatcher (델리게이트)를 블루프린트에서 바인딩할 수 있도록 해주는 키워드
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnGuessMsg OnGuessMsg;

	//OnGuessMsg는 언제 불리는지 -> (메세지 입력하고)엔터가 눌리면

	//meta = (BindWidget): UMG에서 만든 위젯과, 이 C++ 변수 연결 "해줘"
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ChatVerticalBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> HistoryVerticalBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> InputTextBox;

	
protected:
	int32 GuessMsg = 0;
};
/*
 *VerticalBox_Main
 *TextBlock_Title
 *Button_Left
 *TextBlock_Left
 *TextBlock_PlayerCount
 *Button_Right
 *TextBlock_Right
 *Button_Confirm
 *TextBlock_Confirm
 */