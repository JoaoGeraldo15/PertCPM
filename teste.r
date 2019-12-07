# Nome do arquivo de saida, informado pela aplicacao
ArqSaida <- "teste.png"
#
# Parametros que devem ser informados pelo pert-solver
#

# Numero de atividades e makespan calculado

Makespan <- 20.50
Atividades <- 14
# Vetor com as flags das atividades criticas

Cp <- c(T,T,F,F,T,F,F,F,T,F,T,T,T,T)
#Vetor com os indices das atividades
K <- c(0,1,2,3,4,5,6,7,8,9,10,11,12,13)
#Vetor com os tempos de inicio das atividades
Sk <- c(0,1,2,3,4,5,6,7,8,9,10,11,12,Makespan)
#Vetor com os tempos de termino das atividades
Tk <- c(0,1,2,3,4,5,6,7,8,9,10,11,12,Makespan)
#
# Fim dos parametros. Daqui pra frente deve ser automatico.
#

# Calculos auxiliares
xMin <- 0
xMax <- Makespan + 1
yMin <- 0
yMax <- Atividades + 1
# Geracao de arquivo
png(ArqSaida, width=1200)
# Cria o plot
titulo <- "Gráfico de Gantt"
plot(c(xMin, xMax), c(yMin, yMax), type="n",
	main=titulo, xlab="Tempo", ylab="Atividades");
# Desenha um retangulo para cada atividade
for(indice in 1:length(K))
{
	# Busca as coordenadas dos pontos do retangulo
	Px1 <- Sk[indice]
	Py1 <- K[indice]
	Px2 <- Tk[indice]
	Py2 <- K[indice]
	Px3 <- Tk[indice]
	Py3 <- K[indice]+1
	Px4 <- Sk[indice]
	Py4 <- K[indice]+1

	# Define a cor de acordo com a flag de criticidade
	if(Cp[indice] == T)
	{
	cor <- "red"
	cortexto <- "white"
	}
	else
	{
	cor <- "yellow"
	cortexto <- "black"
	}

	# Desenha o polígono, fechando as coordenadas
	polygon(c(Px1, Px2, Px3, Px4, Px1),
		c(Py1, Py2, Py3, Py4, Py1), col=cor)

	text((Px1 + Px2)/2, (Py1 + Py3)/2, K[indice], col=cortexto)
}
dev.off()
