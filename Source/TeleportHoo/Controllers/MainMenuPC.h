#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MainMenuPC.generated.h"

UCLASS()
class TELEPORTHOO_API AMainMenuPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainMenuPC();

protected:	
	virtual void BeginPlay() override;
	
protected:
	// VARIABLES
	UPROPERTY()
	class UUserWidget* MainMenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class TSubclassOf<UUserWidget> MainMenuWidgetClass;

	IOnlineSessionPtr Sessions;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;			// 세션 검색을 위한 변수 선언
	
public:
	// Functions
	UFUNCTION()
	void HostSession();
	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	UFUNCTION()
	void JoinSession();
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void FindSession();
	UFUNCTION()
	void OnFindSessionsComplete(bool bWasSuccessful);

};