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
	UPROPERTY(meta = (BindWidget))
	class UImage* Blood;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* BloodAnimation;
	UPROPERTY(meta = (BindWidget))
	class UCinemaWidget* WBP_Cinema;

public:
	virtual void NativeConstruct() override;

	void SendChat(const FText& Name, const FText& TextToSend);
	void PlayCinema(bool bStart);
	UFUNCTION(BlueprintCallable)
	void OnHitEffect();

	FORCEINLINE UChatBox* GetChatBox() { return WBP_ChatBox; }
};
