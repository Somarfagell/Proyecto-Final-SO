#include <stdio.h>
#include <ncurses.h>
#include <string.h>

void cuenta();
void catalogo();
void carrito();
void imprimeTxt();


WINDOW *mframe;
int y_max,x_max;

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
    char usuario[100];
    int tecla;
    strcpy(usuario,"Persona x");

    while(tecla != KEY_LEFT){
        wclear(mframe);
        wrefresh(mframe);
        box(mframe,0,0); //Margen
        

        wattron(mframe, A_REVERSE);
        mvwprintw(mframe,2,2,"CARRITO", usuario);
        wattroff(mframe, A_REVERSE);
        wrefresh(mframe);

        mvwprintw(mframe,4,3,"Hola usuario %s", usuario);
        mvwprintw(mframe,y_max-4,4,"<- para regresar ||  -> para pagar", usuario);
        mvwprintw(mframe,9,3,"Articulos en el carrito:");
        //Buscar al usuario e imprimir su txt

        wrefresh(mframe);
        tecla = wgetch(mframe);

        if(tecla == KEY_LEFT){
            return;
        }
        else if(tecla == KEY_RIGHT){
            wmove(mframe,7,2);
            wattron(mframe, A_REVERSE);
            mvwprintw(mframe, 7, 2, "ya esta todo pagadisimo :D          <- Menu || Seguir en carrito ->");
            wattroff(mframe, A_REVERSE);
            wrefresh(mframe);
            wgetch(mframe);
        }

    }
    
    wrefresh(mframe);
    return;
}

void catalogo(){
    int opcion;

    while(opcion != KEY_LEFT){
        wclear(mframe);
        box(mframe,0,0);
        wmove(mframe,2,2);
        wattron(mframe, A_REVERSE);
        mvwprintw(mframe, 2, 2, "Catalogo de productos");
        wattroff(mframe, A_REVERSE);

        mvwprintw(mframe,4,2,"<- para regresar ||  -> para agregar a carrito");
        mvwprintw(mframe, 6, 2, "Nombre             Proveedor               Precio");
        imprimeTxt();
        wrefresh(mframe);

        opcion = wgetch(mframe);
        
        if(opcion == KEY_LEFT)
            return;
        else if(opcion == KEY_RIGHT)
            mvwprintw(mframe, 2, 35, "Producto agregado con exito");
    
        wgetch(mframe);
    }

}

void cuenta(){
    int opcion = 0, registrado = 0, pos = 0;
    char menu[4][100];

    strcpy(menu[0], "Iniciar sesion");
    strcpy(menu[1], "Soy proveedor");
    strcpy(menu[2], "Cerrar sesion");
    strcpy(menu[3], "Detalles de usuario");

    while(opcion != KEY_LEFT){
        wclear(mframe);
        box(mframe,0,0);

        wmove(mframe,2,2);
        wattron(mframe, A_REVERSE);
        mvwprintw(mframe, 2, 2, "Cuenta");
        wattroff(mframe, A_REVERSE);
        mvwprintw(mframe,4,30,"<- para regresar ||  -> para confirmar"); //aun no funciona confirmar
        mvwprintw(mframe,3,30,"arriba / abajo para desplazar");

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

        //Checar si hay algun cuenta iniciada
        //Menú de opciones segun la posicion
        //Darle funcion a cada opcion
        
    }


    return;
}

void imprimeTxt(){
    return;
}