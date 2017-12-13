#include "anasin.h"

void erroSintatico(char erro[]){

    printf("\nERRO SINT�TICO na Linha %d: %s", linhas, erro);

    system("pause");
    exit(1);
}

void erroSemantico(char erro[]){

    printf("\nERRO SEMANTICO na Linha %d: %s", linhas, erro);

    system("pause");
    exit(1);
}

/*OK*/
int opr_rel(){
    if(tk.categoria == SN)
    {
        if(tk.cod == COMPARA || tk.cod == DIFERENTE || tk.cod == MAIOR_IG || tk.cod == MAIOR_Q || tk.cod == MENOR_IG || tk.cod == MENOR_Q)
        {
            return 1;
        }else{
            erroSintatico("Esperado operador relacional");
        }
    }else{
        erroSintatico("Esperado sinal");
    }

    return 0;
}

/*OK*/
void termo(){

    fator();

    //Se o proximo token for um sinal
    if(tknext.categoria == SN && (tknext.cod == MULT || tknext.cod == DIV || tknext.cod == AND)){


            analex();//vai pro sinal

            while(1){
                analex();
                fator();
                if(!(tknext.cod == MULT || tknext.cod == DIV || tknext.cod == AND)){
                    break;
                }
                analex(); //vai pro sinal

            }//fim-while


    }//fimSe o proximo token for um sinal

    return;
}

/*OK*/
int fator(){
    /*Se for Inteiro, Real ou Caractere*/
    if(tk.categoria == CT_I || tk.categoria == CT_R || tk.categoria == CT_C  || tk.categoria == CT_LT){
        printf("\nSomente CTI, CTR, CTC OU CTLT");
        return 1;
    }

    /*Se for ID*/
    else if(tk.categoria == ID){

         printf("\nLeu id");

        if(!(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)){
            //Se for somente ID
            printf("\nSomente id");
            return 1;
        }

        printf("\nnao eh somente id");

        //Se for abrir parentesis
        if(tknext.categoria == SN && tknext.cod ==  PARENTESIS_ABRE){

            //vai pro parentesis
            analex();

            printf("\n ta no abre parentesis");
            //Se o pr�ximo token for fecha parentesis
            if(tknext.categoria == SN && tknext.cod ==  PARENTESIS_FECHA){
                //Ve o proximo token e sai
                printf("\n nada dentro do parentesis");
                analex();
                return 1;
            }

            //Chama a fun��o de expressão
            analex();
            expr();

            //Se o proximo token for um sinal E Se esse proximo token for VIRG
            if(tknext.categoria == SN && tknext.cod == VIRG){
                analex(); //vai pra virgula

                //vai chamando expr
                while(1){
                    //vai pra expr
                    analex();
                    expr();

                    if(!(tknext.categoria == SN && tknext.cod == VIRG)){
                        break;
                    }
                    analex(); //vai pra virgula

                }//fim-while
            }//fimSe o proximo token for um sinal E Se esse proximo token for VIRG

            //If para checar se houve fechar parentesis
            if(!(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)){
                //Erro faltando parentesis
                erroSintatico("Falta fecha parentesis");
            }
            //Se for fecha parentesis
            else{
                //Vai pro fecha parentesis
                analex();
                return 1;
            }

        }//FIM-Se for abrir parentesis

    }//Fim-Se for ID

    /* Se for somente ( expr ) */
    //Checar se houve abre parentesis
    else if(tk.categoria == SN && tk.cod ==  PARENTESIS_ABRE){
        analex();
        expr();

        //If para checar se houve fechar parentesis
        if(!(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)){
            //Erro faltando parentesis
            erroSintatico("Falta fecha parentesis");
        }
        //Se for fecha parentesis
        else{
            //Vai pro fecha parentesis
            analex();
            return 1;
        }

    }

    /* Se for nega��o de express�o */
    else if(tk.categoria == SN && tk.cod == NEGACAO){
        printf("\eh negacao");
        analex();
        fator();
    }
    else{
        erroSintatico("Era esperador fator");
    }

    return 0;

}

