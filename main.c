#include <stdio.h>
#include <ncurses.h>
#include <string.h>

void cuenta();
void catalogo();
void carrito();
void imprimeTxt(char[100]);
void iniciarSesion();
void soyProveedor();
void cerrarSesion();
void detallesCuenta();
void menuProveedor();
void agregarProducto();
void cambiarClave();
void cambiarNombre();
void crearCuenta();
void msgNoIniciado();
void msgActualmenteIniciado();
void msgNoExisteUsuario();
int existeUsu(char [100]);
int codigoValido(char [100],char [100]);
void msgContraErronea();
void agregarCatalogo(char [100],char [100],int);
void eliminarCatalogo(char [100],char [100]);
void eliminarProducto();
int estaEnTxt(char [100], char [100]);
char* obtenPrecio(int );
char* obtenProveedor(int );
char* obtenNombre(int );



WINDOW *mframe;
int y_max,x_max, sesion = 0;
char usuActual[100];
char obpre[100];
char obnom[100];
char obprov[100];
//Nota: crear variables compartidas para hacer uso de semaforos

//Checar funciones
//Main con menú principal
int main(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    //Obteniendo datos para el tamaño de ventana
    
    getmaxyx(stdscr,y_max, x_max);

    //iniciando ventana
    mframe = newwin(y_max-2, x_max-4, 1, 2); //main frame
    box(mframe,0,0); //Margen
    refresh();
    wrefresh(mframe);
    keypad(mframe, true); //entrada de teclado
    if(has_colors)
        start_color();

    init_pair(1,COLOR_CYAN, 0);

    char menu[3][100];
    int  opcion, marc = 0;
    strcpy(menu[0],"Carrito");
    strcpy(menu[1],"Productos");
    strcpy(menu[2],"Cuenta");

    
    while(1){
        wclear(mframe);
        wrefresh(mframe);
        box(mframe,0,0); //Margen
        
        wmove(mframe,3,3);
        mvwprintw(mframe, 5,3,"Tecla arriba para confirmar");
        mvwprintw(mframe, 6,3,"Tecla ENTER para salir de programa");
       

        for(int i = 0; i<3;i++){

            switch (marc)
            {
            case 0:
                mvwprintw(mframe, y_max/2,3,"Informacion acerca de los productos dentro del carrito");
                break;

            case 1:
                mvwprintw(mframe, y_max/2,3,"Mostrar el catalogo de productos disponible");
                break;

            case 2:
                mvwprintw(mframe, y_max/2,3,"Detalles de cuenta");
                break;
            
            default:
                break;
            }


            if(i == marc){
                wattron(mframe, A_REVERSE);
            }
        
            mvwprintw(mframe, 1,3+(i*15),menu[i]);
            wattroff(mframe, A_REVERSE);
            wrefresh(mframe);
        }
        opcion = wgetch(mframe);

        switch (opcion)
        {
        case KEY_RIGHT:
            if(marc <2)
                marc+=1;
            break;
        case KEY_LEFT:
            if(marc >0)
                marc-=1;
            break;
        default:
            break;
        }

        if(opcion == KEY_UP && marc == 0)
            carrito();
        else if(opcion == KEY_UP && marc == 1)
            catalogo();
        else if(opcion == KEY_UP && marc == 2)
            cuenta();    
    

        if(opcion == 10)
            break;
        
    }
    endwin();
    return 0;
}


