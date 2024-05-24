#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Leonidas.generated.h"

UCLASS()
class TELEPORTHOO_API ALeonidas : public ABaseCharacter
{
	GENERATED_BODY()
public:
	ALeonidas();
	void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void StartWeaponCollision();
	UFUNCTION(BlueprintCallable)
	void EndWeaponCollision();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack | Mesh")
	class UStaticMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack | Mesh")
	class UStaticMeshComponent* ShieldMesh;
};
