#include "PlayerCountSelectWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "RoRaPlayerController.h"

void UPlayerCountSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Confirm)
		Button_Confirm->OnClicked.AddDynamic(this, &UPlayerCountSelectWidget::OnConfirmClicked);

	if (Button_IncreaseCount)
		Button_IncreaseCount->OnClicked.AddDynamic(this, &UPlayerCountSelectWidget::OnIncreaseClicked);

	if (Button_DecreaseCount)
		Button_DecreaseCount->OnClicked.AddDynamic(this, &UPlayerCountSelectWidget::OnDecreaseClicked);

	UpdateCountText();
}

void UPlayerCountSelectWidget::OnConfirmClicked()
{
	if (APlayerController* Controller = GetOwningPlayer())
	{
		if (ARoRaPlayerController* PlayerController = Cast<ARoRaPlayerController>(Controller))
		{
			//어 우리 이만큼 참여할거야
			PlayerController->ServerSetNumberOfParticipants(PlayerCount);
		}
	}
}

void UPlayerCountSelectWidget::OnIncreaseClicked()
{
	PlayerCount = FMath::Clamp(PlayerCount + 1, 2, 10);
	UpdateCountText();
}

void UPlayerCountSelectWidget::OnDecreaseClicked()
{
	PlayerCount = FMath::Clamp(PlayerCount - 1, 2, 10);
	UpdateCountText();
}

void UPlayerCountSelectWidget::UpdateCountText()
{
	if (Text_PlayerCount)
	{
		Text_PlayerCount->SetText(FText::AsNumber(PlayerCount));
	}
}