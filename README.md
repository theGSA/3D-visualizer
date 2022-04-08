![image](https://user-images.githubusercontent.com/40173977/162535359-6408329f-95ef-4633-bb57-e24b69056aa7.png)


# 3D-visualizer
Programa criado para obtenção da terceira nota de Computação Grafica.
o programa carrega arquivos do tipo obj e projeta a visualização 3d do modelo.

# libs externas
- imgui (interface gráfica, botões etc..)
- SOIL2 (carregar arquivos de imagem (jpg, png e etc..))
# suporte
- carrega arquivos .obj
- Rotação
- Translação
- Escala
- Wireframe mode
- Texturas
- Iluminação

# compilação
- o programa foi compilado usando Visual Studio 2019 v.142
- abra o arquivo Visualizer3D.sln e compile

# mode de uso
- abra o cmd na pasta onde está o Visualizer3D.exe
- e digite o comando sem aspas "Visualizer3D.exe ./pasta/onde/esta/nome_do_objeto.obj
- as texturas devem seguir o expecificado pelo .mtl do .obj
  no entando na pasta onde está o nome_do_arquivo.obj deve ter uma pasta texturas
- se não houver linha de comando o programa abre um modelo 3d na pasta Models (leon.obj)