void carrito(){
    if(sesion == 1){
        char usuario[100];
        int tecla;
        strcpy(usuario,usuActual);

        while(tecla != KEY_LEFT){
            wclear(mframe);
            wrefresh(mframe);
            box(mframe,0,0); //Margen
            

            wattron(mframe, A_REVERSE);
            mvwprintw(mframe,2,2,"CARRITO", usuario);
            wattroff(mframe, A_REVERSE);
            wrefresh(mframe);

            mvwprintw(mframe,2,20,"Hola %s", usuario);
            mvwprintw(mframe,y_max-4,4,"<- para regresar ||  -> para pagar", usuario);
            mvwprintw(mframe,4,2,"Articulos en el carrito:");
            char carritoUsuario[200];
            strcpy(carritoUsuario,usuario);
            strcat(carritoUsuario,"_carrito");
            imprimeTxt(carritoUsuario);

            wrefresh(mframe);
            tecla = wgetch(mframe);

            if(tecla == KEY_LEFT){
                return;
            }
            else if(tecla == KEY_RIGHT){
                wmove(mframe,7,2);
                wattron(mframe, A_REVERSE);
                mvwprintw(mframe, 6, 2, "ya esta todo pagadisimo :D          <- Menu || Seguir en carrito ->");
                wattroff(mframe, A_REVERSE);
                wrefresh(mframe);
                wgetch(mframe);
                FILE *txt;
                txt = fopen(carritoUsuario, "w+");
                fprintf(txt," ");
                fclose(txt);
            }

        }
        
        wrefresh(mframe);
    }
    else{
        msgNoIniciado();
    }
    return;
}

//Insertar opcion que pueda hacer dispplay de la info de todos los productos
void catalogo(){
    int opcion,producto;

    while(opcion != KEY_LEFT){
        wclear(mframe);
        box(mframe,0,0);
        wmove(mframe,2,2);
        wattron(mframe, A_REVERSE);
        mvwprintw(mframe, 2, 2, "Catalogo de productos");
        wattroff(mframe, A_REVERSE);

        mvwprintw(mframe,4,2,"<- para regresar ||  -> para agregar a carrito");
        mvwprintw(mframe, 6, 2, "Num     Nombre             Proveedor               Precio");
        // funcion de impresion de datos imprimeTxt();
        imprimeTxt("catalogo");
        wrefresh(mframe);

        opcion = wgetch(mframe);
        
        if(opcion == KEY_LEFT)
            return;
        else if(opcion == KEY_RIGHT){
            if(sesion == 1){
                echo();
                mvwprintw(mframe, 2, 35, "Introduzca el numero de producto:");
                wrefresh(mframe);
                wmove(mframe,2,68);
                wscanw(mframe,"%d",&producto);
                noecho();
                FILE * carrito;
                char carritoUsuario[100], txt_precio[100],txt_nombre[100], txt_proveedor[100];
                strcpy(carritoUsuario,usuActual);
                strcat(carritoUsuario,"_carrito");
                carrito = fopen(carritoUsuario,"a+");
                
                strcpy(txt_precio,obtenPrecio(producto));
                strcpy(txt_nombre,obtenNombre(producto));
                strcpy(txt_proveedor,obtenProveedor(producto));

                fprintf(carrito,"%s %s %s ",txt_nombre,txt_proveedor,txt_precio);
                fclose(carrito);

                mvwprintw(mframe, 3, 35, "Producto agregado con exito");
                wrefresh(mframe);
            }
            else{
                wclear(mframe);
                box(mframe,0,0);
                mvwprintw(mframe, 2, 2, "Para agregar productos al carrito inicie sesion");
                wgetch(mframe);
            }

        }
        wgetch(mframe);  
    }
}

char* obtenPrecio(int pos){
    FILE *txt;
    int i = (pos*3)+3;
    txt = fopen("catalogo", "r");
    
    while(i!=0){
        fscanf(txt,"%s",obpre);
        i-=1;
    }
    
    fclose(txt);
    return obpre;
}

char* obtenNombre(int pos){
    FILE *txt;
    int i = pos*3;
    txt = fopen("catalogo", "r");
    
    while(i>=0){
        fscanf(txt,"%s",obnom);
        i-=1;
    }
    
    fclose(txt);
    return obnom;
}

