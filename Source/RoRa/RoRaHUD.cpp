#include "RoRaHUD.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Widgets/Text/SlateEditableTextTypes.h"

void URoRaHUD::NativeConstruct()
{
	Super::NativeConstruct();

	//OnTextCommitted(입력 이벤트)에 HandleTextCommitted() 바인딩 -> 채팅 입력하면 호출
	//OnTextCommitted 은 EditableText나 EditableTextBox에만 있는 델리게이트입니다^^7 -> 두 번 다시 수직박스에서 찾는 헛짓거리 하지말자  
	if (InputTextBox)
	{
		InputTextBox->OnTextCommitted.AddDynamic(this,&URoRaHUD::HandleTextCommitted);
	}
}

void URoRaHUD::HandleTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	//입력받은 채팅 처리
	//필요하면 FString -> int32로 변환해주기
	//입력박스 다시 공백으로 만들기
	//OnGuessMsg 델리게이트 호출하여 컨트롤러에 메세지 전달

	if (CommitMethod == ETextCommit::Type::OnEnter)
	{
		FString NewString = Text.ToString();
		ARoRaPlayerController* Ownerptr = Cast<ARoRaPlayerController>(GetOwningPlayer());
		OnGuessMsg.Broadcast(NewString, Ownerptr); 
	}
	InputTextBox->SetText(FText::FromString(""));
}

void URoRaHUD::SetChatVerticalBox(const FString& NewString)
{
	if (!ChatVerticalBox)
	{
		return;
	}
	
	UTextBlock* NewText = NewObject<UTextBlock>(this);
	if (NewText)
	{
		NewText->SetText(FText::FromString(NewString));

		UVerticalBoxSlot* NewSlot = ChatVerticalBox->AddChildToVerticalBox(NewText);
	}
}
