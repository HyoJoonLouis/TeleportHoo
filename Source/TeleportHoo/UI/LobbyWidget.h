#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerLobbyInfoWidget.h"
#include "LobbyWidget.generated.h"

UCLASS()
class TELEPORTHOO_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void UpdatePlayerInfo(int32 PlayerIndex, const FPlayerInfo& PlayerInfo);
	void SetStartButtonEnabled(bool bEnabled);
	void SetStartButtonVisibility(bool bIsVisible);

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void OnStartButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void OnReadyButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void SetServerName(const FString& ServerName);

	void SetWidgetSwitcherIndex(int32 Index);

public:

protected:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WS_WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UPlayerLobbyInfoWidget* WBP_PlayerLobbyInfoWidget_1;
	UPROPERTY(meta = (BindWidget))
	class UPlayerLobbyInfoWidget* WBP_PlayerLobbyInfoWidget_2;

	UPROPERTY(meta = (BindWidget))
	class UButton* B_StartButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* B_ReadyButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* T_ServerName;
};