char* obtenProveedor(int pos){
    FILE *txt;
    int i = (pos*3)+1;
    txt = fopen("catalogo", "r");
    
    while(i>=0){
        fscanf(txt,"%s",obprov);
        i-=1;
    }
    fclose(txt);
    return obprov;
}

void cuenta(){
    int opcion = 0, registrado = 0, pos = 0;
    char menu[6][100];

    strcpy(menu[0], "Iniciar sesion");
    strcpy(menu[1], "Soy proveedor");
    strcpy(menu[2], "Cerrar sesion");
    strcpy(menu[3], "Detalles de usuario");
    strcpy(menu[4], "Crear Usuario");

    while(opcion != KEY_LEFT){
        wclear(mframe);
        box(mframe,0,0);

        wmove(mframe,2,2);
        wattron(mframe, A_REVERSE);
        mvwprintw(mframe, 2, 2, "Cuenta");
        wattroff(mframe, A_REVERSE);
        mvwprintw(mframe,4,30,"<- para regresar ||  -> para confirmar"); //aun no funciona confirmar
        mvwprintw(mframe,3,30,"arriba / abajo para desplazar");

        for(int i = 0; i<6 ;i++){

            if(i == pos){
              wattron(mframe, A_REVERSE);
            }
            mvwprintw(mframe, 5+i, 2, menu[i]);
            wattroff(mframe, A_REVERSE);
            wrefresh(mframe);
        }

        opcion = wgetch(mframe);

        if(opcion == KEY_LEFT)
            return;
        else if(opcion == KEY_UP){
            if(pos > 0)
                pos-=1;
        }
        else if(opcion == KEY_DOWN){
            if(pos <= 3)
                pos+=1;
        }

        if(opcion == KEY_RIGHT && pos == 0)
            iniciarSesion();
        else if(opcion == KEY_RIGHT && pos == 1)
            soyProveedor();
        else if(opcion == KEY_RIGHT && pos == 2)
            cerrarSesion();  
        else if (opcion == KEY_RIGHT && pos == 3)
        {
            detallesCuenta(); 
        }else if (opcion == KEY_RIGHT && pos == 4)
        {
            crearCuenta(); 
        }
        

        //Checar si hay algun cuenta iniciada
        //Menú de opciones segun la posicion
        //Darle funcion a cada opcion
        
    }


    return;
}

void crearCuenta(){
    char nombre[200], clave[200];
    echo();
    wclear(mframe);
    box(mframe,0,0);
    box(mframe,0,0); //Margen

    //if (No se ha iniciado sesion)
    mvwprintw(mframe,2,2,"Introduzca un nombre de usuario: ");
    wmove(mframe,2,40);
    wrefresh(mframe);
    wscanw(mframe,"%s",nombre);

    noecho();
    mvwprintw(mframe,3,2,"Introduzca una clave: ");
    wmove(mframe,3,40);
    wrefresh(mframe);
    wscanw(mframe,"%s",clave);

    //Creando txt con todos los datos del usuario
    
    FILE * usuario;
    usuario = fopen(nombre,"w");
    fprintf(usuario,"%s ",nombre);
    fprintf(usuario,"%s \n",clave);
    fclose(usuario);
    wattron(mframe,A_REVERSE);
    mvwprintw(mframe,8,x_max/2-20,"Usuario creado correctamente, Hola %s", nombre);
    wrefresh(mframe);
    wgetch(mframe);
    wattroff(mframe,A_REVERSE);
    noecho();

    FILE * carrito;
    strcat(nombre,"_carrito");
    carrito = fopen(nombre,"w");
    fclose(carrito);

    
    wrefresh(mframe);
    
    //else mvwprintw(mframe,8,x_max/2-10,"Usuario/Contraseña incorrectos");

    return;
}