/*OK*/
void expr(){

    printf("\ncheguei em expr");

    expr_simp();

    if(tknext.categoria == SN){
        //se o proximo token for op relacional
        if(tknext.cod == COMPARA || tknext.cod == DIFERENTE || tknext.cod == MAIOR_Q || tknext.cod == MENOR_Q || tknext.cod == MAIOR_IG || tknext.cod == MENOR_IG){
            analex(); //vai pro op_rel
            opr_rel(); //confirma op_rel

            analex();
            expr_simp();
        }//fim-se o proximo token for op relacional
        else{
            //Erro, esperando operador relacional
            //erroSintatico("Falta operador relacional");
        }
    }
}

/*OK*/
void expr_simp(){

    /*Se o termo come�ar com + ou - */
    if(tk.categoria == SN && (tk.cod == SOMA || tk.cod == SUB)){
        analex();
    }

    termo();

    if(tknext.categoria == SN && (tknext.cod == SOMA || tknext.cod == SUB || tknext.cod == OR)){
        analex(); //vai pro sinal
        while(1){
            analex();
            termo();
            if(!(tknext.categoria == SN && (tknext.cod == SOMA || tknext.cod == SUB || tknext.cod == OR))){
                break;
            }
            analex();
        }//fim-while

    }

}

/*OK*/
int tipo(){

    //Se for palavra reservada
    if(tk.categoria == PR){

        if(tk.cod == CARACTER){
            return tk.cod;
        }else if(tk.cod == INTEIRO){
            return tk.cod;
        }else if(tk.cod == REAL){
            return tk.cod;
        }else if(tk.cod == BOOLEANO){
            return tk.cod;
        }else{
            return -1;
        }

    }//FIM-se for PR
    return -1;
}

/*OK*/
void tipos_param(){

    int guardarTipo;

    //Se for palavra reservada
    if(tk.categoria == PR && tk.cod == SEMPARAM){
        //Se a palavra reservada for semparam
        return;
    }
    else{
        //SE FOR TIPO
        guardarTipo = tipo();
        if(guardarTipo>=0){
        analex();

            //se for id
            if(tk.categoria == ID){

                //Se não houver o ID na tabela, ele insere
                if(!controlador_TabSimb(CONSULTARPARAM, tk.lexema, 0, LOCAL, 0, 0)){
                    controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, PARAM, SIM_ZUMBI);

                    //E Se esse proximo token for VIRG
                    if(tknext.categoria == SN && tknext.cod == VIRG){
                        analex();
                        while(1){
                            analex();
                            //Se for tipo
                            guardarTipo = tipo();
                            if(guardarTipo>=0){
                                analex();
                                //Se for ID
                                if(tk.categoria == ID){
                                    if(!controlador_TabSimb(CONSULTARPARAM, tk.lexema, 0, LOCAL, 0, 0)){
                                        controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, PARAM, SIM_ZUMBI);
                                    }else{
                                        erroSintatico("ID j� existente");
                                    }
                                }//Fim-Se for ID
                                else{
                                    erroSintatico("ID inv�lido");
                                }

                            }//Fim-Se for tipo
                            else{
                                erroSintatico("Tipo inv�lido");
                            }


                            if(!(tknext.categoria == SN && tknext.cod == VIRG)){
                                break;
                            }
                            analex();

                        }//fim-while

                    }//fim-E Se esse proximo token for VIRG
                    else if(tknext.categoria == ID || tknext.categoria == PR){
                        erroSintatico("� esperado virgula");
                    }

                }else{
                    //Erro id já existente na tabela
                    erroSintatico("ID j� existente na tabela");
                }

            }//fim-se for id
            else{
                //Esperado id
                erroSintatico("Era esperado ID");
            }

        }//fim-tipo
        else{
            //erro sintático TIPO INVALIDO
            erroSintatico("Tipo invalido");
        }

    }//FIM-ELSE PR

}

