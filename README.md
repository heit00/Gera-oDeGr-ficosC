Por padrão 2 funções já estao desenhadas (basta compilar o código) 1- y = sqrt(900 - x^2) 2-  y = -sqrt(900 - x^2). 
Voce pode altera-las por meio das funções function1(x:int):float e funtion2(x:int):float.
Você pode ainda criar mais e passalas via parametro para drawFunctionDefault(f:double (*f)(double)):void onde f é um ponteiro de mesmo nome para a funçaõ que voce criou.