void imprimeTxt(char archivo[100]){
    FILE *txt;
    char nombre[100],proveedor[100],precio[100];
    int i = 0, j = 0, total = 8;
    txt = fopen(archivo, "r");
    
    while(fscanf(txt,"%s",nombre) != EOF){

        fscanf(txt,"%s",proveedor);
        fscanf(txt,"%s",precio);

        mvwprintw(mframe, total,2,"%d    %s",i,nombre);
        mvwprintw(mframe, total,30,"%s",proveedor);
        mvwprintw(mframe, total,55,"%s",precio);
        wrefresh(mframe);
        
        i+=1;
        total+=1;
    }
    fclose(txt);
    return;
}

void iniciarSesion(){
    if(sesion == 0){
        echo();
        int opcion = 0;
        char usuario[100], contras[100];
        wclear(mframe);
        box(mframe,0,0);
        box(mframe,0,0); //Margen
        curs_set(1);

        //if (No se ha iniciado sesion)
        mvwprintw(mframe,2,2,"Bienvenido");
        mvwprintw(mframe,4,2,"Introduzca su usuario: ");
        wmove(mframe,4,25);
        wrefresh(mframe);
        wscanw(mframe,"%s",&usuario);

        noecho();
        mvwprintw(mframe,6,2,"Introduzca su clave: ");
        wmove(mframe,6,25);
        wrefresh(mframe);
        wscanw(mframe,"%s",&contras);

        //funcion chequeo de sesion
        //if(existe usuario y constraseña
        if(existeUsu(usuario) == 1 && codigoValido(usuario,contras) == 1){
            wattron(mframe,A_REVERSE);
            mvwprintw(mframe,8,x_max/2-10,"Sesion iniciada correctamente, Hola %s", usuario);
            wrefresh(mframe);
            wgetch(mframe);
            wattroff(mframe,A_REVERSE);
            curs_set(0);
            wrefresh(mframe);
            strcpy(usuActual,usuario);
            sesion = 1;
        }  
        else{
            msgContraErronea();
        }
        //else mvwprintw(mframe,8,x_max/2-10,"Usuario/Contraseña incorrectos");
    }
    else{
        msgActualmenteIniciado();
    }

    return;
}

void soyProveedor(){
    echo(); //para que se vea lo que se escribe al invocar un scanf
    curs_set(1); //Para que se vea la posicion actual del cursor
    int opcion = 0;
    char usuario[100], contras[100];
    wclear(mframe);
    box(mframe,0,0); //Margen
    

    //if (no se ha iniciado sesion)
    mvwprintw(mframe,2,2,"Bienvenido");
    iniciarSesion();

    //funcion chequeo de sesion
    //if(existe usuario y constraseña)
    if(strcmp(usuActual,"") != 0){
        wattron(mframe,A_REVERSE);
        mvwprintw(mframe,8,x_max/2-10,"Sesion iniciada correctamente, Hola %s", usuActual);
        wrefresh(mframe);
        wattroff(mframe,A_REVERSE);
        curs_set(0);
        wrefresh(mframe);
        menuProveedor();
    }
    curs_set(0);
    wrefresh(mframe);

    
    
    //else mvwprintw(mframe,8,x_max/2-10,"Usuario/Contraseña incorrectos");
    return;
}

