#include "ChatBox.h"
#include "Components/Border.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Chat.h"
#include "../GameModes/IngamePlayerController.h"

void UChatBox::NativeConstruct()
{
	Super::NativeConstruct();

	ChatEditText->OnTextCommitted.AddUniqueDynamic(this, &UChatBox::ChatOnCommitted);
}

void UChatBox::AddChat(const FText& Name, const FText& TextToSend)
{
	UChat* NewChat = Cast<UChat>(CreateWidget(this, ChatClass));
	NewChat->SetTexts(Name, TextToSend);
	ChatScrollBox->AddChild(NewChat);
	ChatScrollBox->ScrollToEnd();
}

void UChatBox::ChatOnCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if(!Text.IsEmpty())
			Cast<AIngamePlayerController>(GetOwningPlayer())->Server_SendChat(Text);
		ChatEditText->SetText(FText::GetEmpty());
	}
	GetOwningPlayer()->SetShowMouseCursor(false);
	GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
	BoxBorder->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f, 0));
	ChatScrollBox->SetAlwaysShowScrollbar(false);
	ChatEditText->SetVisibility(ESlateVisibility::Hidden);
}

void UChatBox::SetFocus()
{
	BoxBorder->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	ChatScrollBox->SetAlwaysShowScrollbar(true);
	ChatEditText->SetVisibility(ESlateVisibility::Visible);
	ChatEditText->SetFocus();
}



