PAV - P3: detección de pitch
============================

Esta práctica se distribuye a través del repositorio GitHub [Práctica 3](https://github.com/albino-pav/P3).
Siga las instrucciones de la [Práctica 2](https://github.com/albino-pav/P2) para realizar un `fork` de la
misma y distribuir copias locales (*clones*) del mismo a los distintos integrantes del grupo de prácticas.

Recuerde realizar el *pull request* al repositorio original una vez completada la práctica.

Ejercicios básicos
------------------

- Complete el código de los ficheros necesarios para realizar la detección de pitch usando el programa
  `get_pitch`.

   * Complete el cálculo de la autocorrelación e inserte a continuación el código correspondiente.
   
     ![image](https://user-images.githubusercontent.com/80951224/144210417-98e9b671-a554-4a7c-ac3a-abe49475d45d.png)


   * Inserte una gŕafica donde, en un *subplot*, se vea con claridad la señal temporal de un segmento de
     unos 30 ms de un fonema sonoro y su periodo de pitch; y, en otro *subplot*, se vea con claridad la
	 autocorrelación de la señal y la posición del primer máximo secundario.
	 
	 **Señal temporal**
	 ![image](https://user-images.githubusercontent.com/91891270/144219057-97a0db44-af49-401e-ae5e-f471fbf8aaa2.png)
	 
	 **Fragmento de fonema sonoro**
	 ![image](https://user-images.githubusercontent.com/91891270/144219509-ec89be5e-9a49-4e01-aed6-ab648ac48e33.png)
	 
	 **Período de pitch**
	 
	 
	 **Máximo secundario de la autocorrelación**

  	

	 `NOTA: es más que probable que tenga que usar Python, Octave/MATLAB u otro programa semejante para
	 hacerlo. Se valorará la utilización de la librería matplotlib de Python.`

   * Determine el mejor candidato para el periodo de pitch localizando el primer máximo secundario de la
     autocorrelación. Inserte a continuación el código correspondiente.
     
     Dado que el primer máximo de la autocorrelación se encuentra en la posición cero, tomamos como valor de pitch el segundo máximo.
     
     ![image](https://user-images.githubusercontent.com/80951224/144223698-b620c906-ece9-44cd-b739-98993b38a9f2.png)

   * Implemente la regla de decisión sonoro o sordo e inserte el código correspondiente.
     
     En la primera versión del programa solo hacemos uso del valor de la autocorrelación r1norm = [1] / r[0] para determinar si un sonido es sordo o sonoro.
     
     ![image](https://user-images.githubusercontent.com/80951224/144215488-ae08bb82-df2b-4da8-8c69-72bd1b66d740.png)


- Una vez completados los puntos anteriores, dispondrá de una primera versión del detector de pitch. El 
  resto del trabajo consiste, básicamente, en obtener las mejores prestaciones posibles con él.

  * Utilice el programa `wavesurfer` para analizar las condiciones apropiadas para determinar si un
    segmento es sonoro o sordo. 
	
	  - Inserte una gráfica con la detección de pitch incorporada a `wavesurfer` y, junto a ella, los 
	    principales candidatos para determinar la sonoridad de la voz: el nivel de potencia de la señal
		(r[0]), la autocorrelación normalizada de uno (r1norm = r[1] / r[0]) y el valor de la
		autocorrelación en su máximo secundario (rmaxnorm = r[lag] / r[0]).

		Puede considerar, también, la conveniencia de usar la tasa de cruces por cero.

	    Recuerde configurar los paneles de datos para que el desplazamiento de ventana sea el adecuado, que
		en esta práctica es de 15 ms.

      - Use el detector de pitch implementado en el programa `wavesurfer` en una señal de prueba y compare
	    su resultado con el obtenido por la mejor versión de su propio sistema.  Inserte una gráfica
		ilustrativa del resultado de ambos detectores.
  
  * Optimice los parámetros de su sistema de detección de pitch e inserte una tabla con las tasas de error
    y el *score* TOTAL proporcionados por `pitch_evaluate` en la evaluación de la base de datos 
	`pitch_db/train`..

   * Inserte una gráfica en la que se vea con claridad el resultado de su detector de pitch junto al del
     detector de Wavesurfer. Aunque puede usarse Wavesurfer para obtener la representación, se valorará
	 el uso de alternativas de mayor calidad (particularmente Python).
   

Ejercicios de ampliación
------------------------

- Usando la librería `docopt_cpp`, modifique el fichero `get_pitch.cpp` para incorporar los parámetros del
  detector a los argumentos de la línea de comandos.
  
  Esta técnica le resultará especialmente útil para optimizar los parámetros del detector. Recuerde que
  una parte importante de la evaluación recaerá en el resultado obtenido en la detección de pitch en la
  base de datos.

  * Inserte un *pantallazo* en el que se vea el mensaje de ayuda del programa y un ejemplo de utilización
    con los argumentos añadidos.
    
    Se han añadido los siguientes parámetros para poder introducirlos por terminal:
       - Umbrales l1 y l2 de permiten fijar los márgenes del central clipping 
       - 
       - 
     **[AÑADIR CAPTURA DOCOPT CUANDO SE HAYAN INCLUIDO TODOS]**  


- Implemente las técnicas que considere oportunas para optimizar las prestaciones del sistema de detección
  de pitch.

  Entre las posibles mejoras, puede escoger una o más de las siguientes:

  * Técnicas de preprocesado: filtrado paso bajo, *center clipping*, etc.
    
    Una de las técnicas de preprocesado que hemos aplicado es la del central clippng.
    
    ![image](https://user-images.githubusercontent.com/80951224/144223489-83a35e0e-c965-4146-bd88-fc9cdd34425b.png)

    Los umbrales l1 y l2 son parámetros que se pueden ajustar introduciéndose por terminal al ejecutar el programa.
	

  * Técnicas de postprocesado: filtro de mediana, *dynamic time warping*, etc.
    
    Con respecto al postprocesado, hemos implementado el filtro de mediana. Este filtro tiene tres coeficientes (muestra anterior, muestra actual y muestra posterior) con el objetivo de eliminar espurios sonoros cuando debería ser una trama sorda y viceversa.
    
    ![Captura de pantalla 2021-12-01 a las 11 59 46](https://user-images.githubusercontent.com/80951224/144222872-962e059e-0fa4-4f58-8018-2f1e6fd40db4.png)
    
  * Métodos alternativos a la autocorrelación: procesado cepstral, *average magnitude difference function*
    (AMDF), etc.
    
    Para mejorar la detección del pitch hemos implementado el Average Magnitude Difference Function (AMDF).
    
    ![image](https://user-images.githubusercontent.com/80951224/144223209-ad5c545d-899b-4a34-8da1-a87afc40234f.png)

    Sin embargo, tras muchas pruebas y modificaciones, hemos comprobado que el porcentage de detección del pitch nos da ligeramente menor que si lo calculamos con la autocorrelación.
    
    **[Anna: poner porcentages con la autocorrelación y con el AMDF]**
    
  * Optimización **demostrable** de los parámetros que gobiernan el detector, en concreto, de los que
    gobiernan la decisión sonoro/sordo.
    
    Para la detección de sonidos sonoros o sordos hemos utilizado los siguientes parámetros:
       - Potencia de la señal 
       - Cociente de la autocorrelación r1norm = r[1] / r[0]
       - Cociente de la autocorrelación rmaxnor = r[pitch] / r[0]
       - **Cruces por cero ZCR**
    
    El algoritmo es el sigueinte:
    
    **[Anna: insertar código de *unvoiced* SI ES QUE SE IMPLEMENTA]**
    
  * Cualquier otra técnica que se le pueda ocurrir o encuentre en la literatura.

  Encontrará más información acerca de estas técnicas en las [Transparencias del Curso](https://atenea.upc.edu/pluginfile.php/2908770/mod_resource/content/3/2b_PS%20Techniques.pdf)
  y en [Spoken Language Processing](https://discovery.upc.edu/iii/encore/record/C__Rb1233593?lang=cat).
  También encontrará más información en los anexos del enunciado de esta práctica.

  Incluya, a continuación, una explicación de las técnicas incorporadas al detector. Se valorará la
  inclusión de gráficas, tablas, código o cualquier otra cosa que ayude a comprender el trabajo realizado.

  También se valorará la realización de un estudio de los parámetros involucrados. Por ejemplo, si se opta
  por implementar el filtro de mediana, se valorará el análisis de los resultados obtenidos en función de
  la longitud del filtro.
   

Evaluación *ciega* del detector
-------------------------------

Antes de realizar el *pull request* debe asegurarse de que su repositorio contiene los ficheros necesarios
para compilar los programas correctamente ejecutando `make release`.

Con los ejecutables construidos de esta manera, los profesores de la asignatura procederán a evaluar el
detector con la parte de test de la base de datos (desconocida para los alumnos). Una parte importante de
la nota de la práctica recaerá en el resultado de esta evaluación.
