#include "IngameHUD.h"
#include "ChatBox.h"

void UIngameHUD::NativeConstruct()
{
	Super::NativeConstruct();

}

void UIngameHUD::SendChat(const FText& Name, const FText& TextToSend)
{
	WBP_ChatBox->AddChat(Name, TextToSend);
}