void menuProveedor(){
    int opcion,pos = 0;
    char menu[6][100];

    strcpy(menu[0], "Agregar Producto");
    strcpy(menu[1], "Eliminar Producto");
    strcpy(menu[2], "Cerrar Sesion");
    strcpy(menu[3], "Cambiar Nombre");
    strcpy(menu[4], "Cambiar Clave");

    wclear(mframe);
    box(mframe,0,0); //Margen

    mvwprintw(mframe,2,2,"");
    wmove(mframe,4,35);
    wrefresh(mframe);

    while(1){
        wclear(mframe);
        wrefresh(mframe);
        box(mframe,0,0); //Margen
        
        wmove(mframe,2,2);
        mvwprintw(mframe, 2,2,"Tecla --> para confirmar");
        mvwprintw(mframe, 3,2,"Tecla <-- para regresar");
       
        for(int i = 0; i<4 ;i++){

            if(i == pos){
              wattron(mframe, A_REVERSE);
            }
            mvwprintw(mframe, 5+i, 2, menu[i]);
            wattroff(mframe, A_REVERSE);
            wrefresh(mframe);
        }

        opcion = wgetch(mframe);

        if(opcion == KEY_LEFT)
            return;
        else if(opcion == KEY_UP){
            if(pos > 0)
                pos-=1;
        }
        else if(opcion == KEY_DOWN){
            if(pos <= 2)
                pos+=1;
        }

        if(opcion == KEY_RIGHT && pos == 0)
            agregarProducto();
        else if(opcion == KEY_RIGHT && pos == 1){
            eliminarProducto();
            return;
        }
        else if(opcion == KEY_RIGHT && pos == 2){
            cerrarSesion();
            return;
        }else if(opcion == KEY_RIGHT && pos == 3){
            cambiarNombre();
        }else if(opcion == KEY_RIGHT && pos == 4){
            cambiarClave();
        }
    }
}

void agregarProducto(){
    echo();
    char nombreProd[100];
    int opciones = 0, precio = 0;
    //Ventana temporal
    wclear(mframe);
    box(mframe,0,0); //Margen

    mvwprintw(mframe,2,2,"Introduzca el nombre del producto:");
    wmove(mframe,2,40);
    wrefresh(mframe);
    wscanw(mframe,"%s",nombreProd);

    mvwprintw(mframe,3,2,"Introduzca el precio del producto:");
    wmove(mframe,3,40);
    wrefresh(mframe);
    wscanw(mframe,"%d",&precio);

    wmove(mframe,5,2);
    wattron(mframe,A_REVERSE);
    wrefresh(mframe);
    //Agregra producto en txt llamado catalogo con el siguiente formato nombre, proveedor, precio
    agregarCatalogo(nombreProd,usuActual,precio);
    mvwprintw(mframe,5,2,"Producto Agregado con Exito");
    wgetch(mframe);

    noecho();
    wattroff(mframe,A_REVERSE);
    wrefresh(mframe);
    return;
}

void agregarCatalogo(char nombreProd[100],char usuario[100],int precio){
    FILE * txtCatalogo;
    txtCatalogo = fopen("catalogo","a+");

    
    mvwprintw(mframe,7,(x_max/2)-6,"%s  %s  %d", nombreProd, usuario, precio);
    refresh();
    fprintf(txtCatalogo,"%s ",nombreProd);
    fprintf(txtCatalogo,"%s ",usuario);
    fprintf(txtCatalogo,"%d ",precio);
    fclose(txtCatalogo);
    return;
}

void eliminarProducto(){
    echo();
    char nombreProd[100];
    int opciones = 0, precio = 0;
    //Ventana temporal
    wclear(mframe);
    box(mframe,0,0); //Margen

    mvwprintw(mframe,2,2,"Introduzca el nombre del producto:");
    wmove(mframe,2,40);
    wrefresh(mframe);
    wscanw(mframe,"%s",nombreProd);

    wmove(mframe,5,2);
    wattron(mframe,A_REVERSE);
    wrefresh(mframe);
    //Agregra producto en txt llamado catalogo con el siguiente formato nombre, proveedor, precio
    
    if(estaEnTxt("catalogo",nombreProd)){
        eliminarCatalogo(nombreProd,usuActual);
        mvwprintw(mframe,5,2,"Producto eliminado con Exito");
        wgetch(mframe);
    }
    else{
        mvwprintw(mframe,5,2,"Producto no encontrado");
        wgetch(mframe);
    }

    noecho();
    wattroff(mframe,A_REVERSE);
    wrefresh(mframe);
    return;
}