/*OK*/
void tipos_p_opc(){
    int guardarTipo;

    //Se for palavra reservada
    if(tk.categoria == PR && tk.cod == SEMPARAM){
        //Se a palavra reservada for semparam
        return;
    }
    else{
        //SE FOR TIPO
        guardarTipo = tipo();
        if(guardarTipo>=0){

            //Se o pr�ximo token for ID
            if(tknext.categoria == ID){
                analex();//to no id
                if(!controlador_TabSimb(CONSULTARPARAM, tk.lexema, 0, LOCAL, 0, 0)){
                    controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, PARAM, SIM_ZUMBI);
                }
                else{
                    erroSintatico("ID j� existente");
                }
            }//fim-se for id

            //Se o pr�ximo token for virgula
            while(tknext.categoria == SN && tknext.cod == VIRG){
                analex();//ta na virgula
                analex();//ta no tipo
                guardarTipo = tipo();
                if(guardarTipo>=0){

                    //Se o pr�ximo token for ID
                    if(tknext.categoria == ID){
                        analex();//to no id
                        if(!controlador_TabSimb(CONSULTARPARAM, tk.lexema, 0, LOCAL, 0, 0)){
                            controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, PARAM, SIM_ZUMBI);
                        }
                        else{
                            erroSintatico("ID j� existente");
                        }
                    }//fim-se for id

                }//fim-se for tipo2
                else{
                    erroSintatico("Tipo invalido!");
                }
            }//fim-while



        }//fim-tipo
        else{
            //erro sintático TIPO INVALIDO
            erroSintatico("Tipo invalido");
        }

    }//FIM-ELSE PR

}

/*OK*/
void atrib(){

    //Se  for ID
    if(tk.categoria == ID){
        analex();

        //Se for um sinal de atribui��o
        if(tk.categoria == SN && tk.cod == ATRIB){

            analex();
            expr();

        }//fim-Se for um sinal de atribui��o
        else{
            erroSintatico("� esperado um sinal de atribui��o");
        }

    }//fim- se for id
    else{
        erroSintatico("� esperado um ID");
    }

}

