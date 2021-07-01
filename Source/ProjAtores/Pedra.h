// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pedra.generated.h"

UCLASS()
class PROJATORES_API APedra : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APedra();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(EditAnywhere, Category = "Pedra")
	class UStaticMeshComponent* MalhaPedra;
	class USphereComponent* ColisorPedra;

};