void eliminarCatalogo(char nombreProd[100],char usuario[100]){
    char aux[100], aEliminar[100];
    FILE * us;
    FILE * destino;
    us = fopen("catalogo","r");
    destino = fopen("auxiliar","a+");

    while(fscanf(us,"%s",aux) != EOF){
        if(strcmp(aux,nombreProd) != 0){
            fprintf(destino,"%s ",aux);
        }
        else{
            for(int i = 0; i<3; i++){
                fscanf(us,"%s",aux);
            }
        }
    }
    fclose(us);
    strcat(aEliminar,".txt");
    remove(aEliminar);
    rename("auxiliar","catalogo");
    fclose(destino);
    return;
}

void cerrarSesion(){
    //Ventana temporal
    wclear(mframe);
    box(mframe,0,0); //Margen

    wattron(mframe,A_REVERSE);
    mvwprintw(mframe,2,2,"Cerrando sesion");
    //Codigo para checar variables semaforo y compartidas
    mvwprintw(mframe,4,2,"Sesion cerrada con Exito");
    wrefresh(mframe);
    wgetch(mframe);//Paro para ver que funcione todo
    wattroff(mframe,A_REVERSE);
    wrefresh(mframe);
    sesion = 0;
    strcpy(usuActual,"");
    return;
}

void detallesCuenta(){
    if(sesion == 1){
        int opcion,pos = 0;
        char menu[2][100];

        strcpy(menu[0], "Cambiar Usuario");
        strcpy(menu[1], "Cambiar Clave");

        wclear(mframe);
        box(mframe,0,0); //Margen

        mvwprintw(mframe,2,2,"");
        wmove(mframe,4,35);
        wrefresh(mframe);

        while(1){
            wclear(mframe);
            wrefresh(mframe);
            box(mframe,0,0); //Margen
            
            wmove(mframe,2,2);
            mvwprintw(mframe, 2,2,"Tecla --> para confirmar");
            mvwprintw(mframe, 3,2,"Tecla <-- para regresar");
            mvwprintw(mframe,1,2,"Hola %s",usuActual);
        
            for(int i = 0; i<2 ;i++){

                if(i == pos){
                wattron(mframe, A_REVERSE);
                }
                mvwprintw(mframe, 5+i, 2, menu[i]);
                wattroff(mframe, A_REVERSE);
                wrefresh(mframe);
            }

            opcion = wgetch(mframe);

            if(opcion == KEY_LEFT)
                return;
            else if(opcion == KEY_UP){
                if(pos > 0)
                    pos-=1;
            }
            else if(opcion == KEY_DOWN){
                if(pos <= 2)
                    pos+=1;
            }

            if(opcion == KEY_RIGHT && pos == 0)
                cambiarNombre();
            else if(opcion == KEY_RIGHT && pos == 1)
                cambiarClave();
            else if(opcion == KEY_LEFT){
                return;
            }
        }
    }
    else
        msgNoIniciado();
    return;
}

void cambiarNombre(){
    echo();
    char nombre[100], clave[100];
    //Ventana temporal
    wclear(mframe);
    box(mframe,0,0); //Margen

    mvwprintw(mframe,2,2,"Introduzca su nuevo nombre de usuario:");
    wmove(mframe,2,40);
    wrefresh(mframe);
    wscanw(mframe,"%s",&nombre);

    noecho();
    mvwprintw(mframe,3,2,"Confirme con su clave:");
    wmove(mframe,3,40);
    wrefresh(mframe);
    wscanw(mframe,"%s",&clave);

    wmove(mframe,5,2);
    wattron(mframe,A_REVERSE);
    wrefresh(mframe);

    if(codigoValido(usuActual,clave)){
        FILE * usuario;
        usuario = fopen(nombre,"w");
        fprintf(usuario,"%s %s",nombre, clave);
        mvwprintw(mframe,5,2,"Nombre modificado con exito");
        rename(usuActual, nombre);
        wgetch(mframe);
        strcpy(usuActual,nombre); 
        fclose(usuario);
    }
    else{
        mvwprintw(mframe,5,2,"Clave no valida");
        wgetch(mframe);
    }

    noecho();
    wattroff(mframe,A_REVERSE);
    wrefresh(mframe);

    return;
}

