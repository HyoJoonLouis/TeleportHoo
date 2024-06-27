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

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UStaticMeshComponent* GetSwordMesh() { return SwordMesh; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UStaticMeshComponent* GetShieldMesh() { return ShieldMesh; }

	UFUNCTION(BlueprintCallable)
	void SetSwordAsCollision();
	UFUNCTION(BlueprintCallable)
	void SetShieldAsCollision();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack | Mesh")
	class UStaticMeshComponent* SwordMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack | Mesh")
	class UStaticMeshComponent* ShieldMesh;
};