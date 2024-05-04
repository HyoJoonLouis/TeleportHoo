#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngameHUD.generated.h"

UCLASS()
class TELEPORTHOO_API UIngameHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UChatBox* WBP_ChatBox;

public:
	virtual void NativeConstruct() override;

	void SendChat(const FText& Name, const FText& TextToSend);
};