void cambiarClave(){
    noecho();
    char nclave[100], clave[100];
    //Ventana temporal
    wclear(mframe);
    box(mframe,0,0); //Margen

    mvwprintw(mframe,2,2,"Introduzca su nueva clave:");
    wmove(mframe,2,40);
    wrefresh(mframe);
    wscanw(mframe,"%s",&nclave);


    wclear(mframe);
    box(mframe,0,0); //Margen
    mvwprintw(mframe,2,2,"Introduzca su clave actual:");
    wmove(mframe,2,40);
    wrefresh(mframe);
    wscanw(mframe,"%s",&clave);


    if(codigoValido(usuActual,clave)){
        FILE * usuario;
        usuario = fopen(usuActual,"w");
        fprintf(usuario,"%s %s",usuActual, nclave);
        mvwprintw(mframe,5,2,"Clave modificada con exito");
        wgetch(mframe); 
        fclose(usuario);
    }
    else{
        mvwprintw(mframe,5,2,"Clave no valida");
        wgetch(mframe);
    }
    
    wattroff(mframe,A_REVERSE);
    wrefresh(mframe);
    return;
}

void msgNoIniciado(){
            wclear(mframe);
            wrefresh(mframe);
            box(mframe,0,0); //Margen
        
            wattron(mframe, A_REVERSE);
            mvwprintw(mframe,2,2,"NO HAY SESION EXISTENTE");
            wattroff(mframe, A_REVERSE);
            wrefresh(mframe);
            wgetch(mframe);
            return;
}

void msgContraErronea(){
            wclear(mframe);
            wrefresh(mframe);
            box(mframe,0,0); //Margen
        
            wattron(mframe, A_REVERSE);
            mvwprintw(mframe,2,2,"Clave/Usuario no valido");
            wattroff(mframe, A_REVERSE);
            wrefresh(mframe);
            wgetch(mframe);
            return;
}

void msgActualmenteIniciado(){
            wclear(mframe);
            wrefresh(mframe);
            box(mframe,0,0); //Margen
        
            wattron(mframe, A_REVERSE);
            mvwprintw(mframe,2,2,"NO PUEDE INICIAR SESION SI NO HA CERRADO LA EXISTENTE");
            wattroff(mframe, A_REVERSE);
            wrefresh(mframe);
            wgetch(mframe);
            return;
}

int existeUsu(char nombre[100]){
    FILE * usuario;
    if(usuario = fopen(nombre,"r")){
        fclose(usuario);
        return 1;
    }
    else{
        return 0;
    }
}

void msgNoExisteUsuario(){
            wclear(mframe);
            wrefresh(mframe);
            box(mframe,0,0); //Margen
        
            wattron(mframe, A_REVERSE);
            mvwprintw(mframe,2,2,"EL USUARIO QUE HA BUSCADO NO EXISTE");
            wattroff(mframe, A_REVERSE);
            wrefresh(mframe);
            wgetch(mframe);
            return;
}

int codigoValido(char nombre[100],char codigo[100]){

    char aux[100],aux2[100];
    FILE * usuario;
    usuario = fopen(nombre,"r");
    fscanf(usuario,"%s %s",aux,aux2);

    if(strcmp(codigo,aux2) == 0){
        return 1;
    }
    else
        return 0;
}

int estaEnTxt(char txt[100], char palabra[100]){

    char aux[100];
    FILE * usuario;
    usuario = fopen(txt,"r");
    while(fscanf(usuario,"%s",aux) != EOF){
        if(strcmp(aux,palabra) == 0){
            return 1;
        }
    }
    return 0;
}
