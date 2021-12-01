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
	 
	 **Señal temporal - Imagen 1**
	 ![image](https://user-images.githubusercontent.com/91891270/144219057-97a0db44-af49-401e-ae5e-f471fbf8aaa2.png)
	 
	 **Fragmento de fonema sonoro - Imagen 2**
	 ![image](https://user-images.githubusercontent.com/91891270/144219509-ec89be5e-9a49-4e01-aed6-ab648ac48e33.png)
	 
	 **Período de pitch - Imagen 3**
	 
	 ![image](https://user-images.githubusercontent.com/91891270/144229199-e6c27ab6-3b4e-4f5a-8829-b508282c4a02.png)
	 
	 **Máximo secundario de la autocorrelación - Imagen 4**
	 
	<img width="388" alt="image" src="https://user-images.githubusercontent.com/91891270/144229923-6517a715-6c89-45e6-b837-93deaaa95f09.png">

	Para visualizar este resultado hemos guardado en diferentes ficheros .txt los valores de l'autocorrelación de cada trama. Para ello hemos introducido las sigüientes líneas de código:
	
	![image](https://user-images.githubusercontent.com/91891270/144229631-ac9d4a9c-1886-405a-89d2-2b7c73e99192.png)
	
	Al ejecutar el programa se nos crean los siguientes ficheros.
	![image](https://user-images.githubusercontent.com/91891270/144230166-440689dc-b1dc-4d58-8508-b0b9af3d5bbf.png)

	Una vez ejecudado el programa hemos buscado una trama sonora, en nuestro caso la 14 (imagen 3) y hemos visualizado la autocorrelación con Matlab (imagen 4). Vemos que efectivamente, la detección de pitch se ha realizado correctamente.
	
	 `NOTA: Las linias de código referentes a la creación de ficheros deberan dejarse comentadas en futuras ejecuciones.`

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
		
		![image](https://user-images.githubusercontent.com/91891270/144232007-a2a02835-5b18-448d-9740-6ef1a6055a09.png)


		Puede considerar, también, la conveniencia de usar la tasa de cruces por cero.

	    Recuerde configurar los paneles de datos para que el desplazamiento de ventana sea el adecuado, que
		en esta práctica es de 15 ms.

      - Use el detector de pitch implementado en el programa `wavesurfer` en una señal de prueba y compare
	    su resultado con el obtenido por la mejor versión de su propio sistema.  Inserte una gráfica
		ilustrativa del resultado de ambos detectores.
		
	![image](https://user-images.githubusercontent.com/91891270/144232305-d2ce3956-10bc-4a81-ba22-cfb6394483b7.png)

  
  * Optimice los parámetros de su sistema de detección de pitch e inserte una tabla con las tasas de error
    y el *score* TOTAL proporcionados por `pitch_evaluate` en la evaluación de la base de datos 
	`pitch_db/train`..

   * Inserte una gráfica en la que se vea con claridad el resultado de su detector de pitch junto al del
     detector de Wavesurfer. Aunque puede usarse Wavesurfer para obtener la representación, se valorará
	 el uso de alternativas de mayor calidad (particularmente Python)
	 
	 Vemos que el resultado es correcto ya que la presencia de pitch (tramas sordas y sonoras) se refleja en ambas representaciones y que la envolvente que caracteritza la evolución del pitch es similar en ambos casos.
	 
	 ![image](https://user-images.githubusercontent.com/91891270/144232401-42d8cb22-4ade-4ed9-82ea-8f259b38e73b.png)


Ejercicios de ampliación
------------------------

- Usando la librería `docopt_cpp`, modifique el fichero `get_pitch.cpp` para incorporar los parámetros del
  detector a los argumentos de la línea de comandos.
  
  Esta técnica le resultará especialmente útil para optimizar los parámetros del detector. Recuerde que
  una parte importante de la evaluación recaerá en el resultado obtenido en la detección de pitch en la
  base de datos.

  * Inserte un *pantallazo* en el que se vea el mensaje de ayuda del programa y un ejemplo de utilización
    con los argumentos añadidos.
   
   ![image](https://user-images.githubusercontent.com/91891270/144266784-ca821f89-832f-4d0f-8bb1-77f5a85ea496.png)

    
    Se han añadido los siguientes parámetros para poder introducirlos por terminal:
       - Umbrales l1 y l2 de permiten fijar los márgenes del central clipping.
       - Umbrales threshold1 i threshold2 para fijar los margenes de los ratios rnorm i rmaxnorm.
       - Umbrales threshold3 i threshold3 para fijar los margenes de las features.


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
    
    Avaluación con algoritmo de AMDF:
    
    ![image](https://user-images.githubusercontent.com/91891270/144268417-0f3bbbb1-0caa-4dba-8615-113ac84c7167.png)   
    
    Avaluación con algoritmo de autocorrelación:
    
    ![image](https://user-images.githubusercontent.com/91891270/144268068-62ba2b6e-9971-451c-8723-f755e56dac96.png)

    
  * Optimización **demostrable** de los parámetros que gobiernan el detector, en concreto, de los que
    gobiernan la decisión sonoro/sordo.
    
    Para la detección de sonidos sonoros o sordos hemos utilizado los siguientes parámetros:
       - Potencia de la señal .
       - Cociente de la autocorrelación r1norm = r[1] / r[0].
       - Cociente de la autocorrelación rmaxnor = r[pitch] / r[0].
       - Cruces por cero ZCR.
    
    El algoritmo es el sigueinte:
    
    ![image](https://user-images.githubusercontent.com/91891270/144267259-9dbb5436-7bc5-49e2-93c5-bd5edf66afec.png)

    
    Debemos tener en cuenta que el algoritmo de detección de tramas sordas y sonoras es bastante más simple que el que se realizó
     en la práctica anterior (VAD), por lo que consideramos que el porcentage final es bastante bueno. Quizá hubiese sido una buena opción incorporar el algortimo del VAD en esta práctica, pero lo hemos intentado hacer y se nos ha liado la cosa.  
    
  * Cualquier otra técnica que se le pueda ocurrir o encuentre en la literatura.

Para mejorar definitivamente el sistema y usar la implementación del AMDF creemos que seria útil hacer un filtrage previo mediante la herramienta sox. Eso nos permetiria eliminar ruido i mejorar el resultado de la detección mediante AMDF. Sin embargo este último preprocesado no se ha implementado debido a la dificultad de usar sox en múltiples ficheros wav. Se deberia realitzar un script que permitiera el filtrado completo de toda la base de datos.

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
