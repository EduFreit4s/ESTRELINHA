

<h1 align="center">
    
    Estrelinha - Um rastreador solar com arduino
</h1>

<h1 align="center">
  <img src="https://github.com/EduFreit4s/ESTRELINHA/blob/master/images/estrelinha_1.jpg" width=60%>
</h1>
  
<p align="center">
  <a href="#introducao">Introdução</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#objetivos">Objetivos</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#materiais">Materiais</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#metodologia">Metodologia</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#resultados">Resultados</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#conclusão">Conclusão</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#referências">Referências</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#sobre">Sobre</a>
</p>

### Introdução

>&nbsp;&nbsp;&nbsp;&nbsp; Painéis solares são o jeito mais fácil de obter energia limpa e renovável. Os painéis
fixos são a maneira mais barata para isso. Tipicamente os painéis solares são instalados com a
face voltada para norte ou sul cartográfico (dependendo do hemisfério onde se encontra) e
com uma inclinação abaixo da latitude local. Isso garante que a produção energética anual
seja boa, mas não significa que os painéis estejam gerando tanta energia quanto possível. Isso
acontece devido ao fato que as células que compõe um painel solar só atingem seu pico de
produção quando o ângulo de incidência dos raios solares é perpendicular à superfície desta
célula. E essa é a proposta da nossa máquina.

<p align="center">
  <img src="https://github.com/EduFreit4s/ESTRELINHA/blob/master/images/estrelinha_2.jpg" width=60%>
</p>

### Objetivos

>&nbsp;&nbsp;&nbsp;&nbsp; Julho de 2019 foi o mês mais quente da história segundo a organização meteorológica mundial. O último relatório climático da ONU apontou que o planeta já está um grau celsius
mais quente do que antes da revolução industrial. É pouco, mas o suficiente para modificar o clima de forma irreversível. Hodiernamente, precisamos transformar nossa matriz energética em uma escala global. “Essa é a corrida da nossa vida, por nossas vidas”. E se a gente não pensar fora da caixa, a nossa história nesse pálido ponto azul será breve. Foi pensado que o primeiro passo poderia ser dado conseguir maximizar a eficiência da tecnologia atual, não apenas do ponto de vista de consumo, mas também da produção que é a função de um rastreador solar.

<p align="center">
  <img src="" width=60%>
</p>

### Materiais

> ➔ 2 servos SG90  
➔ Arduino nano  
➔ 4 ldrs 10mm  
➔ 4 resistores de 10㏀  
➔ 1 resistor 220Ω  
➔ 1 cooler  
➔ Mini protoboard  
➔ Jumpers  
➔ Estrutura em madeira  
➔ Parafusos

<p align="center">
  <img src="" width=60%>
</p>

### Metodologia

>&nbsp;&nbsp;&nbsp;&nbsp; Existe diversas formas de seguir o sol. Alguns sistemas se destacam por terem a possibilidade de executar múltiplos movimentos. Com um eixo, é possível acompanhar a estrela ao longo do dia. Esse sistema é o mais adotado atualmente, principalmente em fazendas solares (usinas) onde mais se empregam essa tecnologia.   
&nbsp;&nbsp;&nbsp;&nbsp; Já com dois eixos é possível compensar a inclinação da terra ao longo das estações.
Isso torna o sistema ainda mais produtivo.  
Nosso protótipo conta com dois eixos. Isso significa que ele é capaz de se inclinar para
quatro direções diferentes, além disso, esses movimentos são coordenados por duas maneiras
diferentes:  
&nbsp;&nbsp;&nbsp;&nbsp; No modo fotossensível, os fotossensores posicionados nas extremidades do painel são
responsáveis por informar o microcontrolador a direção da fonte de luz. Cada ldr está em série com resistor de 10k atuando como divisores de tensão. Quando
a intensidade da luz é máxima, a resistência do ldr é desprezível e a assim, a corrente total flui para o pino analógico do arduino. Quando a intensidade da luz cai, a resistência do ldr aumenta proporcionalmente provocando uma queda de tensão que deixa de chegar ao microcontrolador. Internamente, as correntes de cada ldr são interpretadas pelo conversor A/D com 2 10 bits de resolução, em seguida, transformadas em um número que vai de 0 a 1023 para cada ponto. Utilizamos esse número para fazer médias, esquerda, direita, superior, inferior e depois a diferença entre elas. Com essas médias, o arduino determina a posição da luz e diz para qual direção cada servo irá girar.  
&nbsp;&nbsp;&nbsp;&nbsp; No modo cronológico, mapeamos a trajetória do sol no céu em uma escala de graus/minutos. Utilizamos o relógio interno do arduino para contar um intervalo. A cada lapso de tempo, o arduino envia um sinal para servo mover a estrutura em um grau. Uma vez que o movimento do sol é altamente previsível, podemos descartar o uso de sensores externos e conseguir o mesmo efeito gastando menos energia e reduzindo componentes.  

