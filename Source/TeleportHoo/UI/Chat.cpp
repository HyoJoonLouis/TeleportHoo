#include "Chat.h"
#include "Components/TextBlock.h"

void UChat::SetTexts(const FText& PlayerName, const FText& TextToShow)
{
	//NameText->SetText(PlayerName);
	TextText->SetText(TextToShow);
}
