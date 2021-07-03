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

	// Declara a assinatura da fun��o EndPlay.
	// Como essa fun��o pertece a classe pai desta classe MeuGameMode
	//  Estamos sobrescrevfendo ela via override
	// Virtual indica que � uma fun��o pom�rfica(polimorfismo)
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Fun��o que vai ser respons�vel por colocar o tempo mais lento
	// Ela receve um parametro float(DilatacaoDoTempo) que deve ser entre 0.0 e 1.0
	// Quanto mais pr�ximo de zero mais lento ficar� o tempo de todo o jogo.
	UFUNCTION()
	void TempoDevagar(float DilatacaoDoTempo);

	// Fun��o que ser� respons�vel por restaurar o tempo ao normal(que � 1.0)
	UFUNCTION()
	void RestaurarTempo();	

	// Foi criado mais um Manipulador(Handle) para gerenciar a fun��o: &AGranada::RestaurarTempo
	// Este manipulador � necess�rio para n�o cocorrer problemas de uso com o Hrandle de tempo anterior(Temporizador)
	FTimerHandle Temporizador, TimerDoTempo;

	class UAudioComponent* SomDaExplosao;

	class UParticleSystemComponent* Explosao;

	UFUNCTION()
	void ExplodirGranada();

	// Fun��o que recebe como par�metro o Raio e a For�a a ser aplicado no objeto
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