### Resultados

>&nbsp;&nbsp;&nbsp;&nbsp; Primeiramente, nosso projeto tinha como objetivo apenas montar uma máquina inteligente capaz de seguir uma fonte de luz e justificar sua necessidade, mas durante a jornada, pensamos que seria interessante compará-la com uma instalação tradicional, onde o painel se encontra fixo. Devido ao orçamento baixo e alta criatividade, utilizamos um cooler
de computador para aferir a produção de energia de cada sistema. A ideia consiste em contar o número de rotações que o cooler efetuou no final do dia. Mais rotações significa mais energia produzida.    
&nbsp;&nbsp;&nbsp;&nbsp; O cooler conta com um sensor interno conhecido como hall. Esse sensor detecta a variação do campo magnético produzida pela bobina do motor que faz o rotor girar. Um circuito interno percebe essa variação e manda um sinal alto pelo fio azul do cooler, por último, nosso software dentro do arduino evita que um único pulso seja contabilizado múltiplas vezes durante os ciclos de loop.    
&nbsp;&nbsp;&nbsp;&nbsp; A cada minuto, o arduino imprime no serial monitor a quantidade de rotações efetuadas naquele momento. É importante ressaltar que essa aferição não segue critérios exigentes. O ideal seria utilizar sensores de tensão e corrente em cima de uma carga puramente resistiva além de executar diversos ensaios. Entretanto, foi o necessário pela nossa condição financeira e foi válido para a comparação.

<p align="center">
  <img src="" width=60%>
</p>

### Conclusão

>&nbsp;&nbsp;&nbsp;&nbsp; O jeito mais barato de obter energia solar ainda é instalando painéis fixos no telhado. O alto custo de um sistema inteligente e a necessidade de manutenções periódicas inviabiliza seu uso em escala reduzida, entretanto, a vantagem na produção de energia (cerca de 20%) com a mesma quantidade de células é essencial para locais com áreas reduzidas ou usinas solares. Em síntese, observou-se a necessidade do rastreador solar para aumentar o grau de obtenção de energia, como operações eficientes de produção de potência, boas condições de armazenamento e tem a vantagem de não produzir danos ao meio ambiente.

### Referências

> BrownDogGadgets. Rastreador de eixo duplo. Acesso em 15 de junho de 2019. Disponível em: https://www.instructables.com/id/Dual-Axis-Tracker-V20/  
EduFreit4s. Código fonte. Acesso em 7 de agosto de 2019. Disponível em: https://github.com/EduFreit4s/estrelinha/blob/master/estrelinha.ino    
Tinkercad. Simulação. Acesso em 9 de agosto de 2019. Disponível em: https://www.tinkercad.com/things/bVD1aZfUp95-estrelinha    
Multisim. Simulação/Circuito. Acesso em 2 de agosto de 2019.      
Portal solar. Site/consulta. Acesso em 26 de junho de 2019. Disponível em: https://www.portalsolar.com.br/a-melhor-direcao-do-painel-solar-fotovoltaico.html  

### Sobre

-   Autores: Eduardo Freitas, Fernanda Ferreira, Matheus Inocêncio e Victor Ramalho
-   Estudantes do IFPB - Campus João Pessoa
-   Curso: Bacharelado em Engenharia Elétrica
-   Disciplinas: Algoritmos e lógica de programação | Introdução a Engenharia Elétrica 
> Email:
-   [freitas.eduardo@academico.ifpb.edu.br](mailto:freitas.eduardo@academico.ifpb.edu.br)
-   [fernanda.n.f.a@gmail.com](mailto:fernanda.n.f.a@gmail.com)       
-   [matheusino11@gmail.com](mailto:matheusino11@gmail.com) 
-   [VictorRamalho101@gmail.com](mailto:VictorRamalho101@gmail.com)


