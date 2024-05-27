#pragma once
#include "CoreMinimal.h"
#include "LobbyStructs.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* AvatarImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsReady;

	// == 방법과, https://forums.unrealengine.com/t/tarray-find-c2678-error-even-with-operator-overload/422829
	bool operator==(const FPlayerInfo& Other) const
	{
		return PlayerName == Other.PlayerName
			&& AvatarImage == Other.AvatarImage
			&& bIsReady == Other.bIsReady;
	}
};