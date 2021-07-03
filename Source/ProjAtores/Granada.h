// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Granada.generated.h"

UCLASS()
class PROJATORES_API AGranada : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGranada();

	// Declara a assinatura da função EndPlay.
	// Como essa função pertece a classe pai desta classe MeuGameMode
	//  Estamos sobrescrevfendo ela via override
	// Virtual indica que é uma função pomórfica(polimorfismo)
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Função que vai ser responsável por colocar o tempo mais lento
	// Ela receve um parametro float(DilatacaoDoTempo) que deve ser entre 0.0 e 1.0
	// Quanto mais próximo de zero mais lento ficará o tempo de todo o jogo.
	UFUNCTION()
	void TempoDevagar(float DilatacaoDoTempo);

	// Função que será responsável por restaurar o tempo ao normal(que é 1.0)
	UFUNCTION()
	void RestaurarTempo();	

	// Foi criado mais um Manipulador(Handle) para gerenciar a função: &AGranada::RestaurarTempo
	// Este manipulador é necessário para não cocorrer problemas de uso com o Hrandle de tempo anterior(Temporizador)
	FTimerHandle Temporizador, TimerDoTempo;

	class UAudioComponent* SomDaExplosao;

	class UParticleSystemComponent* Explosao;

	UFUNCTION()
	void ExplodirGranada();

	// Função que recebe como parâmetro o Raio e a Força a ser aplicado no objeto
	UFUNCTION()
	void ImpulsoRadial(float Raio, float Forca);

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Granada")
	class UStaticMeshComponent* MalhaDoAutor;
	class USphereComponent* ColisorDoAtor;
};