/*OK*/
void cmd(){
    if(tk.categoria == PR || tk.categoria == ID || tk.categoria == SN){

        //Se for sinal
        if(tk.categoria == SN){

            switch(tk.cod){

                case PT_VIRG:
                    break;

                case CHAVES_ABRE:
                    while(1){
                        if((tknext.categoria == SN && tknext.cod==CHAVES_FECHA)){
                            analex();
                            break;
                        }
                        analex();
                        cmd();
                    }
                    break;

                default:
                    /*ERRO DE CATEGORIA*/
                    erroSintatico("Sinal inv�lido! � esperado { ou ;");

            }//fim-switch

        }//if SN

        //Se for palavra reservada
        else if(tk.categoria == PR){

            switch(tk.cod){

                case SE:
                    analex();
                    if(tk.categoria == SN && tk.cod == PARENTESIS_ABRE){
                        analex(); /*OBSERVAR AQUI*/
                        expr();
                        analex();
                        if(tk.categoria == SN && tk.cod == PARENTESIS_FECHA){
                            analex();
                            cmd();

                            //Saiu de CMD
                            //Se o pr�ximo token for SENAO
                            if(tknext.categoria == PR && tknext.cod == SENAO){
                                analex(); //vai estar em senao
                                analex();
                                cmd();
                            }

                        }//if PARENTESIS_FECHA
                        else{
                            //Erro fecha parentesis esperado
                            erroSintatico("Esperado )");
                        }

                    }//if PARENTESIS_ABRE
                    else {
                        /*ERRO DE N�O ABERTURA DE PARENT�SE*/
                        erroSintatico("Esperado (");
                    }
                    break;
                case ENQUANTO:
                    analex();
                    if(tk.categoria == SN && tk.cod == PARENTESIS_ABRE){
                        analex();
                        expr();
                        analex();
                        if(tk.categoria == SN && tk.cod == PARENTESIS_FECHA){
                            analex();
                            cmd();
                        }else{

                            /*ESPERADO FECHA PARENTESE*/
                            erroSintatico("Esperado )");
                        }
                    }else {

                        /*ESPERADO ABRE PARENTESE*/
                        erroSintatico("Esperado (");
                    }
                    break;

                case PARA:
                    analex();
                    //Se for parentesis
                    if(tk.categoria == SN && tk.cod == PARENTESIS_ABRE){

                        //Se o proximo token for ID, ent�o ocorrer� a atribui��o 1
                        if(tknext.categoria == ID){
                            analex();
                            atrib();
                        }//-Fim possivel atrib 1

                        //Verifica se o pr�ximo token � PT_VIRG 1
                        if(tknext.categoria == SN && tknext.cod == PT_VIRG){
                            analex();//to no pt_virg 1

                            //Verifica se pr�ximo token � PT_VIRG 2
                            //Se for, n�o ocorrer� expr
                            //Se n�o for, pode ocorrer expr
                            if(tknext.categoria == SN && tknext.cod == PT_VIRG){
                                analex(); //to no pt_virg 2

                                //Se o proximo token for ID, ent�o ocorrer� a atribui��o 2
                                if(tknext.categoria == ID){
                                    analex();
                                    atrib();

                                    if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA){
                                        analex();//ta no parentesis
                                        analex();//prepatativo pro cmd
                                        cmd();
                                    }else{
                                        erroSintatico("� esperado FECHA PARENTESIS1");
                                    }



                                }//-Fim possivel atrib 2
                                else if(!(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)){
                                    //sob observa��o
                                    erroSintatico("� esperado FECHA PARENTESIS3");
                                }


                            }//FIM-SE FOR PT_VIRG 2
                            //Caso n�o for PTVIRG, pode ocorrer expr
                            else{
                                analex();
                                expr();

                                //� esperado PT_VIRG 2 APOS EXPR
                                if(tknext.categoria == SN && tknext.cod == PT_VIRG){
                                    analex(); //to no pt_virg 2

                                    //Se o proximo token for ID, ent�o ocorrer� a atribui��o 2
                                    if(tknext.categoria == ID){
                                        analex();
                                        atrib();

                                        if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA){
                                            analex();//ta no parentesis
                                            analex();//prepatativo pro cmd
                                            cmd();
                                        }else{
                                            erroSintatico("� esperado FECHA PARENTESIS2");
                                        }
                                    }else if(!(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)){
                                        //sob observa��o
                                        erroSintatico("� esperado FECHA PARENTESIS4");
                                    }


                                }//fIM-ESPERADO PT_VIRG2 APOS EXPR
                                else{
                                    erroSintatico("� esperado PT_VIRG");
                                }

                            }//fim-pode ocorrer expr

                        }//FIM-SE FOR PT_VIRG 1
                        else{
                            erroSintatico("� esperado PT_VIRG");
                        }


                    }//Fim- se for parentesis
                    else{
                        erroSintatico("� esperado PARENTESIS_ABRE");
                    }

                    break;

                case RETORNE:
                    if(tknext.categoria == SN && tknext.cod == PT_VIRG){
                        analex();
                        break;
                    }

                    //Se n�o entrou no if do pt e virg
                    analex();
                    expr();
                    analex();
                    if(tk.categoria == SN && tk.cod == PT_VIRG){
                        break;
                    }else{
                        erroSintatico("Falta PT_VIRG ;");
                    }
                    break;

                default:
                    /*ERRO de comando n�o esperado*/
                    erroSintatico("Cmd invalido");


            }//switch


        }//if PR

        //SE FOR ID
        else if(tk.categoria == ID){

            //olha o proximo token, se for atribui��o
            //temos uma atribui��o
            if(tknext.categoria == SN && tknext.cod == ATRIB){
                atrib(); //n�o chama analex pq ele j� vai t� no id
                analex();
                if(tk.categoria == SN && tk.cod == PT_VIRG){
                    return;
                }else{
                    erroSintatico("Falta PT_VIRG ; ap�s atribui��o");
                }

            }//fim se for atrib


            //temos um id([expr{,expr}]);
            else if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE){
                //Ta no id

                analex(); //passa pro pr�ximo token

                //Se o pr�ximo token for fecha parentesis
                if(tknext.categoria == SN && tknext.cod ==  PARENTESIS_FECHA){
                    analex();
                    //O proximo token tem que ser ponto e virgula

                    if(tknext.categoria == SN && tknext.cod == PT_VIRG){
                        analex();
                        return;
                    }else{
                        //Se n�o for, d� erro
                        erroSintatico("Falta PT_VIRG");
                    }
                }


                //Chama a fun��o de expressão
                analex();
                expr();

                //Se o proximo token for uma virgula
                if(tknext.categoria == SN && tknext.cod == VIRG){
                    analex();
                    while(1){
                        analex();
                        expr();
                        if(!(tknext.categoria == SN && tknext.cod == VIRG)){
                            break;
                        }
                        analex();

                    }//fim-while
                }//fimSe o proximo token for uma virgula

                analex();
                //If para checar se houve fechar parentesis
                if((tk.categoria == SN && tk.cod == PARENTESIS_FECHA)){
                    //Agora vai ver se o proximo token � pt_virg
                    if(tknext.categoria == SN && tknext.cod == PT_VIRG){
                        analex();
                        return;
                    }//fim-se � pt_virg
                    else{
                        //n�o encontrou pt_virg
                        erroSintatico("Falta PT_VIRG");
                    }
                }//If encontrou fecha parentesis
                else{
                    //Erro faltando parentesis
                    erroSintatico("Falta fecha parentesis");
                }

                return;

            }////temos um id([expr{,expr}]);
            else{
                erroSintatico("Esperado cmd");
            }

        }//fim- if id
        else{
            erroSintatico("Cmd inv�lido");
        }


    }//If PR ou ID ou SN
    else{
        erroSintatico("Cmd inv�lido");
    }
}//void
/*Obs: se der erro em cmd, provavelmente seja por conta dos dois ultimos elses ;*/

