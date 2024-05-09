#include "ChatBox.h"
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
		Cast<AIngamePlayerController>(GetOwningPlayer())->Server_SendChat(Text);
		ChatEditText->SetText(FText::GetEmpty());
	}
	GetOwningPlayer()->SetShowMouseCursor(false);
	GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
	ChatEditText->SetVisibility(ESlateVisibility::Collapsed);
}

void UChatBox::SetFocus()
{
	ChatEditText->SetVisibility(ESlateVisibility::Visible);
	ChatEditText->SetFocus();
}



