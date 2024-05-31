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
	UPROPERTY(meta = (BindWidget))
	class UScoreBoard* WBP_ScoreBoard;
	UPROPERTY(meta = (BindWidget))
	class UImage* FadeInOutImage;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FadeOutAnimation;

public:
	virtual void NativeConstruct() override;

	void SendChat(const FText& Name, const FText& TextToSend);
	void PlayCinema(bool bStart);
	void UpdateScore();
	UFUNCTION(BlueprintCallable)
	void OnHitEffect();
	void FadeInOut(bool FadeOut);

	FORCEINLINE UChatBox* GetChatBox() { return WBP_ChatBox; }
};