void prog(){
    int guardarTipo;
    //analex();
    if(tk.categoria == PR && tipo() < 0)// if 1
    {
        printf("PALAVRA RESERVADA \n");
       if(tk.cod == PROTOTIPO)// if prototipo
       {
            printf("PROTOTIPO \n");
            if(tknext.categoria == PR && tknext.cod == SEMRETORNO)// if 2
            {
                    printf("SEMRETORNO \n");
                    analex();
                    if(tknext.categoria == ID)// if 3
                    {
                        printf("ID \n");
                        analex();
                        if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, GLOBAL, 0, 0))// if 10
                        {
                            controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, GLOBAL, PARAM, SIM_ZUMBI);

                            if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)// if 4
                            {
                                printf("PARENTESI ABRE \n");
                                analex();// vai para o parentesi
                                analex();// vai para o tipos_p_opc
                                printf("TIPOS_O_OPC \n");
                                tipos_p_opc();
                                if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)// if 5
                                {
                                    printf("PARENTESI FECHA\n");
                                    analex();
                                        while(tknext.categoria == SN && tknext.cod == VIRG)// while VIRG
                                        {
                                                printf("VIRG \n");
                                                analex();// vai para VIRG
                                                if(tknext.categoria == ID)// if 40
                                                {
                                                    printf("ID");
                                                            analex();
                                                            if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0))// if 11
                                                            {
                                                                controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, LOCAL, PARAM, SIM_ZUMBI);
                                                                 /*if(tknext.categoria == ID)// if 6
                                                                {

                                                                    analex();*/
                                                                    if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)// if 7
                                                                    {
                                                                        printf("PARENTESIS_ABRE\n");
                                                                        analex();// vai para o abre parentese
                                                                        analex();// vai para o tipos_p_opc
                                                                        printf("tipos_p_opc\n");
                                                                        tipos_p_opc();
                                                                        if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)// if 8
                                                                        {
                                                                            printf("PARENTESIS_FECHA\n");
                                                                            analex();
                                                                        }else{
                                                                            erroSintatico("Esperava-se um fecha parentesi");
                                                                        }// fecha else do if 8
                                                                    }else{
                                                                            erroSintatico("Esperava-se um abre parentesi");
                                                                        }//fecha else do if 7

                                                            /*}// fecha if 6*/
                                                        }else{
                                                            erroSintatico("Dupla declaracao");
                                                        }// fim do else do if 11

                                                }// fim do if 40

                                    }// while VIRG

                                    //analex();
                                    if(tknext.categoria == SN && tknext.cod == PT_VIRG)// if 9
                                    {
                                        printf("PT_VIRG \n");
                                        analex();
                                    }
                                    else{
                                        erroSintatico("Esperava-se um ponto virgula");
                                    }// fecha else do if 9

                                }else
                                {
                                    erroSintatico("Esperava-se um fecha parentesi");
                                }// fecha else if 5
                            }else
                            {
                                erroSintatico("Esperava-se um abre parentesi");
                            }// fecha do if 4

                        } else // do if 10
                        {
                            erroSintatico("Dupla declaracao");
                        }// fecha else do if 10

                    }// fecha do if 3

                } else // else do if 2
                {
                    analex();
                    printf("Tipo\n");
                    tipo();
                    if(tknext.categoria == ID)// if 3
                    {
                        analex();
                         printf("ID\n");
                        if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, GLOBAL, 0, 0))// if 10
                        {
                            controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, GLOBAL, PARAM, SIM_ZUMBI);

                            if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)// if 4
                            {
                                 printf("PARENTESIS_ABRE\n");
                                analex();// vai para o abre parentesi
                                analex();// vai para o tipos_p_opc
                                printf("TIPOS_P_OPC\n");
                                tipos_p_opc();
                                if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)// if 5
                                {
                                    analex();
                                    printf("PARENTESIS_FECHA\n");
                                    while(tknext.categoria == SN && tknext.cod == VIRG)
                                    {
                                        printf("VIRG \n");
                                        analex();
                                        if(tknext.categoria == ID)// if 45.
                                        {
                                            printf("ID\n");
                                            analex();
                                            if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, GLOBAL, 0, 0))// if 11
                                                {
                                                    controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, GLOBAL, PARAM, SIM_ZUMBI);
                                                    /* if(tknext.categoria == ID)// if 6
                                                    {

                                                        analex();*/
                                                        if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)// if 7
                                                        {
                                                            printf("PARENTESIS_ABRE \n");
                                                            analex();// vai para o abre parentese
                                                            analex();// vai para o tipos_p_opc
                                                            tipos_p_opc();
                                                            if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)// if 8
                                                            {
                                                                printf("PARENTESIS_FECHA \n");
                                                                analex();
                                                            }else{
                                                                erroSintatico("Esperava-se um fecha parentesi");
                                                            }// fecha else do if 8
                                                        }else{
                                                                erroSintatico("Esperava-se um abre parentesi");
                                                            }//fecha else do if 7

                                               /* }// fecha if 6*/
                                            }else{
                                                erroSintatico("Dupla declaracao");
                                            }// fim do else do if 11

                                        }// fecha if 45.


                                }// while parentese

                                    //analex();
                                    if(tknext.categoria == SN && tknext.cod == PT_VIRG)// if 9
                                    {
                                        analex();
                                    }
                                    else{
                                        erroSintatico("Esperava-se um ponto virgula");
                                    }// fecha else do if 9
                                }else
                                {
                                    erroSintatico("Esperava-se um fecha parentesi");
                                }// fecha else if 5
                            }else
                            {
                                erroSintatico("Esperava-se um abre parentesi");
                            }// fecha do if 4

                        } else // do if 10
                        {
                            erroSintatico("Dupla declaracao");
                        }// fecha else do if 10

                    }// fecha do if 3
                }// fecha else do if 2

       }else if(tk.cod == SEMRETORNO)// fecha if prototipo
       {

                printf("SEM RETORNO \n");
                if(tknext.categoria == ID )// if 20
                {
                    printf("ID \n");
                    analex();
                    if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, GLOBAL, 0, 0))// if 21
                      {
                           controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, GLOBAL, PARAM, SIM_ZUMBI);
                           if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)// if 23
                            {
                                printf("ABRE PARENTESI \n");
                                analex();// vai para o parentesi
                                printf("TIPOS_PARM \n");
                                analex();//vai para o tipos_param
                                tipos_param();
                              if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)// if 24
                               {


                                    printf("PAARENTESI FECHA \n");
                                    analex();
                                    //Espera pelo abre chaves
                                            if(tknext.categoria == SN && tknext.cod == CHAVES_ABRE){
                                                analex(); //ta no abre chaves

                                                //Se depois vier um fecha chaves, retorna
                                                if(tknext.categoria == SN && tknext.cod == CHAVES_FECHA){
                                                    analex();
                                                     printf("\n{}");
                                                    return;
                                                }
                                                //Enquanto tnext for tipo
                                                while(tknext.categoria == PR && (tknext.cod == CARACTER || tknext.cod == INTEIRO || tknext.cod == REAL ||tknext.cod == BOOLEANO)){
                                                    printf("\nEntrei no while");
                                                    analex();//ta no tipo
                                                    guardarTipo = tipo();
                                                    printf("\nTo no tipo");
                                                    //Se o pr�ximo token for ID
                                                    if(tknext.categoria == ID){
                                                        analex();//t� no id

                                                        //Insere ID na tabela
                                                        if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0)){
                                                            controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, VAR, NAO_ZUMBI);
                                                        }else{
                                                            erroSintatico("ID j� existente");
                                                        }

                                                        printf("\nTo no id");

                                                        //Enquanto TNEXT for v�rgula
                                                        while(tknext.categoria == SN && tknext.cod == VIRG){
                                                            printf("\nEntrei no while2");
                                                            analex();//Pra pegar a virgula
                                                            if(tknext.categoria != ID){
                                                                erroSintatico("Eh esperado ID ap�s virgula");
                                                            }
                                                            analex();//pra pegar o ID

                                                            //Insere ID na tabela
                                                            if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0)){
                                                                controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, VAR, NAO_ZUMBI);
                                                            }else{
                                                                erroSintatico("ID j� existente");
                                                            }

                                                            if(!(tknext.categoria == SN && tknext.cod == VIRG) && !(tknext.categoria == SN && tknext.cod == PT_VIRG)){
                                                                erroSintatico("Eh esperado virgula ou PT_VIRG apos id");
                                                            }
                                                        }

                                                        if(!(tknext.categoria == SN && tknext.cod == PT_VIRG)){
                                                            erroAnalisadorLexico("� esperado PT_VIRG");
                                                        }
                                                        analex(); //pergou pt e virgula
                                                        printf("\nPeguei o ponto e virgula");

                                                    }//Fim-se tknext for id
                                                    else{
                                                        erroAnalisadorLexico("Ap�s tipo � esperado ID");
                                                    }


                                                }//fim-Enquanto tnext for tipo

                                                //chamar CMD
                                                printf("\nTalvez ocorra cmd");
                                                while(!(tknext.categoria == SN && tknext.cod == CHAVES_FECHA)){
                                                    analex();
                                                    printf("\nvai ocorrer cmd");
                                                    cmd();
                                                }

                                                //Apos encontrar o fecha chaves, desempilha a tabela de sinais
                                                desempilhar();

                                            }

                                   // }






                            }else// else do if 24
                                {
                                    erroSintatico("Esperava-se um fecha parentesi");
                                }// fecha else do if 24
                            }else// else do if 23
                                {
                                    erroSintatico("Esperava-se um abre parentesi");
                                }//fim do else do if 23
                      }//fim do if 21
                }//fecha if 20

       }// fecha else if prototipo


    }else if(tipo() >= 0)//else if do if 1
    {
            printf("TIPO \n");
            if(tknext.categoria == ID )// if 72
            {
                 printf("ID \n");
                 analex();
                if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, GLOBAL, 0, 0))// if 71
                 {
                    controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, GLOBAL, PARAM, SIM_ZUMBI);
                    if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)// if 73
                    {
                        analex();
                        analex();
                        tipos_param();
                        if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)// if 77
                        {
                            analex();
                            //Espera pelo abre chaves
                            if(tknext.categoria == SN && tknext.cod == CHAVES_ABRE){
                                analex(); //ta no abre chaves

                                //Se depois vier um fecha chaves, retorna
                                if(tknext.categoria == SN && tknext.cod == CHAVES_FECHA){
                                    analex();
                                     printf("\n{}");
                                    return;
                                }
                                //Enquanto tnext for tipo
                                while(tknext.categoria == PR && (tknext.cod == CARACTER || tknext.cod == INTEIRO || tknext.cod == REAL ||tknext.cod == BOOLEANO)){
                                    printf("\nEntrei no while");
                                    analex();//ta no tipo
                                    guardarTipo = tipo();
                                    printf("\nTo no tipo");
                                    //Se o pr�ximo token for ID
                                    if(tknext.categoria == ID){
                                        analex();//t� no id

                                        //Insere ID na tabela
                                        if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0)){
                                            controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, VAR, NAO_ZUMBI);
                                        }else{
                                            erroSintatico("ID j� existente");
                                        }

                                        printf("\nTo no id");

                                        //Enquanto TNEXT for v�rgula
                                        while(tknext.categoria == SN && tknext.cod == VIRG){
                                            printf("\nEntrei no while2");
                                            analex();//Pra pegar a virgula
                                            if(tknext.categoria != ID){
                                                erroSintatico("Eh esperado ID ap�s virgula");
                                            }
                                            analex();//pra pegar o ID

                                            //Insere ID na tabela
                                            if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0)){
                                                controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, VAR, NAO_ZUMBI);
                                            }else{
                                                erroSintatico("ID j� existente");
                                            }

                                            if(!(tknext.categoria == SN && tknext.cod == VIRG) && !(tknext.categoria == SN && tknext.cod == PT_VIRG)){
                                                erroSintatico("Eh esperado virgula ou PT_VIRG apos id");
                                            }
                                        }

                                        if(!(tknext.categoria == SN && tknext.cod == PT_VIRG)){
                                            erroAnalisadorLexico("� esperado PT_VIRG");
                                        }
                                        analex(); //pergou pt e virgula
                                        printf("\nPeguei o ponto e virgula");

                                    }//Fim-se tknext for id
                                    else{
                                        erroAnalisadorLexico("Ap�s tipo � esperado ID");
                                    }


                                }//fim-Enquanto tnext for tipo

                                //chamar CMD
                                printf("\nTalvez ocorra cmd");
                                while(!(tknext.categoria == SN && tknext.cod == CHAVES_FECHA)){
                                    analex();
                                    printf("\nvai ocorrer cmd");
                                    cmd();
                                }

                                //Apos encontrar o fecha chaves, desempilha a tabela de sinais
                                desempilhar();

                            }


                        }// fecha if 77

                    }else
                    {
                       // printf("Vim no while");
                        while(tknext.categoria == SN && tknext.cod == VIRG)// while 6
                        {
                            printf("VIRG c \n");
                            analex();
                            if(tknext.categoria == ID)
                            {
                                printf("ID \n");
                                analex();
                                 if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, GLOBAL, 0, 0))// if 71
                                 {
                                    controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, GLOBAL, PARAM, SIM_ZUMBI);
                                }else// if 71
                                {
                                    erroSintatico("Dupla decrara��o");
                                }// if 71
                            }

                        }// while 6


                         if(tknext.categoria == SN && tknext.cod == PT_VIRG)// if 74
                           {
                               printf("PT_VIRG \n");
                                  analex();

                           }else // if 74
                           {
                                erroSintatico("Esperava-se um ponto virgula");
                           }// fecha else do if 74



                    }// if 73

                 }else// if 71
                {
                    erroSintatico("Dupla decrara��o");
                }// if 71
            }// if 72

    }else// else do else if 1
    {
        printf("VAZIO \n");
        //return;
    }// fecha else do else if 1

}// fecha prog


void anasin(){
    iniciarTabelaDeSimbolos();

    analex();
    analex();

    //enquanto n�o for fim de arquivo
    while(!feof(arquivo)){
        prog();
        analex();
        printf("\nLinha: %d",linhas);
    }//fim-enquanto n�o for fim de arquivo
}

int main(){
    char nomeArquivo[1000];

    printf("\n\tDigite o nome do arquivo fonte: "); scanf("%s", nomeArquivo);

    /*ABRE O ARQUIVO*/
	if ( (arquivo = fopen(nomeArquivo,"r")) != NULL ){
        printf("\n\tArquivo aberto com sucesso!\n");
        iniciarTabelaDeSimbolos();

        anasin();

        fclose(arquivo);
    }
    else{
        printf("\n\tN�o foi poss�vel abrir o arquivo %s\n", nomeArquivo);
        system("pause");
        exit(1);
    }
    /*FIM-ABRE O ARQUIVO*/

    printf("\n");
    system("pause");
    return 0;
}

