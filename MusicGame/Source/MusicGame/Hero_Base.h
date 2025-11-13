#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Hero_Base.generated.h"


//-------------------------------------------------------------------------------------------------------------
UCLASS()
class MUSICGAME_API AHero_Base : public APaperZDCharacter {
	GENERATED_BODY()

public:
	AHero_Base();

	// Функции и переменная ниже написаны только для примера и более быстрого вникания,
	// они доступны при добавлении event-ов в blueprint-е Blueprint_Hero
	UFUNCTION(BlueprintCallable) int Test_Function(int param);

	UFUNCTION(BlueprintCallable, Category = "Hero") void Redraw();

	UPROPERTY(BlueprintReadWrite) int Test_Data;
};
//-------------------------------------------------------------------------------------------------------------
