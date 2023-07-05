#include "principal.h"
#include "ui_principal.h"

#define DEFAULT_ANCHO 3

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    // Instanciando la imagen (creando)
    mImagen = new QImage(this->size(),QImage::Format_ARGB32_Premultiplied);
    // Rellenar la imagen de color blanco
    mImagen->fill(Qt::white);
    // Instanciar el Painter a partir de la imagen
    mPainter = new QPainter(mImagen);
    mPainter->setRenderHint(QPainter::Antialiasing);
    // Inicializar otras variables
    mPuedeDibujar = false;
    mColor = Qt::black;
    mAncho = DEFAULT_ANCHO;
    mNumLineas = 0;
    mModo = LIBRE;
}

Principal::~Principal()
{
    delete ui;
    delete mPainter;
    delete mImagen;
}

void Principal::paintEvent(QPaintEvent *event)
{
    // Crear el painter de la ventana principal
    QPainter painter(this);
    // Dibujar la imagen
    painter.drawImage(0, 0, *mImagen);
    // Acepatr el evento
    event->accept();
}

void Principal::mousePressEvent(QMouseEvent *event)
{
    // Levanta la bandera (para que se pueda dibujar)
    mPuedeDibujar = true;
    if (mModo!=LIBRE){
        event->accept();
        return;
    }
    // Captura la posición (punto x,y) del mouse
    mInicial = event->pos();
    // Acepta el evento
    event->accept();
}

void Principal::mouseMoveEvent(QMouseEvent *event)
{
    // Validar si se puede dibujar
    if ( !mPuedeDibujar ) {
        // Acepta el evento
        event->accept();
        // Salir del método
        return;
    }
    if (mModo!=LIBRE){
        event->accept();
        return;
    }
    // Capturar el punto a donde se mueve el mouse
    mFinal = event->pos();
    // Crear un pincel y establecer atributos
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    // Dibujar una linea
    mPainter->setPen(pincel);
    mPainter->drawLine(mInicial, mFinal);
    // Mostrar el número de líneas en la barra de estado
    ui->statusbar->showMessage(tr("Número de líneas: %1 || Modo: Libre").arg(++mNumLineas));
    // Actualizar la interfaz (repinta con paintEvent)
    update();
    // actualizar el punto inicial
    mInicial = mFinal;
}

void Principal::mouseReleaseEvent(QMouseEvent *event)
{
    switch (mModo){
    case LIBRE:
        // Bajar la bandera (no se puede dibujar)
        mPuedeDibujar = false;
        // Aceptar el vento
        event->accept();
        break;
    case LINEA:
        if (!mClic){
            mInicial = event->pos();
            mClic = true;
        }else if (mClic){
            mFinal = event->pos();
            // Crear un pincel y establecer atributos
            QPen pincel;
            pincel.setColor(mColor);
            pincel.setWidth(mAncho);
            // Dibujar una linea
            mPainter->setPen(pincel);
            mPainter->drawLine(mInicial, mFinal);
            ui->statusbar->showMessage(tr("Número de líneas: %1 || Modo: Linea").arg(++mNumLineas));
            // Actualizar la interfaz (repinta con paintEvent)
            update();
            mClic = false;
        }
        break;
    case RECTANGULO:
        if (!mClic){
            mInicial = event->pos();
            mClic = true;
        }else if (mClic){
            mFinal = event->pos();
            // Crear un pincel y establecer atributos
            QPen pincel;
            pincel.setColor(mColor);
            pincel.setWidth(mAncho);
            // Dibujar una linea
            mPainter->setPen(pincel);
            QRect rec(mInicial,mFinal);
            mPainter->drawRect(rec);
            ui->statusbar->showMessage(tr("Número de líneas: %1 || Modo: Rectangulo").arg(++mNumLineas));
            // Actualizar la interfaz (repinta con paintEvent)
            update();
            mClic = false;
        }
        break;
    case CIRCULO:
        if (!mClic){
            mInicial = event->pos();
            mClic = true;
        }else if (mClic){
            mFinal = event->pos();
            // Crear un pincel y establecer atributos
            QPen pincel;
            pincel.setColor(mColor);
            pincel.setWidth(mAncho);
            // Dibujar una linea
            mPainter->setPen(pincel);
            QRect rec(mInicial,mFinal);
            int d,a,c;
            a=mInicial.x()-mFinal.x();
            c=mInicial.y()-mFinal.y();
            d=pow(a,2)+pow(c,2);
            d=sqrt(d);
            mPainter->drawEllipse(mInicial,d,d);
            ui->statusbar->showMessage(tr("Número de líneas: %1 || Modo: Circunferencia").arg(++mNumLineas));
            // Actualizar la interfaz (repinta con paintEvent)
            update();
            mClic = false;
        }
        break;
    }
}


void Principal::on_actionAncho_triggered()
{
    mAncho = QInputDialog::getInt(this,
                                  "Ancho del pincel",
                                  "Ingrese el ancho del pincel de dibujo",
                                  mAncho,
                                  1, 20);
}

void Principal::on_actionSalir_triggered()
{
    this->close();
}

void Principal::on_actionColor_triggered()
{
    mColor = QColorDialog::getColor(mColor,
                                    this,
                                    "Color del pincel");
}

void Principal::on_actionNuevo_triggered()
{
    mImagen->fill(Qt::white);
    mNumLineas = 0;
    update();
}

void Principal::on_actionGuardar_triggered()
{
    // Abrir cuadro de diálogo para obtener el nombre del archivo
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar imagen",
                                                         QString(),
                                                         "Imágenes .png (*.png)");
    // Validar que el nombre del archivo no sea vacío
    if ( !nombreArchivo.isEmpty() ){
        // Guardar imagen
        if (mImagen->save(nombreArchivo)){
            // Si todo va bien, muestra un mensaje de información
            QMessageBox::information(this,
                                     "Guardar imagen",
                                     "Archivo almacenado en: " + nombreArchivo);
        } else{
            // Si hay algún error, muestro advertencia
            QMessageBox::warning(this,
                                 "Guardar imagen",
                                 "No se pudo almacenar la imagen.");
        }
    }
}

void Principal::on_actionLineas_triggered()
{
    mModo = LINEA;
}


void Principal::on_actionLibre_triggered()
{
    mModo = LIBRE;
}


void Principal::on_actionRect_nculos_triggered()
{
    mModo = RECTANGULO;
}


void Principal::on_actionCircunferencias_triggered()
{
   mModo = CIRCULO;
}

